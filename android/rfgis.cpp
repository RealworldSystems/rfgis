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

#include <QtCore/QRegExp>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QStringBuilder>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>

#include <sys/types.h>
#include <unistd.h>

extern int runtime (int argc, char *argv[]);

static void setenv_from_qstring (const char *name, QString value)
{
  QByteArray byteArray = value.toUtf8();
  setenv (name, byteArray.constData(), 1);
}

static QString acquire_base_path (void)
{
  QString proc_path = "/proc/";
  QString maps_file = "/maps";

  QString pid_as_string = QString::number (getpid ());
  
  QString the_maps = proc_path % pid_as_string % maps_file;
  
  QFile file (the_maps);

  if (!file.open (QIODevice::ReadOnly | QIODevice::Text))
    {
      qDebug () << "FATAL: Don't know how to open: " << the_maps;
      exit (1);
    }

  QString base_path;

  QTextStream in (&file);

  QRegExp splitter ("^[^\\s]+\\s+[^\\s]+\\s+[^\\s]+\\s+[^\\s]+\\s+[^\\s]+\\s+");
  QString line;
  
  do
    {
      line = in.readLine ();
      
      if (line.isNull ())
	{
	  qDebug () << "FATAL: Can't determine base path";
	  exit (1);
	}

      // Find if line contains our library name (librfgis_android_wrap.so)
      
      QString end = line.split (splitter).last ();

      QStringList path_elements = end.split ("/");

      if (path_elements.last () == "librfgis.so")
	{
	  qDebug() << "Found ourselves: " << end;
	  int max_elements = path_elements.size () - 2;
	  for (int i = 0; i < max_elements; i++)
	    {
	      if (i + 1 == max_elements)
		{
		  base_path = base_path % path_elements [i];
		}
	      else
		{
		  base_path = base_path % path_elements [i] % "/";
		}
	    }
	  break;
	}
    }
  while (!line.isNull ());

  return base_path;
}

static void initialize_environ (const QString &base_path)
{
  QString files_path = base_path % "/files";
  QString python_home_path = files_path % "/python";

  QString library_path = base_path % "/lib";

  setenv_from_qstring ("PYTHONHOME", python_home_path);
  
  QString ld_library_path = 
    python_home_path % "/lib" % ":" %
    "/system/lib" % ":" %
    "/data/data/org.kde.necessitas.ministro/qt/lib";

  setenv_from_qstring ("LD_LIBRARY_PATH", ld_library_path);

  setenv_from_qstring ("PYTHONPATH", 
		       ld_library_path % ":" %
		       files_path % "/qgsmsystem" % ":" %
		       python_home_path % "/lib/python2.7/lib-dynload" % ":" %
		       python_home_path % "/lib/python2.7" % ":" %
		       python_home_path % "/lib/python2.7/site-packages");
  
  char *path = getenv ("PATH");
  
  setenv_from_qstring ("PATH",
		       QString(path) % ":" %
		       python_home_path % "/bin");

  setenv_from_qstring ("PREFIX_PATH", base_path);
  setenv_from_qstring ("PLUGIN_PATH", library_path);
  
}

int main (int argc, char *argv[])
{
  
  /* The idea here is to read a specific configuration file from a known
     location. It should be possible to know where we (this library) is
     located on the target device.
  */
  
  QString base_path = acquire_base_path ();
  initialize_environ (base_path);
  
  return runtime (argc, argv);
}
