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

ACLOCAL_AMFLAGS = -I m4

@extra_verbosity_rules@

lib_LTLIBRARIES=
bin_PROGRAMS=

FORCE_USE_PIC=1

QTEMP_DIR = $(top_srcdir)/rfgis_native/generated
QITEMP_DIR = $(top_srcdir)/include/generated
SIPTEMP_DIR = sip/generated

TESTS_ENVIRONMENT = PYTHONPATH='$(top_srcdir)'; export PYTHONPATH;
PY_LOG_COMPILER = $(PYTHON)
TEST_EXTENSIONS = .py

lib_LTLIBRARIES += librfgis.la
librfgis_la_SOURCES = \
	rfgis_native/RFGisQMLMap.cpp \
        rfgis_native/RFGisMain.cpp \
	rfgis_native/RFGisConfigure.cpp \
	rfgis_native/RFGis.cpp \
	rfgis_native/RFGisWorker.cpp \
	rfgis_native/RFGisPainter.cpp

librfgis_la_CPPFLAGS = $(DEPS_CFLAGS) -I$(top_srcdir)/include $(SIP_CPPFLAGS)
librfgis_la_CXXFLAGS = $(DEPS_CFLAGS) -I$(top_srcdir)/include $(SIP_CPPFLAGS)
librfgis_la_LIBADD = $(DEPS_LIBS)

# Only build executables if we enabled the option during configuration

if EXECUTABLE

# Made this for temporary test purposes

bin_PROGRAMS += rfgis-bin
rfgis_bin_SOURCES = \
	programs/rfgis.cpp
rfgis_bin_CPPFLAGS = $(DEPS_CFLAGS) $(AM_CFLAGS)
rfgis_bin_CFLAGS = $(DEPS_CFLAGS) $(AM_CFLAGS)
rfgis_bin_LDADD = $(DEPS_LIBS) $(top_builddir)/librfgis.la

endif

if ANDROID_WRAPPER

# Made for wrapping the runtime function to read parameters and set 
# the system environment through an XML configuration file.

lib_LTLIBRARIES += librfgis_android_wrap.la
librfgis_android_wrap_la_SOURCES = android/rfgis.cpp

endif

clean-local:
	@echo "  RM      $(QTEMP_DIR)/*" ; rm -rf $(top_srcdir)/rfgis/generated/*
	@echo "  RM      $(SIPTEMP_DIR)/*.cpp"; rm -rf $(SIPTEMP_DIR)/*.cpp
	@echo "  RM      $(SIPTEMP_DIR)/*.h"; rm -rf $(SIPTEMP_DIR)/*.h


install-exec-hook: install-system
uninstall-hook: uninstall-system


install-system:
	$(ECHO_V) $(MAKE) uninstall-system
	$(ECHO_V) $(MKDIR_P) "$(DESTDIR)$(libdir)/rfgis"
	$(COPY_V) cp -R "$(top_srcdir)/rfgis" "$(DESTDIR)$(libdir)/rfgis"

uninstall-qgsmsystem:
	$(RM_V) rm -rf "$(DESTDIR)$(libdir)/rfgis"

# Tests can only be performed if the build produced a valid executable
# and the executable can be used on the target system (build triplet
# must thus be the same as the host triplet)

TESTS=

if UNIT_TESTING_ENABLED

TESTS += tests/unittests/spatialpair_test.py 
TESTS += tests/unittests/spatialfinder_test.py

endif


@moc_section@

@sip_section@
