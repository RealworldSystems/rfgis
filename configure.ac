#
# RFGis - Real Focus GIS
# Copyright (C) 2014  Realworld Software Products B.V.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

AC_INIT([Real Focus GIS], 
        [0.9],
        [sjoerd.van.leent@realworld-systems.com],
        [rfgis])

AC_PREREQ([2.59])

AC_CONFIG_HEADERS([rfgis_native/config.h])
AC_CANONICAL_HOST

AC_SUBST(HEADER_DIR, include)

AC_PROG_CXX
LT_INIT
AM_INIT_AUTOMAKE([1.10 no-define subdir-objects])
AM_SILENT_RULES([yes])
AC_CONFIG_MACRO_DIR([m4])

dnl ==== BASE PATH ====
dnl
dnl The base path indicates where ordinarily libraries, headers, etc. should
dnl be found. The base path can be modified so that it points to a different
dnl stack.

BASE_PATH=/usr

AC_ARG_WITH(
	[base-path],
	AS_HELP_STRING(
		[--with-base-path=[BASE_PATH]],
		[to specify the base path, default: /lib]),
	[BASE_PATH="$withval"],
	[])

dnl ==== QT Framework ====
dnl
dnl We need to determine where QT is hanging out. Sometimes, the headers
dnl are installed in a subdirectory, other times not.
dnl
dnl ---- QT Base directory ----
dnl
dnl QT is normally installed there where the BASE_PATH points. It can
dnl be overridden to use another QT infrastructure

QT_BASE_PATH=$BASE_PATH

AC_ARG_WITH(
	[qt-base-path],
	AS_HELP_STRING(
		[--with-qt-base-path=[QT_BASE_PATH]],
		[to specify the base path, default: BASE_PATH]),
	[QT_BASE_PATH="$withval"],
	[])

dnl
dnl ---- QT library path ----
dnl
dnl The QT libraries normally hang out in BASE_PATH/lib

QT_LIBRARY_PATH=$QT_BASE_PATH/lib

AC_ARG_WITH(
	[qt-library-path],
	AS_HELP_STRING(
		[--with-qt-library-path=[QT_LIBRARY_PATH]],
		[to specify the library path, default: $QT_BASE_PATH/lib]),
	[QT_LIBRARY_PATH="$withval"],
	[])

dnl
dnl ---- QT include path ----
dnl
dnl The QT headers normally hang out in BASE_PATH/include/qt4

QT_INCLUDE_PATH=$QT_BASE_PATH/include/qt4

AC_ARG_WITH(
	[qt-include-path],
	AS_HELP_STRING(
		[--with-qt-include-path=[QT_INCLUDE_PATH]],
		[to specify the include path, default: $QT_BASE_PATH/include/qt4]),
	[QT_INCLUDE_PATH="$withval"],
	[])

dnl
dnl ---- QT binary path ----
dnl
dnl The QT binaries normally hang out in BASE_PATH/bin


QT_BINARY_PATH=$QT_BASE_PATH/bin

AC_ARG_WITH(
	[qt-binary-path],
	AS_HELP_STRING(
		[--with-qt-binary-path=[QT_BINARY_PATH]],
		[to specify the include path, default: $QT_BASE_PATH/bin]),
	[QT_BINARY_PATH="$withval"],
	[])

dnl ==== QGIS ====
dnl
dnl We need to determine where QT is hanging out.
dnl
dnl ---- QGIS Base directory ----
dnl
dnl QT is normally installed there where the BASE_PATH points. It can
dnl be overridden to use another QT infrastructure

QGIS_BASE_PATH=$BASE_PATH

AC_ARG_WITH(
	[qgis-base-path],
	AS_HELP_STRING(
		[--with-qgis-base-path=[QGIS_BASE_PATH]],
		[to specify the base path, default: BASE_PATH]),
	[QGIS_BASE_PATH="$withval"],
	[])

dnl
dnl ---- QGIS library path ----
dnl
dnl The QGIS libraries normally hang out in BASE_PATH/lib

QGIS_LIBRARY_PATH=$QGIS_BASE_PATH/lib

AC_ARG_WITH(
	[qgis-library-path],
	AS_HELP_STRING(
		[--with-qgis-library-path=[QGIS_LIBRARY_PATH]],
		[to specify the library path, default: $QGIS_BASE_PATH/lib]),
	[QGIS_LIBRARY_PATH="$withval"],
	[])

dnl
dnl ---- QGIS include path ----
dnl
dnl The QGIS headers normally hang out in BASE_PATH/include/qgis

QGIS_INCLUDE_PATH=$QGIS_BASE_PATH/include/qgis

AC_ARG_WITH(
	[qgis-include-path],
	AS_HELP_STRING(
		[--with-qgis-include-path=[QGIS_INCLUDE_PATH]],
		[to specify the include path, default: $QGIS_BASE_PATH/include/qgis]),
	[QGIS_INCLUDE_PATH="$withval"],
	[])


