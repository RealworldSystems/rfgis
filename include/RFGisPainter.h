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

#if !defined (_RFGIS_PAINTER_H)
#define _RFGIS_PAINTER_H

#include <QtGui/QPaintDevice>
#include <QtGui/QPainter>

class RFGisPainter : public QPainter
{
public:
  bool begin (QPaintDevice *);
  static void setMobilityRenderHints (QPainter *);
};

#endif // _RFGIS_PAINTER_H
