// -*- c-file-style: "gnu"; c-basic-offset: 2 -*-

/*
 *  RFGis - Real Focus GIS
 *  Copyright (C) 2014  Realworld Software Products B.V.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <qgsmaplayerregistry.h>

#include <RFGisWorker.h>
#include <RFGisPainter.h>
#include <RFGis.h>

#include <QtCore/QMutexLocker>
#include <QtCore/QStringList>
#include <QtXml/QDomNodeList>
#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>

static RFGisProjectWorker *projectWorker_p = 0;
static RFGisWorker *worker_p = 0;
static double currentScale = 0;

RFGisWorker::RFGisWorker (void) :
  mSemaphore (0),
  mHalt (false),
  mMutex (QMutex::Recursive) { }

RFGisWorker::~RFGisWorker (void)
{
  this->halt();
}

RFGisProjectWorker::RFGisProjectWorker
(RFGisWorker *worker_p) :
  mWorker_p (worker_p) {}

RFGisWorker::Data::Data (void) :
  size (0, 0) { }



RFGisWorker &RFGisWorker::instance (void)
{
  if (::worker_p == 0)
    {
      ::worker_p = new RFGisWorker ();
      atexit (RFGisWorker::destroy);
      ::projectWorker_p = new RFGisProjectWorker(::worker_p);
      ::worker_p->init ();
    }
  
  return *(::worker_p);
}

void RFGisWorker::destroy (void)
{
  delete ::projectWorker_p;
  delete ::worker_p;
}

void RFGisWorker::init (void)
{
  connect (QgsProject::instance(), SIGNAL (readProject (const QDomDocument &)),
	   ::projectWorker_p,      SLOT (readProject (const QDomDocument &)));

  connect (RFGis::instance(), SIGNAL (panMapByPixels (int, int, int, int)),
	   ::projectWorker_p,       SLOT (panMapByPixels (int, int, int, int)));

  connect (RFGis::instance(), SIGNAL (scaleMap (int)),
	   ::projectWorker_p,       SLOT (scaleMap (int)));

  connect (RFGis::instance(), SIGNAL (moveMapCenter (const QgsPoint &)),
	   ::projectWorker_p,       SLOT (moveMapCenter (const QgsPoint &)));

  connect (RFGis::instance(), SIGNAL (resetMap ()),
	   ::projectWorker_p,       SLOT (reset()));

  connect (RFGis::instance(), SIGNAL (setLayerSetMap (const QStringList &)),
	   ::projectWorker_p,       SLOT (setLayerSet (const QStringList &)));

  connect (this,                    SIGNAL (ready (const QImage &)),
	   RFGis::instance(), SIGNAL (ready (const QImage &)));



  connect (&(this->mThread), SIGNAL (started ()), 
	   this,             SLOT (doWork ()));
  
  this->moveToThread (&(this->mThread));
  this->mThread.start ();
}

QgsMapRenderer &RFGisProjectWorker::renderer (void)
{
  return this->mWorker_p->mRenderer;
}

void RFGisProjectWorker::reset (void)
{
  return this->mWorker_p->reset ();
}


void RFGisProjectWorker::readProject (const QDomDocument &doc)
{
  QMutexLocker locker (&(this->mWorker_p->mMutex));

  QDomNodeList nodes = doc.elementsByTagName( "mapcanvas" );
  if ( nodes.count() )
    {
      QDomNode node = nodes.item(0);
      this->renderer().readXML (node);
    }

  this->setLayerSet(QgsMapLayerRegistry::instance()->mapLayers().keys());
}

void RFGisProjectWorker::setLayerSet (const QStringList &list)
{
  QMutexLocker locker (&(this->mWorker_p->mMutex));
  
  this->renderer().setLayerSet (list);
  this->reset ();
}

void RFGisProjectWorker::setExtent (const QgsRectangle &extent)
{
  QMutexLocker locker (&(this->mWorker_p->mMutex));

  // Set the extent of the renderer
  this->renderer().setExtent (extent);

  // Reset the host worker
  this->reset ();
}

void RFGisProjectWorker::moveMapCenter (const QgsPoint &point)
{
  QgsRectangle old_extent = this->renderer().extent();

  double width = old_extent.width();
  double height = old_extent.height();
  
  QgsRectangle new_extent (point.x() - (width / 2), point.y() - (width / 2),
			   point.x() + (width / 2), point.y() + (width / 2));

  this->setExtent (new_extent);
}

void RFGisProjectWorker::moveExtent (double offset_x, double offset_y)
{
  // Get the old extent and create a new extent based on the old extent using
  // the given offsets
  QgsRectangle old_extent = this->renderer().extent();
  
  QgsRectangle new_extent (old_extent.xMinimum () - offset_x,
			   old_extent.yMinimum () - offset_y,
			   old_extent.xMaximum () - offset_x,
			   old_extent.yMaximum () - offset_y);
 
  this->setExtent (new_extent);
}

void RFGisProjectWorker::panMapByPixels (int start_x, int start_y,
					       int end_x, int end_y)
{
  // First transform the given pixels to the coordinates on the map.
  QgsPoint start = this->mWorker_p->pixelToCoordinate(start_x, start_y);
  QgsPoint end = this->mWorker_p->pixelToCoordinate(end_x, end_y);
  
  // Determine the offsets from start -> end
  double offset_x = end.x() - start.x();
  double offset_y = end.y() - start.y();
  
  // Move the extent given the offsets
  this->moveExtent (offset_x, offset_y);
}

void RFGisProjectWorker::scaleMap (int scale)
{
  /* The original scale was set to the renderer scale by the renderer's 
     updateScale () routine. */

  double orig_scale = this->renderer().scale();

  /* The difference between the original scale and the destination scale
     is how the extent is going to be altered. */
  double delta_scale = ((double)scale) / orig_scale;

  /* Store the scale into the current scale, if the size has changed,
     it should restore to this scale */
  ::currentScale = scale;


  qDebug() << 
    "Performing scaling: " <<
    " orig: " << orig_scale <<
    " new: " << scale <<
    " delta: " << delta_scale;

  /* Given the center coordinate of old_extent, the values need to get
     closer to it or more distant from it (shrink or explode) */
  QgsRectangle extent = this->renderer().extent();
  extent.scale (delta_scale);
  
  // Set the extent
  this->setExtent (extent);
  
}

