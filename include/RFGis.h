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

#if !defined (_RFGIS_H)
#define _RFGIS_H

#include <QtCore/QObject>
#include <QtCore/QPointF>
#include <QtGui/QImage>
#include <qgsrectangle.h>
#include <qgspoint.h>

class RFGis : public QObject
{
  Q_OBJECT
  
private:
  RFGis (void);

public:
  QString load_project (QString);
  int rotate (int);
  bool panByPixels (int start_x, int start_y, int end_x, int end_y);
  void scale (int);
  QPointF center (void);
  QgsRectangle extent (void);
  QPointF coordinateToPixel (double, double);
  void moveCenter (double, double);
  void setLayerSet (const QStringList &);
  void reset (void);
  
  // Public but not visible to the python layer, as it is internal
  
  void mapClickAndHoldByPixels (int x, int y);
  void mapClickedByPixels (int x, int y);
    
  static RFGis * instance (void);

signals:
  
  void clickAndHold (double x, double y);
  void clicked (double x, double y);
  void ready (const QImage &);
  void rendered (void);
  void rotateView (int rotation);

  // Public but not visible to the python layer, as it is internal

  void panMapByPixels (int start_x, int start_y, int end_x, int end_y);
  void scaleMap (int);
  void resetMap (void);
  void setLayerSetMap (const QStringList &);
  void moveMapCenter (const QgsPoint &);
};

#endif // _RFGIS_H
