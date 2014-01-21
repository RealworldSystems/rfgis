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

#if !defined (_RFGIS_CONFIGURE_H)
#define _RFGIS_CONFIGURE_H

#include <QObject>
#include <QMap>
#include <QString>

class RFGisRouter;

class RFGisConfigure : public QObject
{
  Q_OBJECT

public:
  RFGisConfigure (void);
  bool set_preconfiguration_dictionary (QMap<QString, QString>);
  QString default_plugin_path (void);
  QString default_prefix_path (void);
  QString plugin_path (void);
  QString prefix_path (void);
  QString load_path (void);
  QMap<QString, QString> preconfiguration (void);
  bool route (RFGisRouter);
};


#endif // _RFGIS_CONFIGURE_H