dnl ==== PYTHON ====
dnl
dnl Python's folder structure is different from normal folder structures.
dnl it has it's header installed one entry further than the normal headers.
dnl
dnl ---- Base directory ----
dnl
dnl The base directory of python is normally
dnl set the base directory of python

PYTHON_BASE_PATH=$BASE_PATH

AC_ARG_WITH(
	[python-base-path],
	AS_HELP_STRING(
		[--with-python-base-path=[PYTHON_BASE_PATH]],
		[to specify the python base path, default: BASE_PATH]),
	[PYTHON_BASE_PATH="$withval"],
	[])

dnl
dnl ---- Python binary path ----
dnl
dnl We need the binary path to be able to get configuration items from SIP

AC_ARG_WITH(
	[python-binary-path],
	AS_HELP_STRING(
		[--with-python-binary-path=[PYTHON_BINARY_PATH]],
		[to specify the python base path, default: PYTHON_BASE_PATH/bin]),
	[PYTHON_BASE_PATH=${withval}],
	[PYTHON_BINARY_PATH=${PYTHON_BASE_PATH}/bin])

dnl
dnl ---- Python library path ----
dnl
dnl Set the library directory of python
PYTHON_LIBRARY_PATH=$PYTHON_BASE_PATH/lib

AC_ARG_WITH(
	[python-library-path],
	AS_HELP_STRING(
		[--with-python-library-path=[PYTHON_LIBRARY_PATH]],
		[to specify the path to the python library directory, default: $PYTHON_BASE_PATH/lib]),
	[PYTHON_LIBRARY_PATH="$withval"],
	[])

dnl
dnl ---- Python library name ----
dnl
dnl Set the library name of the python library to use in -l notation
PYTHON_LIBRARY=python2.7

AC_ARG_WITH(
	[python-library],
	AS_HELP_STRING(
		[--with-python-library=[PYTHON_LIBRARY]],
		[to specify the name of the library, default: python2.7]),
	[PYTHON_LIBRARY="$withval"],
	[])

dnl
dnl ---- Python include path ----
dnl
dnl Set the include directory of python, normally in PYTHON_BASE_PATH/include/python
PYTHON_INCLUDE_PATH=$PYTHON_BASE_PATH/include/python2.7

AC_ARG_WITH(
	[python-include-path],
	AS_HELP_STRING(
		[--with-python-include-path=[PYTHON_INCLUDE_PATH]],
		[to specify where the python libraries are, default: PYTHON_BASE_PATH/include/python2.7]),
	[PYTHON_INCLUDE_PATH="$withval"],
	[])

dnl
dnl ---- Option to enable whether or not to build an executable
dnl
dnl Devices don't always support directly invoking executables. This might be
dnl accomplished through an additional layer. By using this option, an
dnl executable binary is compiled to load the library. If not given,
dnl the configure script attempts to determine this itself.
AC_ARG_ENABLE(
	[executable],
	AS_HELP_STRING(
		[--enable-executable],
		[If you want to compile an aiding executable to the library]),
		[executable=${enableval}],
		[executable=maybe])

dnl
dnl ==== RFGis ====
dnl
dnl ---- Overrides the Runtime RFGis prefix path
dnl
dnl Ordinarily, on Unix systems, QGis lives at /usr, and is set to QGis Base 
dnl Path. RFGis uses this information to determine where to set
dnl the prefix path and the plugin path.

AC_ARG_WITH(
	[runtime-prefix-path],
	AS_HELP_STRING(
		[--with-runtime-prefix-path=QGIS_PREFIX_PATH],
		[to specify where the runtime should find the]
		[QGis 3rd party libraries.]
		[(Default: QGIS_BASE_PATH)]),
	[RUNTIME_PREFIX_PATH=${withval}],
	[RUNTIME_PREFIX_PATH=${QGIS_BASE_PATH}])

AC_DEFINE_UNQUOTED(
	[PREFIX_PATH],
	["$RUNTIME_PREFIX_PATH"],
	[Define the runtime prefix path])

dnl
dnl ---- Overrides the Runtime QGis plugin path
dnl
dnl Ordinarily, on Unix systems, the plugins of QGis live at 
dnl /usr/lib/qgis/plugins. This is used for both plugins as well as providers.

AC_ARG_WITH(
	[runtime-plugin-path],
	AS_HELP_STRING(
		[--with-runtime-plugin-path=QGIS_PLUGIN_PATH],
		[to speficy where the runtime should find the]
		[QGis plugins.]
		[(Default: RUNTIME_PREFIX_PATH/lib/qgis/plugins)]),
	[RUNTIME_PLUGIN_PATH=${withval}],
	[RUNTIME_PLUGIN_PATH=${RUNTIME_PREFIX_PATH}/lib/qgis/plugins])

AC_DEFINE_UNQUOTED(
	[PLUGIN_PATH],
	["$RUNTIME_PLUGIN_PATH"],
	[Define the runtime plugin path])