QgsPoint RFGisWorker::pixelToCoordinate (int x, int y)
{
  return this->mRenderer.coordinateTransform()->toMapCoordinates (x, y);  
}

QgsPoint RFGisWorker::coordinateToPixel (const QgsPoint &coord)
{
  return this->mRenderer.coordinateTransform()->transform (coord);
}

QgsPoint RFGisWorker::centerCoordinate (void)
{
  return this->mRenderer.extent().center();
}

QgsRectangle RFGisWorker::extent (void)
{
  return this->mRenderer.extent();
}

void RFGisWorker::setSize (const QSize &size)
{
  QMutexLocker locker (&(this->mMutex));
  this->mData.size = size;
  if (size.width () && size.height ())
    {
      this->mRenderer.setOutputSize (this->mData.size, 96);

      if (::currentScale != 0)
	{
	  ::projectWorker_p->scaleMap (currentScale);
	}
      else
	{
	  this->reset ();
	}
    }
  else
    {
      this->reset ();
    }
}

void RFGisWorker::reset (void)
{
  QMutexLocker locker (&(this->mMutex));
  if (!mSemaphore.available ())
    {
      qDebug() << "Performing Reset";
      mSemaphore.release ();
    }
}

void RFGisWorker::halt (void)
{
  this->mHalt = true; /* Boolean assignment is atomic */
  this->mSemaphore.release ();
  this->mThread.wait ();
}

RFGisWorker::Data RFGisWorker::getDataCopy (void)
{
  QMutexLocker locker (&(this->mMutex));
  return this->mData;
}

void RFGisWorker::doWork (void)
{
  do
    {
      qDebug() << "Re-enter";
      mSemaphore.acquire ();
      qDebug() << "Should halt?" << this->mHalt;
      
      /* If halt is set, work is done */
      if (! this->mHalt)
	{
	  Data data = this->getDataCopy ();
	  QImage image (data.size, QImage::Format_ARGB32);
	  
	  if (data.size.width () && data.size.height ())
	    {
	      image.fill (QColor (255, 255, 255).rgb ());
	      RFGisPainter paint;
	      paint.begin (&image);
	      paint.setClipRect (image.rect ());
	      this->mRenderer.render (&paint);
	      paint.end ();
	    }
  	  emit ready (image);
	}
    }
  while (! this->mHalt);
  qDebug() << "Halted";
  this->mThread.exit ();
  QCoreApplication::instance () -> processEvents ();
}
