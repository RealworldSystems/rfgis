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

from interactivenetconsole import InteractiveNetConsoleServer
from PyQt4.QtCore import QObject
from PyQt4.QtCore import QTimer
from PyQt4.QtCore import pyqtSlot
from PyQt4.QtCore import qDebug
from socket import socket
import asyncore


class INCSConnector(QObject):
    """
    This class is used to connect an InteractiveNetConsoleServer to a
    free port on a given channel
    """

    class __AsyncoreQtBridge(QObject):
        """
        This class is instantiated once to use as bridge between asyncore
        and Qt when necessary
        """
        def __init__(self, timer):
            self.timer = timer
            QObject.__init__(self)
    
        @pyqtSlot()
        def pull(self):
            asyncore.loop(timeout=0.010, count=1)

        __bridge = None

        @staticmethod
        def __create_bridge():
            """
            Creates the bridge for asyncore and Qt
            """
            timer = QTimer()
            bridge = __AsyncoreQtBridge(timer)
            timer.setInterval(0)
            timer.timeout.connect(bridge.pull)
            timer.start()
            return bridge
        
        @classmethod
        def ensure_loop(cls):
            """
            Ensures that Qt takes responsibility for looping `asyncore' events
            whenever it sees fit.
            """
            if cls.__timer == None: cls.__bridge = cls.__create_bridge()

    def __init__(self, host, **kwargs):
        """
        The host used should be a host name or up address. The kwargs are
        optional and passed to the `InteractiveNetConsoleServer' class.
        See the `InteractiveNetConsoleServer.__init__' method for more
        information
        """
        QObject.__init__(self, address)
        self.delayed_args = kwargs
        self.host = host

    def __scan_free_port(self):
        """
        Attempts to find a free port in the range 8192..66535, and returns
        the first port found free
        """
        qDebug("Resolving free port for binding INCS")
        try:
            for port in range(1024*8, 65535):
                sock = socket(socket.AF_INET, socket.SOCK_STREAM)
                result = sock.connect_ex((self.host, port))
                if result == 0:
                    sock.close()
                    qDebug("Found port: {0}".format(port))
                    return port # Early return sending back the port

        except socket.gaierror:
            qDebug('Hostname could not be resolved. Exiting')
 
        except socket.error:
            qDebug("Couldn't connect to server")
            
        return None
        

    def connect_to_free():
        """
        Attempts to set up a connection to run the InteractiveNetConsoleServer
        on a free port listening on the given host. If given `localhost' or
        `127.0.0.1' this means only local connections are allowed. If given
        the proper network interface IP address or `0.0.0.0' connections
        from outside are allowed'
        """
        free_port = self.__scan_free_port()
        if free_port != None:
            __AsyncoreQtBridge.ensure_loop()
            return InteractiveNetConsoleServer((self.host, free_port),
                                               **self.delayed_args)
        else: return None