dnl
dnl ---- The path where the project code lives
dnl
dnl This should be set to be the project code directory. This could be different
dnl on the build host versus the target. This needs to have the explicit code
dnl for the target, rather than the build machine.

AC_ARG_WITH(
	[project-code-path],
	[AS_HELP_STRING(
		[--with-project-code-path=PROJECT_CODE_PATH],
		[to specify where the project code lives. The default points]
		[to the example path in the source folder])],
	[PROJECT_CODE_PATH=${withval}],
	[PROJECT_CODE_PATH=$srcdir/samples/centercontrol])

AC_DEFINE_UNQUOTED(
	[PROJECT_CODE_PATH],
	["$PROJECT_CODE_PATH"],
	[Define the project code path])


dnl
dnl ---- The path where the preconfiguration code lives
dnl
dnl This should be set to be the preconfiguration directory. It defaults to
dnl preconfig

AC_ARG_WITH(
	[preconfig-path],
	[AS_HELP_STRING(
		[--with-preconfig-path=PRECONFIG_PATH],
		[to specify where preconfiguration lives])],
	[PRECONFIG_PATH=${withval}],
	[PRECONFIG_PATH=$srcdir/qgsmsystem])

AC_DEFINE_UNQUOTED(
	[PRECONFIG_PATH],
	["$PRECONFIG_PATH"],
	[Define the preconfiguration path])


dnl
dnl ---- This is a modified version of AX_SIP_DEVEL, look in m4/
dnl
AX_SIP_DEVEL

dnl
dnl ---- Doing the SIP matter with Autoconf
dnl
dnl SIP has rich documentation, however, there appears to be little about
dnl how to integrate it in autoconf. It is clear that it depends on a valid
dnl python installation.
dnl
	 
AC_PATH_PROG([PYTHON], [python], [], [${PYTHON_BINARY_PATH}])

if test x$PYTHON == x ; then
   AC_MSG_ERROR([python required for generating SIP files])
fi

SIP_PATH=$srcdir/sip
SIP_GENERATED=`pwd`/sip/generated

mkdir -p $SIP_GENERATED

ac_sip_build_file="$SIP_GENERATED/rfgis.sbf"
ac_py_import="from PyQt4 import pyqtconfig"
ac_py_pyqt_sip_flags="pyqtconfig.Configuration().pyqt_sip_flags"
ac_py_pyqt_sip_dir="pyqtconfig.Configuration().pyqt_sip_dir"


dnl
dnl ---- Override options for PyQt4 flags
dnl
dnl The normal python routine returns the sip flags from the PyQt4 package,
dnl however, if these need to be adapted to fit a certain configuration, this
dnl can be done by setting the proper flags
dnl
AC_ARG_WITH(
	[pyqt4-flags],
	[AS_HELP_STRING(
		[--with-pyqt4-flags=PYQT4_FLAGS],
		[to specify what flags SIP should be using for PyQt4])],
	[PYQT4_FLAGS=${withval}],
	[PYQT4_FLAGS=`$PYTHON -c "$ac_py_import; print $ac_py_pyqt_sip_flags"`])

dnl
dnl ---- Override options for PyQt4 SIP directory
dnl
dnl The normal python routine returns the directory where the PyQt4 SIP files
dnl are. If for some reason these are not correct, it should be possible to
dnl override this behavior.

