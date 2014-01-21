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

#define QGISDEBUG

#include <string.h>

#include <RFGisPython.h>
#include <QDebug>
#include <QtGui/QMessageBox>
#include <QtCore/QThread>

#include <QtCore/QStringList>
#include <QtCore/QLibrary>
#include <QtCore/QString>
#include <QtCore/QStringBuilder>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>

#include <qgsapplication.h>
#include <qgsproject.h>
#include <qgsproviderregistry.h>
#include <qgsmaplayerregistry.h>
#include <qgsvectordataprovider.h>

#include <RFGisConfigure.h>
#include <RFGisWorker.h>

#include <RFGisQMLMap.h>

#if defined(HAVE_GETPID)
#if HAVE_GETPID == 1
#include <sys/types.h>
#include <unistd.h>
#endif
#endif

#include <stdio.h>
#include <string.h>

PyMODINIT_FUNC initrfgis (void);


static inline void 
RFGisInitialization (const QString & prefix_path,
			   const QString & plugin_path);

static inline void checkedImportModule (const char *name, bool decref = true);
static inline void configure (void);
static inline void preConfigure (void);

#if defined (HAVE_GETPID) && !defined (ANDROID) && HAVE_GETPID == 1
  // Salt python path with the current executable path of the /proc filesystem
  // is available.
static QString getBasePath (void)
{ 
  int pid = getpid();
  QString proc_name = QString("/proc/%1/exe").arg(pid);
  QString base_path;
  QFile proc_file(proc_name);
  if (proc_file.exists ())
    {
      QFileInfo info(proc_name);
      if (info.isSymLink ())
	{
	  QString link_target = info.symLinkTarget();
	  QString compilation_path = link_target.section ('/', -2, -2);
	  if (compilation_path == ".libs")
	    {
	      base_path += link_target.section('/', 0, -3);
	    }
	  else
	    {
	      base_path += link_target.section('/', 0, -2);
	      base_path += "/../lib/rfgis";
	    }
	}
    }
  return base_path;
}
#endif

/*
 * The initialization routine is responsible for delegating the necessary
 * initialization options to QGis Core. It is a replacement routine of the
 * original initialization routine.
 */
static inline void 
RFGisInitialization (const QString & prefix_path,
			   const QString & plugin_path)
{
  // The prefix path is responsible for recognizing where libraries are
  // put. In normal circumstances, the configure script will set this on
  // our config.h, however, if for some reason this isn't the proper
  // location, it can be overriden using the --prefix-path option.
  
  // Besides the prefix path, it is feasible to require the need of the
  // plugin path to be specified. Normally, the plugin path is determined
  // using the configure script, however, this might need to be overridden.
  
  // Due to the dependent layout of initialization, the first thing set is the
  // QgsProviderRegistry.

  qDebug() << "Plugin Path: " << plugin_path << "\n";
  qDebug() << "Prefix Path: " << prefix_path << "\n";

  QgsProviderRegistry::instance (plugin_path);
  QgsApplication::setPrefixPath (prefix_path);
  QgsApplication::setPluginPath (plugin_path);
  QStringList svgPaths;
  
#if defined (ANDROID)
  QgsApplication::setPkgDataPath (prefix_path + "/files");
  svgPaths << (prefix_path + "/files/application");
#else
  QgsApplication::setPkgDataPath (prefix_path);
  svgPaths << prefix_path;
#if defined (HAVE_GETPID) && HAVE_GETPID == 1
  QString basePath = getBasePath();
  qDebug() << "BasePath: " + basePath;
  svgPaths << basePath;
#endif
  QDir dir;
  svgPaths << dir.absolutePath();
#endif
  QgsApplication::setDefaultSvgPaths (svgPaths);
  QgsMapLayerRegistry::instance ();

  RFGisWorker::instance ();
}

static inline QString QgmPyRepr (PyObject *object)
{
  PyObject *repr = PyObject_Repr (object);
  QString s = PyString_AsString (repr);
  Py_XDECREF (repr);
  return s;
}

static inline void QgmPyDebug (PyObject *object)
{
  if (object == 0)
    {
      qDebug() << "Object is NULL" << "\n";
    }
  else
    {
      if (PyCallable_Check (object))
	{
	  qDebug() << "Next object is callable"  << "\n";
	}
      
      qDebug() << QgmPyRepr (object) << "\n";
    }
}


static inline bool checkTypeIsExit (PyObject *etype)
{
  PyObject *exceptions_module = PyImport_ImportModule("exceptions");
  PyObject *exceptions_module_dict = PyModule_GetDict (exceptions_module);
  PyObject *system_exit = PyDict_GetItemString (exceptions_module_dict,
						"SystemExit");
  bool result = (PyObject_RichCompareBool (system_exit, etype, Py_EQ) == 1);
  
  Py_XDECREF (exceptions_module);
  return result;
}

static inline void rfgisExit (int code)
{
  RFGisWorker::instance().halt();
  QApplication::processEvents ();
  exit(code);
}

