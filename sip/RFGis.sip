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

class RFGis : public QObject
{
%TypeHeaderCode
#include "../include/RFGis.h"
%End

private:
  RFGis ();

public:
  QString load_project (QString);
  int rotate (int);
  static RFGis * instance ();
  void scale (int);
  QPointF center ();
  QgsRectangle extent ();
  QPointF coordinateToPixel (double coordX, double coordY);
  void moveCenter (double coordX, double coordY);
  void setLayerSet (const QStringList &);
  void reset ();

signals:
  void rotateView (int rotation);
  void clickAndHold (double x, double y);
  void clicked (double x, double y);
  void ready (const QImage &image);
  void rendered ();
};
