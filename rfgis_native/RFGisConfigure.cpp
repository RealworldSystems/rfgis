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

#include <RFGisConfigure.h>

RFGisConfigure::RFGisConfigure (void)
{
}


static QMap<QString, QString> _systemDictionary;

static QString system_dictionary_entry (QString);

bool
RFGisConfigure::set_preconfiguration_dictionary
(QMap<QString, QString> dict)
{
  if (_systemDictionary.empty ())
    {
      _systemDictionary = dict;
      return true;
    }
  else
    {
      return false;
    }
}

QString RFGisConfigure::default_plugin_path (void)
{
  return PLUGIN_PATH;
}

QString RFGisConfigure::default_prefix_path (void)
{
  return PREFIX_PATH;
}

static QString system_dictionary_entry (QString key)
{
  if (_systemDictionary.empty ())
    {
      return "";
    }
  else
    {
      return _systemDictionary[key];
    }
}

QString RFGisConfigure::plugin_path (void)
{
  return system_dictionary_entry("plugin_path");
}

QString RFGisConfigure::prefix_path (void)
{
  return system_dictionary_entry("prefix_path");
}

QString RFGisConfigure::load_path (void)
{
  return system_dictionary_entry("load_path");
}

QMap<QString, QString> RFGisConfigure::preconfiguration (void)
{
  return _systemDictionary;
}
