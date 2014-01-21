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

%Module(name=rfgis, version=0)

%Platforms {ANDROID}

%If (ANDROID)
    typedef qint64 Q_PID;
%End


%Import QtCore/QtCoremod.sip
%Import QtGui/QtGuimod.sip
%Import core/core.sip

%If (Qt_4_2_0 -)

%Include RFGisConfigure.sip
%Include RFGis.sip

%End