AC_ARG_WITH(
	[pyqt4-dir],
	[AS_HELP_STRING(
		[--with-pyqt4-dir=PYQT4_DIR],
		[to specify in which directory PyQt4's SIP module files]
		[are installed])],
	[PYQT4_DIR=${withval}],
	[PYQT4_DIR=`$PYTHON -c "$ac_py_import; print $ac_py_pyqt_sip_dir"`])

dnl
dnl ---- Specify additional SIP include directories
dnl
dnl Normally, these are the SIP directories we need. If additional directories
dnl are necessary, they should be configured here

AC_ARG_WITH(
	[xtra-sip-dirs],
	[AS_HELP_STRING(
		[--with-xtra-sip-dirs=EXTRA_SIP_DIRS],
		[to specify additional SIP directories, separated with colons])],
	[XTRA_SIP_DIRS=${withval}],
	[XTRA_SIP_DIRS=])

dnl
dnl ---- Specify the URL of a QGIS raw tree, where release 1.8 is located
dnl
dnl Normally, the raw URL is on GitHub, however, if for some reason this doesnt
dnl work as expected, it should be possible to override it.
dnl

AC_ARG_WITH(
	[raw-qgis-url],
	[AS_HELP_STRING(
		[--with-raw-qgis-url=URL],
		[to specify the raw URL of QGIS with release 1.8
		 Defaults to https://raw.github.com/qgis/QGIS/release-1_8])],
	[RAW_QGIS_URL=${withval}],
	[RAW_QGIS_URL=https://raw.github.com/qgis/QGIS/release-1_8])

dnl The python path in the raw QGUS url is used to download the SIP files of
dnl QGIS.

AC_ARG_WITH(
	[raw-sip-qgis-url],
	[AS_HELP_STRING(
		[--with-raw-sip-qgis-url=URL],
		[to specify the raw URL of the QGIS 1.8 sip directory
		 defaulting to raw-qgis-url/python])],
	[RAW_SIP_QGIS_URL=${withval}],
	[RAW_SIP_QGIS_URL=${RAW_QGIS_URL}/python])


SIP_FEATURES=

AC_PATH_XTRA

LIB_DIRS="-L$PYTHON_LIBRARY_PATH -L$QT_LIBRARY_PATH -L$QGIS_LIBRARY_PATH"

QT_INCLUDES="\
	-I$QT_INCLUDE_PATH \
	-I$QT_INCLUDE_PATH/Qt \
	-I$QT_INCLUDE_PATH/QtCore \
	-I$QT_INCLUDE_PATH/QtXml \
	-I$QT_INCLUDE_PATH/QtGui \
	-I$QT_INCLUDE_PATH/QtDeclarative"

dnl TODO Need to find ways to do autoconf checks on these
QT_LIBS="-lQtGui -lQtCore -lQtXml -lQtDeclarative -lQtSensors"

dnl TODO Need to find wais to do autoconf checks on -lz and -lm

EXT_LIBS="$QT_LIBS -lz -lm $LIB_DIRS $X_LIBS $X_EXTRA_LIBS"
EXT_INCLUDES="$QT_INCLUDES -I$QGIS_INCLUDE_PATH -I$PYTHON_INCLUDE_PATH"
EXT_FLAGS="$X_CFLAGS -DQT_CORE_LIB -DQT_SHARED -DCORE_EXPORT= -DGUI_EXPORT="

AC_SUBST(QT_BINARY_PATH, $QT_BINARY_PATH)
AC_SUBST(EXT_LIBS, $EXT_LIBS)
AC_SUBST(PLUGIN_DIR, $PLUGIN_DIR)
AC_SUBST(PYTHON_INCLUDE, $PYTHON_INCLUDE)
#AC_SUBST(QT_LIBS, $QT_LIBS)

export INCLUDES="$INCLUDES $EXT_INCLUDES"
export PATH="$PATH:$QT_BINARY_PATH"
export CXXFLAGS="$CXXFLAGS $EXT_INCLUDES $EXT_FLAGS"
export LDFLAGS="$LDFLAGS"
export LIBS="$LIBS $EXT_LIBS"

export LD_RUN_PATH=$QT_LIBRARY_PATH

AC_CHECK_LIB(
	[qgis_core], [GEOPROJ4], ,
	AC_MSG_ERROR([QGis core library not found]))

AC_CHECK_LIB(
	[$PYTHON_LIBRARY], [Py_Initialize], ,
	AC_MSG_ERROR([Python library not found]))
	
AC_CHECK_HEADER(
	[$PYTHON_INCLUDE_PATH/Python.h], ,
	AC_MSG_ERROR([Python header not found]),
	AC_INCLUDES_DEFAULT)

AC_CHECK_FUNCS(getpid)


# Now we check whether we can actually build a Qt app.
cat > myqt.h << EOF
#include <QtCore/QObject>
class Test : public QObject
{
Q_OBJECT
public:
  Test() {}
  ~Test() {}
public slots:
  void receive() {}
signals:
  void send();
};
EOF

cat > myqt.cpp << EOF
#include "myqt.h"
#include <QtGui/QApplication>
int main( int argc, char **argv )
{
  QApplication app( argc, argv );
  Test t;
  QObject::connect( &t, SIGNAL(send()), &t, SLOT(receive()) );
}
EOF

AC_MSG_CHECKING(does moc work?)
bnv_try_1="$QT_BINARY_PATH/moc myqt.h -o moc_myqt.cpp"
AC_TRY_EVAL(bnv_try_1)
if test x"$ac_status" != x0; then
   AC_MSG_ERROR(moc doesn't work)
fi
AC_MSG_RESULT(yes)
AC_MSG_CHECKING(can I compile moc_myqt.cpp?)
bnv_try_2="$CXX -c $CXXFLAGS $EXTLIBS -o moc_myqt.o moc_myqt.cpp"
AC_TRY_EVAL(bnv_try_2)
if test x"$ac_status" != x0; then
   AC_MSG_ERROR(couldn't compile moc_myqt.cpp)
fi
AC_MSG_RESULT(yes)
AC_MSG_CHECKING(can I compile myqt.cpp?)
bnv_try_3="$CXX $QTINC -c $CXXFLAGS $EXTLIBS -o myqt.o myqt.cpp"
AC_TRY_EVAL(bnv_try_3)
if test x"$ac_status" != x0; then
   AC_MSG_ERROR(couldn't compile myqt.cpp)
fi
AC_MSG_RESULT(yes)
AC_MSG_CHECKING(can I link against QT?)
nv_try_4="$CXX $LIBS -o myqt myqt.o moc_myqt.o"
AC_TRY_EVAL(bnv_try_4)
if test x"$ac_status" != x0; then
   AC_MSG_ERROR(couldn't link)
fi
AC_MSG_RESULT(yes)

AC_MSG_CHECKING(for mkoctfile)
AC_TRY_EVAL(mkoctfile)
if test x"$ac_status" != x0; then
   AC_MSG_ERROR(mkoctfile is not in the path)
fi
AC_MSG_RESULT(yes)
rm -f moc_myqt.cpp myqt.h myqt.cpp myqt.o myqt moc_myqt.o


AC_DEFUN([ANDROID_ME], [dnl
   echo "This is an Android targetting build"
   AC_DEFINE([ANDROID], [1], [We are targetting an ANDROID device])
   export android_wrapper=yes
   export LDFLAGS="$LDFLAGS -avoid-version"   

   AC_CONFIG_COMMANDS(
	[libtool-rpath-patch],
	[echo config.status: patching libtool to not use rpath
	 sed < libtool > libtool-2 's/^hardcode_libdir_flag_spec.*$'/'hardcode_libdir_flag_spec=" -D__LIBTOOL_NO_RPATH__ "/'
	 mv libtool-2 libtool
	 chmod 755 libtool])
])  

if echo "$host $host_alias" | grep android >/dev/null ; then
   ANDROID_ME
   SIP_FEATURES="-t ANDROID"
   dnl Inform Automake that we want an executable if executable is set to yes
   AM_CONDITIONAL(EXECUTABLE, test x$executable == xyes)
else
   dnl Inform Automake that we want an executable if executable is not set to no
   AM_CONDITIONAL(EXECUTABLE, test x$executable != xno)
fi

dnl Build the android wrapper library code if the android wrapper is
dnl selected. This is done by the ANDROID_ME function.
AM_CONDITIONAL(ANDROID_WRAPPER, test x"$android_wrapper" == xyes)

dnl
dnl -- Expose additional verbosity/silence rules to the Makefile
dnl
dnl These lines are used by the makefile or by the MOC/SIP compilation 
dnl routines. It is better to create them here as it is easier for 
dnl generic application.
dnl

VERBOSITY_DEFINE_FROM_TO([MOC])
VERBOSITY_DEFINE([WGET])
VERBOSITY_DEFINE([SIP])
VERBOSITY_DEFINE([ECHO])
VERBOSITY_DEFINE([RM])
VERBOSITY_DEFINE([MV])
VERBOSITY_DEFINE([MKDIR])

VERBOSITY_SUBST()


dnl
dnl -- Creation macros for compiling MOC and SIP CPP files into a library
dnl

dnl
dnl Creates a line representing a makefile line triggered prior to a given
dnl dependency.
dnl

AC_DEFUN([RFGIS_LO_LINE], [

am_v_cxx_flag="A""M_V_CXX"
am_v_lt_flag="A""M_V_lt"
an_environment="\$($am_v_cxx_flag) \$(LIBTOOL) \$($am_v_lt_flag)"

lo_line="$1":" $2"$'\n'
lo_line=$lo_line$'\t'"$an_environment --tag=CXX "
lo_line=$lo_line"$flags --mode=compile "
lo_line=$lo_line"\$(CXX) \$(DEFS) \$(DEFAULT_INCLUDES) \$(INCLUDES) "
lo_line=$lo_line"\$(librfgis_la_CPPFLAGS) \$(CPPFLAGS) \$(librfgis_la_CXXFLAGS) "
lo_line=$lo_line"\$(CXXFLAGS) -c -o $1 \`test -f $2 || echo '\$(srcdir)/'\`$2"$'\n'$'\n'

lo_line=$lo_line"librfgis_la_SOURCES += $2"$'\n'
lo_line=$lo_line"librfgis_la_OBJECTS += $1"$'\n'

])

dnl
dnl Creates several lines to bind a linked object to a parent linked object
dnl for initiation. This is the main method to create several lines into the 
dnl makefile to instruct how to compile, link and conversely remove the necessary
dnl files. A typical call requires three parameters:
dnl
dnl [dependency] - The name of the parent .lo file or other wise parent 
dnl                Makefile rule
dnl [objectfile] - The name of the object file which needs to be created
dnl                (and linked)
dnl [sourcefile] - The name of the source file to be used
dnl

AC_DEFUN([RFGIS_LO_BIND], [

ac_io=`echo "$2" | sed "s/\\.lo$/.o/"`
add_section="$1":" $2"
rm_section="rm_$2: "$'\n'
rm_section="$rm_section"$'\t'"rm -f $2"$'\n'
rm_section="$rm_section"$'\t'"rm -f $ac_io"$'\n'
rm_section="$rm_section"$'\t'"rm -f $3"
clean_section="clean: rm_$2"

RFGIS_LO_LINE([$2], [$3])

lo_bind="$lo_line"$'\n'"$add_section"$'\n'"$clean_section"$'\n'"$rm_section"$'\n'

])


dnl
dnl QGis uses SIP files, however, these files are difficult to come by as they 
dnl are not part of the binary distribution. The SIP files therefore need to
dnl be gathered from the internet. This is done by the tooling "wget" through
dnl the generated makefile.
dnl

SIP_PROPER_XTRA_SIP_DIRS=

SIP_PROPER_XTRA_SIP_DIRS_COUNT=`echo -n "$XTRA_SIP_DIRS" | wc -m`

if test x$SIP_PROPER_XTRA_SIP_DIRS_COUNT != x0 ; then
   SIP_PROPER_XTRA_SIP_DIRS=`echo "$XTRA_SIP_DIRS" | sed 's/:/ -I /'`
   SIP_PROPER_XTRA_SIP_DIRS="-I $SIP_PROPER_XTRA_SIP_DIRS"
fi

SIP_RUN="$SIP -c $SIP_GENERATED -b $ac_sip_build_file $SIP_FEATURES \
	      $SIP_PROPER_XTRA_SIP_DIRS -I $PYQT4_DIR -I $SIP_PATH/qgis \
	      -x QSETINT_CONVERSION -x QSETTYPE_CONVERSION \
	      $PYQT4_FLAGS $SIP_PATH/RFGismod.sip"

AC_SUBST(SIP_RUN)

ac_sip_files="\
	analysis/analysis.sip\
	analysis/network/networkanalysis.sip\
	analysis/network/qgsarcproperter.sip\
	analysis/network/qgsdistancearcproperter.sip\
	analysis/network/qgsgraph.sip\
	analysis/network/qgsgraphanalyzer.sip\
	analysis/network/qgsgraphbuilder.sip\
	analysis/network/qgsgraphbuilderintr.sip\
	analysis/network/qgsgraphdirector.sip\
	analysis/network/qgslinevectorlayerdirector.sip\
	analysis/qgsgeometryanalyzer.sip\
	analysis/qgsoverlayanalyzer.sip\
	analysis/qgszonalstatistics.sip\
	core/conversions.sip\
	core/core.sip\
	core/qgis.sip\
	core/qgsaddremoveitemcommand.sip\
	core/qgsapplication.sip\
	core/qgsattributeaction.sip\
	core/qgscomposerattributetable.sip\
	core/qgscomposeritem.sip\
	core/qgscomposeritemcommand.sip\
	core/qgscomposerlabel.sip\
	core/qgscomposerlegend.sip\
	core/qgscomposermap.sip\
	core/qgscomposerpicture.sip\
	core/qgscomposerscalebar.sip\
	core/qgscomposershape.sip\
	core/qgscomposertable.sip\
	core/qgscomposition.sip\
	core/qgscontexthelp.sip\
	core/qgscontinuouscolorrenderer.sip\
	core/qgscontrastenhancement.sip\
	core/qgscontrastenhancementfunction.sip\
	core/qgscoordinatereferencesystem.sip\
	core/qgscoordinatetransform.sip\
	core/qgsdataprovider.sip\
	core/qgsdatasourceuri.sip\
	core/qgsdiagramrendererv2.sip\
	core/qgsdistancearea.sip\
	core/qgsexception.sip\
	core/qgsexpression.sip\
	core/qgsfeature.sip\
	core/qgsfield.sip\
	core/qgsgeometry.sip\
	core/qgsgpsconnection.sip\
	core/qgsgpsconnectionregistry.sip\
	core/qgsgpsdconnection.sip\
	core/qgsgpsdetector.sip\
	core/qgsgraduatedsymbolrenderer.sip\
	core/qgslabel.sip\
	core/qgslabelattributes.sip\
	core/qgslegendmodel.sip\
	core/qgslogger.sip\
	core/qgsmaplayer.sip\
	core/qgsmaplayerregistry.sip\
	core/qgsmaprenderer.sip\
	core/qgsmaptopixel.sip\
	core/qgsmarkercatalogue.sip\
	core/qgsmessagelog.sip\
	core/qgsmessageoutput.sip\
	core/qgsnetworkaccessmanager.sip\
	core/qgsnmeaconnection.sip\
	core/qgsoverlayobject.sip\
	core/qgspaperitem.sip\
	core/qgspluginlayer.sip\
	core/qgspluginlayerregistry.sip\
	core/qgspoint.sip\
	core/qgsproject.sip\
	core/qgsprojectversion.sip\
	core/qgsprovidermetadata.sip\
	core/qgsproviderregistry.sip\
	core/qgsrasterbandstats.sip\
	core/qgsrasterdataprovider.sip\
	core/qgsrasterlayer.sip\
	core/qgsrasterpyramid.sip\
	core/qgsrastershader.sip\
	core/qgsrastershaderfunction.sip\
	core/qgsrastertransparency.sip\
	core/qgsrasterviewport.sip\
	core/qgsrect.sip\
	core/qgsrendercontext.sip\
	core/qgsrenderer.sip\
	core/qgsrunprocess.sip\
	core/qgsscalebarstyle.sip\
	core/qgsscalecalculator.sip\
	core/qgssearchstring.sip\
	core/qgssearchtreenode.sip\
	core/qgssinglesymbolrenderer.sip\
	core/qgssnapper.sip\
	core/qgsspatialindex.sip\
	core/qgssymbol.sip\
	core/qgssymbologyutils.sip\
	core/qgstolerance.sip\
	core/qgsuniquevaluerenderer.sip\
	core/qgsvectordataprovider.sip\
	core/qgsvectorfieldsymbollayer.sip\
	core/qgsvectorfilewriter.sip\
	core/qgsvectorlayer.sip\
	core/qgsvectorlayerimport.sip\
	core/qgsvectoroverlay.sip\
	core/symbology-ng-core.sip\
	gui/gui.sip\
	gui/qgisinterface.sip\
	gui/qgsannotationitem.sip\
	gui/qgsattributeeditor.sip\
	gui/qgscolorbutton.sip\
	gui/qgscomposerview.sip\
	gui/qgsencodingfiledialog.sip\
	gui/qgsfieldvalidator.sip\
	gui/qgsformannotationitem.sip\
	gui/qgsgenericprojectionselector.sip\
	gui/qgslegendinterface.sip\
	gui/qgsmapcanvas.sip\
	gui/qgsmapcanvasitem.sip\
	gui/qgsmapcanvasmap.sip\
	gui/qgsmapcanvassnapper.sip\
	gui/qgsmapoverviewcanvas.sip\
	gui/qgsmaptip.sip\
	gui/qgsmaptool.sip\
	gui/qgsmaptoolemitpoint.sip\
	gui/qgsmaptoolpan.sip\
	gui/qgsmaptoolzoom.sip\
	gui/qgsmessageviewer.sip\
	gui/qgsprojectbadlayerguihandler.sip\
	gui/qgsprojectionselector.sip\
	gui/qgsquerybuilder.sip\
	gui/qgsquickprint.sip\
	gui/qgsrubberband.sip\
	gui/qgsscalecombobox.sip\
	gui/qgssearchquerybuilder.sip\
	gui/qgstextannotationitem.sip\
	gui/qgsvertexmarker.sip\
	gui/symbology-ng-gui.sip"

sip_header=
sip_section=

for sip_file in $ac_sip_files
do
	src="\$(top_srcdir)/sip/qgis/$sip_file"
	sip_mkdir=$'\t'"@mkdir -p `dirname $src`"
	external_sip_file="$RAW_SIP_QGIS_URL/$sip_file"
	
	getter=$'\t'"\$(WGET_V) wget -q $external_sip_file -O $src.tmp"$'\n'
	getter=$getter$'\t'"@mv $src.tmp $src"

	sip_section="$sip_section"$'\n'"$src":$'\n'"$sip_mkdir"$'\n'"$getter"$'\n'
	sip_header="$sip_header $src"
done


sip_almagation="\$(top_builddir)/sip/generated/almagation.cpp"
sip_object="sip/generated/almagation.lo"
sip_inter_object="sip/generated/almagation.o"
sip_sbf="sip/generated/rfgis.sbf"

dnl
dnl To create the almagation file, it is necessary to perform the following
dnl steps:
dnl
dnl Check if the almagation file exists and whether or not any of the base
dnl SIP files changed. If any of the SIP files changed, the almagation needs
dnl to be removed.
dnl

sip_script="@echo -n ;( should_sip=\"\" ; "
sip_script=$sip_script"if test -f $sip_almagation ; then "
sip_script=$sip_script"for file_name in \`ls \$(top_srcdir)/sip/*.sip\` ; do "
sip_script=$sip_script"if test \`stat -c %Y \$\$file_name\` -gt \`stat -c %Y $sip_almagation\`; then "
sip_script=$sip_script"echo \"  SIP    \$\$file_name has changed\" ; "
sip_script=$sip_script"should_sip=\"t\" ; "
sip_script=$sip_script"fi ;"
sip_script=$sip_script"done ; "
sip_script=$sip_script"else "
sip_script=$sip_script"echo \"  SIP    No almagation file found\" ; "
sip_script=$sip_script"should_sip=\"t\" ; "
sip_script=$sip_script"fi ; "
sip_script=$sip_script"if test x\$\$should_sip != x ; then "
sip_script=$sip_script"mkdir -p \$(SIPTEMP_DIR) ; "
sip_script=$sip_script"rm -rf \$(SIPTEMP_DIR)/*.cpp ; "
sip_script=$sip_script"rm -rf \$(SIPTEMP_DIR)/*.h ; "
sip_script=$sip_script"echo \"  SIP    $< > $sip_almagation\" ; "
sip_script=$sip_script"if test x\$(V) != x -o x\$(AM_DEFAULT_VERBOSITY) != x0 ; then "
sip_script=$sip_script"echo $SIP_RUN ; "
sip_script=$sip_script"fi ; "
sip_script=$sip_script"$SIP_RUN ; "
sip_script=$sip_script"cat \$(SIPTEMP_DIR)/*.cpp > $sip_almagation.tmp ; "
sip_script=$sip_script"sip_alma_size=\`stat -c %s $sip_almagation.tmp\` ; "

sip_script=$sip_script"if test x\$\$sip_alma_size != x0 ; then "
sip_script=$sip_script"mv $sip_almagation.tmp $sip_almagation ; "
sip_script=$sip_script"fi ; "

sip_script=$sip_script"fi ) "


sip_section="$sip_almagation: $sip_header \$(top_srcdir)/sip/*.sip"$'\n'$'\t'$sip_script$'\n'"$sip_section"


RFGIS_LO_BIND([rfgis_native/librfgis_la-RFGisMain.lo], [$sip_object], [$sip_almagation])

dnl add_section="rfgis_native/librfgis_la-RFGisMain.lo: $sip_object"
dnl rm_section="rm_$sip_object: "$'\n'$'\t'"rm -f sip_object"$'\n'$'\t'"rm -f $sip_almagation"$'\n'$'\t'"rm -f $sip_inter_object"$'\n'$'\t'"rm -rf \$(top_srcdir)/sip/qgis"$'\n'$'\t'"rm -f $sip_sbf"
dnl clean_section="clean: rm_$sip_object"

cruft_rm_section="rm_almagation_cruft:"$'\n'
cruft_rm_section="$cruft_rm_section"$'\t'"rm -rf \$(top_srcdir)/sip/qgis"$'\n'
cruft_rm_section="$cruft_rm_section"$'\t'"rm -f $sip_sbf"$'\n'

cruft_clean_section="clean: rm_almagation_cruft"

sip_section="$sip_section"$'\n'"$lo_bind"$'\n'"$cruft_rm_section"$'\n'"$cruft_clean_section"



AM_SUBST_NOTMAKE(sip_section)
AC_SUBST(sip_section)

dnl
dnl The moc files are typically build when an object requires one. These files
dnl come from pre-compilation of given moc information on the C++ header
dnl and source files.
dnl
dnl A C++ file needs moc compilations when satisfying the following:
dnl
dnl - The file contains the macro SIGNAL
dnl - The file contains the macro SLOT
dnl

ac_moc_sources=`find rfgis_native -name "*.cpp" -exec grep -l "emit" {} \; -exec grep -l "SIGNAL" {} \; -exec grep -l "SLOT" {} \; | grep -v "/generated/" | sort -u`

ac_moc_headers=`find include -name "*.h" -exec grep -l "Q_OBJECT" {} \; | sort -u | sed "s|include/\(.*\)\\.h|rfgis_native/\\1.cpp|" | sort -u`

ac_moc_sources=`echo "${ac_moc_sources}"$'\n'"${ac_moc_headers}" | sort -u`

moc_section=
moc_sources=
lo_line=

for moc_source in $ac_moc_sources
do
	moc_cpp=`echo $moc_source | sed "s|rfgis_native/\(.*\)\\.cpp|rfgis_native/generated/moc_\\1.cpp|"`
	echo "auto-discovery MOC generation for: $moc_cpp"
	
	moc=`echo $moc_source | sed "s|rfgis_native/\(.*\)\\.cpp|rfgis_native/generated/moc_\\1.cpp: \$\(top_srcdir\)/include/\\1.h \$\(top_srcdir\)/rfgis_native/\\1.cpp|"`
	moc_mkdir=$'\t'"@\$(MKDIR_P) \"$(dirname $moc_cpp)\""
	moc_gen=$'\t'"\$(MOC_V) $QT_BINARY_PATH/moc \$< -o \$@"
	moc_section="$moc_section""$moc"$'\n'"$moc_mkdir"$'\n'"$moc_gen"$'\n'
	moc_object=`echo $moc_source | sed "s|rfgis_native/\(.*\)\\.cpp|rfgis_native/generated/librfgis_la-moc_\\1.lo|"`
	moc_inter_object=`echo $moc_source | sed "s|rfgis_native/\(.*\)\\.cpp|rfgis_native/generated/librfgis_la-moc_\\1.o|"`

	base_object=`echo $moc_source | sed "s|rfgis_native/\(.*\)\\.cpp|rfgis_native/librfgis_la-\\1.lo|"`
	
	RFGIS_LO_BIND([$base_object], [$moc_object], [$moc_cpp])

	moc_section=$moc_section$'\n'$lo_bind$'\n'
done

moc_section=$moc_sources$'\n'$moc_section

AM_SUBST_NOTMAKE(moc_section)
AC_SUBST(moc_section)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
