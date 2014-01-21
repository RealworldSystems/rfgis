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

#include "config.h"

#include <QtCore/QFileInfo>

#include <RFGis.h>
#include <RFGisWorker.h>
#include <RFGisQMLMap.h>
#include <qgsproject.h>
#include <qgspoint.h>


static RFGis * instance = 0;

RFGis * RFGis::instance (void)
{
  if (::instance == 0)
    {
      ::instance = new RFGis ();
    }
  return ::instance;
}

RFGis::RFGis (void)
{
}


QString RFGis::load_project (QString projectfile)
{
  QgsProject *project = QgsProject::instance ();
  QFileInfo fileInfo = QFileInfo (projectfile);
  project->read (fileInfo);
  QString error = project->error ();
  return error;
}


int RFGis::rotate (int rotation)
{
  rotation = rotation % 360;
  if (rotation > 180) { rotation -= 360; }
  if (rotation <= -180) { rotation += 360; }

  emit rotateView (rotation);
  return rotation;
}

bool RFGis::panByPixels (int start_x, int start_y, int end_x, int end_y)
{
  if (start_x != end_x || start_y != end_y)
    {
      emit panMapByPixels (start_x, start_y, end_x, end_y);
      return true;
    }
  else
    {
      return false;
    }
}

void RFGis::mapClickAndHoldByPixels (int x, int y)
{
  QgsPoint point = RFGisWorker::instance().pixelToCoordinate(x, y);
  emit clickAndHold (point.x (), point.y ());
}

void RFGis::mapClickedByPixels (int x, int y)
{
  QgsPoint point = RFGisWorker::instance().pixelToCoordinate(x, y);
  emit clicked (point.x (), point.y ());
}

void RFGis::scale (int scale)
{
  emit scaleMap (scale);
}

void RFGis::moveCenter (double coordX, double coordY)
{
  emit moveMapCenter (QgsPoint(coordX, coordY));
}

QPointF RFGis::center (void)
{
  QgsPoint point = RFGisWorker::instance().centerCoordinate();
  return QPointF((qreal)(point.x ()), (qreal)(point.y ()));
}

QgsRectangle RFGis::extent (void)
{
  return RFGisWorker::instance().extent();
}

QPointF RFGis::coordinateToPixel (double coordX, double coordY)
{
  QgsPoint phase1 = RFGisWorker::instance().coordinateToPixel 
    (QgsPoint(coordX, coordY));
  
  QPointF phase2 = QPointF (phase1.x(), phase1.y());
  RFGisQMLMap *map = RFGisQMLMap::getDefault();
    if (map)
      {
	return map->renderPixelToMapPixel (phase2);
      }
    else
      {
	return QPointF (0, 0);
      }
}

void RFGis::reset (void)
{
  emit resetMap();
}

void RFGis::setLayerSet(const QStringList &list)
{
  emit setLayerSetMap (list);
}
