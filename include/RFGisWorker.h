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

#if !defined (_RFGIS_WORKER_H)
#define _RFGIS_WORKER_H

#include <QtCore/QSize>
#include <QtCore/QSemaphore>
#include <QtCore/QMutex>
#include <QtCore/QThread>

#include <QtXml/QDomDocument>

#include <qgsmaprenderer.h>
#include <qgsproject.h>


class RFGisWorker;

class RFGisProjectWorker : public QObject
{
  Q_OBJECT
  
  friend class RFGisWorker;

public slots:
  void readProject (const QDomDocument &);
  void panMapByPixels (int start_x, int start_y, int end_x, int end_y);
  void scaleMap (int);
  void moveMapCenter (const QgsPoint &);
  void reset (void);
  void setLayerSet (const QStringList &);
  
private:
  RFGisProjectWorker (RFGisWorker *);
  RFGisWorker *mWorker_p;
  QgsMapRenderer &renderer (void);
  void moveExtent (double offset_x, double offset_y);
  void setExtent (const QgsRectangle &);
};

class RFGisWorker : public QObject
{
  Q_OBJECT

  friend class RFGisProjectWorker;

public slots:
  void doWork (void);

signals:
  void ready (const QImage &);

public:
  void setSize (const QSize &);
  void reset (void);
  void halt (void);
  QgsPoint pixelToCoordinate (int, int);
  QgsPoint coordinateToPixel (const QgsPoint &);
  QgsPoint centerCoordinate (void);
  QgsRectangle extent (void);
  
  static RFGisWorker &instance (void);
  static void destroy (void);

private:
  RFGisWorker (void);
  ~RFGisWorker (void);
  struct Data
  {
    QSize size;
    
    Data (void);
  };

  QSemaphore mSemaphore;
  QMutex mMutex;
  Data mData;
  QgsMapRenderer mRenderer;
  QThread mThread;
  bool mHalt;

  void init (void);
  Data getDataCopy (void);
};

#endif // _RFGIS_WORKER_H
