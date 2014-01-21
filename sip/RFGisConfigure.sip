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

class RFGisConfigure : public QObject
{
%TypeHeaderCode
#include "../include/RFGisConfigure.h"
%End

public:
  bool set_preconfiguration_dictionary (QMap<QString, QString>);
  QString default_plugin_path ();
  QString default_prefix_path ();
  QString plugin_path ();
  QString prefix_path ();
  QString load_path ();
  QMap<QString, QString> preconfiguration ();

};
