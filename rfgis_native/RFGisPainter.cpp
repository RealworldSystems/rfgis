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

#include <RFGisPainter.h>

bool RFGisPainter::begin (QPaintDevice *device)
{
  if (QPainter::begin (device))
    { 
      RFGisPainter::setMobilityRenderHints (this);
      return true;
    }
  else
    { return false; }
}

void RFGisPainter::setMobilityRenderHints (QPainter *paint)
{
  paint->setRenderHint (QPainter::Antialiasing, true);
  paint->setRenderHint (QPainter::HighQualityAntialiasing, true);
  paint->setRenderHint (QPainter::SmoothPixmapTransform, true);
}
