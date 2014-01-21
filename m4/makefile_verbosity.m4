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

#
# This file is used to create verbosity prefixes for Makefiles.
# To create a verbosity one should either call:
#
# VERBOSITY_DEFINE([aname])
# -- or --
# VERBOSITY_DEFINE_FROM_TO([aname])
#
# To define the verbosity in the makefile, call VERBOSITY_SUBST()
#
# The parameter aname should have a string with capitals, further,
# it should be less than or equal to 6 characters and it can only contain
# ASCII-characters A to Z.
#

extra_verbosity_rules=

dnl
dnl _AX_INTERNAL_ANAME_IS_STRING([aname]) -- PRIVATE
dnl
dnl Checks if the given parameter "aname" is a proper character array
dnl
dnl INFO: Should not be used from outside the scope of this file
dnl
AC_DEFUN([_AX_INTERNAL_ANAME_IS_STRING], [

pushdef([error_str], [Procedure should be called with parameter a-name])
pushdef([aname], [$1])

AS_IF([test x"aname" == x""], [AC_MSG_ERROR([error_str])])

popdef([aname])
popdef([error_str])
])

dnl
dnl _AX_INTERNAL_VERBOSTY_DEFINE([aname], [display]) -- PRIVATE
dnl
dnl Produces the actual code and stores it into the variable 
dnl extra_verbosity_rules. This method does NO error checking as it
dnl expects aname and display to be correct.
dnl
dnl INFO: Should not be used from outside the scope of this file
dnl
AC_DEFUN([_AX_INTERNAL_VERBOSITY_DEFINE], [

dnl aname needs to be padded. If aname's length < 6 then it needs to
dnl be filled with spaces to get to 6 characters.

pushdef([aname], $1)
pushdef([shorthand], [`printf "  %-7s" "$1"`])
pushdef([display], $2)

extra_verbosity_rules="$extra_verbosity_rules""aname""_V = \$(aname""_V_\$(V))"$'\n'
extra_verbosity_rules="$extra_verbosity_rules""aname""_V_ = \$(aname""_V_\$(AM_DEFAULT_VERBOSITY))"$'\n'
extra_verbosity_rules="$extra_verbosity_rules""aname""_V_0 = @echo \""shorthand"display""$""@\";"$'\n'$'\n'

popdef([display])
popdef([shorthand])
popdef([aname])

])


dnl
dnl Produces a verification procedure (VERBOSITY_ANAME_CHECK) which needs
dnl the argument aname and checks if the spelling is correct.
dnl
AC_DEFUN([VERBOSITY_ANAME_CHECK], [

_AX_INTERNAL_ANAME_IS_STRING([$1])

pushdef([error_str], [aname should be at most 6 characters and only characters A-Z])
pushdef([valid_1], [`echo $1 | sed "s/@<:@A-Z@:>@*/\\0:/" | cut -d ":" -f1`])
pushdef([valid_2], [`expr length "$1"`])

AS_IF([!(test valid_2 -lt 7 -a valid_1 == "$1")], [AC_MSG_ERROR([error_str])])

popdef([valid_2])
popdef([valid_1])
popdef([error_str])
])

dnl
dnl VERBOSITY_DEFINE([aname])
dnl
dnl Produces the rules to define the verbosity given the aname. Only
dnl a valid aname can be used as defined in the function aname_check.
dnl The rule will produce a silencer with $@ as it's argument in the
dnl Makefile and should be used in the case of generation.
dnl
AC_DEFUN([VERBOSITY_DEFINE], [

# The call should typically be VERBOSITY_DEFINE(aname). Note that
# @<:@ and @:>@ will be replaced by autom4te at the and of processing.

VERBOSITY_ANAME_CHECK([$1])

 _AX_INTERNAL_VERBOSITY_DEFINE([$1], [])
])

dnl
dnl VERBOSITY_DEFINE_FROM_TO([aname])
dnl
dnl Produces the rules to define the verbosity given the aname. Only
dnl a valid aname can be used as defined in the function aname_check.
dnl The rule will produce a silencer with $< > $@ as it's argument in the
dnl Makefile and should be used in the case of transformation.
dnl
AC_DEFUN([VERBOSITY_DEFINE_FROM_TO], [

# The call should typically be VERBOSITY_DEFINE(aname)
VERBOSITY_ANAME_CHECK([$1])

_AX_INTERNAL_VERBOSITY_DEFINE([$1], [\$< > ])
])

dnl
dnl VERBOSITY_SUBST
dnl
dnl Produces the verbosities up to now as as AC_SUBST but marked as
dnl AM_SUBST_NOTNAME. In files using this substitution such as
dnl the Makefile.am, this will replace the line @extra_verbosity_rules@
dnl
AC_DEFUN([VERBOSITY_SUBST], [
AM_SUBST_NOTMAKE(extra_verbosity_rules)
AC_SUBST(extra_verbosity_rules)
])