static inline void checkedImportModule (const char *name, bool decref)
{
  qDebug () << "Attempting import of " << name << "\n";
  PyObject *config_module = PyImport_ImportModule (name);
  PyObject *etype, *evalue, *etraceback;
  etype = NULL;
  evalue = NULL;
  etraceback = NULL;
  PyErr_Fetch (&etype, &evalue, &etraceback);
  if (evalue != NULL)
    { 
      if (checkTypeIsExit (etype))
	{
	  Py_ssize_t the_code = PyNumber_AsSsize_t (evalue, NULL);
	  rfgisExit ((int)the_code);
	}
      else
	{
	  if (etraceback != NULL)
	    {
	      /* If the application ends up here, it means trouble */
	      PyObject *traceback_module = PyImport_ImportModule ("traceback");
	      PyObject *args = PyTuple_New (3);
	      PyObject *traceback_module_dict = PyModule_GetDict (traceback_module);
	      PyTuple_SetItem (args, 0, etype);
	      PyTuple_SetItem (args, 1, evalue);
	      PyTuple_SetItem (args, 2, etraceback);
	      PyObject *func = PyDict_GetItemString (traceback_module_dict, 
						     "format_exception");
	      PyObject *res = PyObject_CallObject (func, args);
	      if (res)
		{
		  for (Py_ssize_t i = 0; i < PyList_GET_SIZE (res); ++i)
		    {
		      PyObject *item = PyList_GET_ITEM (res, i);
		      QString s = PyString_AsString (item);
		      Py_XDECREF (item);
		      qDebug() << s;
		    }
		}
	  
	      Py_XDECREF (args);
	      Py_XDECREF (traceback_module);
	    }
	  rfgisExit (01);
	}
    }
  else if (decref)
    {
      Py_XDECREF (config_module);
    }

}

static inline void configure (void)
{
  checkedImportModule ("config.target", false);
}

static inline void preConfigure (void)
{
  initrfgis ();
  checkedImportModule ("rfgis.system.preconfig");
}

#if defined (ANDROID)

static inline void run_interactivenetconsole (void)
{
  checkedImportModule ("rfgis.system.interactivenetconsole.boot", false);
}

#endif


int runtime (int argc, char * argv[])
{
  QApplication app(argc, argv);

  qmlRegisterType <RFGisQMLMap> ("RFGis", 1, 0, "Map");


#if !defined (ANDROID)  
  setenv ("AUTOCONF_PROJECT_CODE_PATH", PROJECT_CODE_PATH, 1);
#else
  char *android_argv[3];
  android_argv[0] = argv[0];

  QString prefix_path = "--prefix-path=" % QString(getenv("PREFIX_PATH"));
  QString plugin_path = "--plugin-path=" % QString(getenv("PLUGIN_PATH"));
  QString app_path = QString(getenv("PREFIX_PATH")) % "/files/application";

  qDebug() << "Prefix Path: " << prefix_path;
  qDebug() << "Plugin Path: " << plugin_path;
  qDebug() << "Application: " << app_path;

  char *alloca_prefix = new char[prefix_path.size () + 1];
  char *alloca_plugin = new char[plugin_path.size () + 1];
  char *alloca_app = new char[app_path.size () + 1];

  memset (alloca_prefix, 0, prefix_path.size () + 1);
  memset (alloca_plugin, 0, plugin_path.size () + 1);
  memset (alloca_app, 0, app_path.size () + 1);

  QByteArray prefix_ba = prefix_path.toUtf8();
  QByteArray plugin_ba = plugin_path.toUtf8();
  QByteArray app_ba = app_path.toUtf8();

  memcpy(alloca_prefix, prefix_ba.constData (), prefix_path.size ());
  memcpy(alloca_plugin, plugin_ba.constData (), plugin_path.size ());
  memcpy(alloca_app, app_ba.constData (), app_path.size ());

  qDebug() << "Allocated Prefix" << QString(alloca_prefix);
  qDebug() << "Allocated Plugin" << QString(alloca_plugin);
  qDebug() << "Allocated Application" << QString(alloca_app);

  android_argv[1] = alloca_prefix;
  android_argv[2] = alloca_plugin;
  
  setenv ("AUTOCONF_PROJECT_CODE_PATH", alloca_app, 1);
#endif

  Py_Initialize();

#if !defined (ANDROID)
  PySys_SetArgv(argc, argv);
#else
  PySys_SetArgv(3, android_argv);  
#endif

  QString python_path = QString(getenv("PYTHONPATH"));
  
#if defined (HAVE_GETPID) && !defined (ANDROID) && HAVE_GETPID == 1
  if (python_path.size ())
    {
      python_path += ":" + getBasePath();
    }
  else
    {
      python_path = getBasePath();
    }
#endif

  if (python_path.size ()) python_path += ":";
  python_path += ".";

  char *python_path_buffer = 0;
  char path_object_name[5];

  qDebug() << "Additional python path: " << python_path;
  memcpy(path_object_name, "path", 5);

  PyObject *sys_path = PySys_GetObject(path_object_name);
  if (sys_path != 0)
    {
      QStringList paths_list = python_path.split(':');
      for (int i = 0; i < paths_list.size(); ++i)
	{
	  char *buffer = 0;
	  QByteArray b_path = paths_list[i].toLatin1();
	  const char *path = b_path.data();
	  size_t len = strlen(path) + 1;
	  buffer = new char[len];
	  memcpy(buffer, path, len);
	  PyList_Append(sys_path, PyString_FromString(buffer));
	  delete buffer;
	}
    }
  else
    {
      QByteArray b_path = python_path.toLatin1();
      const char *path = b_path.data();
      size_t len = strlen(path) + 1;
      python_path_buffer = new char[len];
      memcpy(python_path_buffer, path, len);
      PySys_SetPath(python_path_buffer);
    }

  PyEval_InitThreads();

  preConfigure ();

  qDebug() << "Preconfiguration Finished";

  RFGisConfigure config;
  
  RFGisInitialization (config.prefix_path (), config.plugin_path ());

  configure ();

#if defined (ANDROID)
  run_interactivenetconsole ();
#endif
  int code = app.exec ();
  rfgisExit(code);
  return code;
}
