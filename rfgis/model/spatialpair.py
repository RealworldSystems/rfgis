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

# QObject is the parent object of all Qt related objects
from PyQt4.QtCore import QObject

# QgsDataProvider is the implementing class used for data providers. Each 
# instance of QgsDataProvider can either be a QgsVectorDataProvider or 
# otherwise, but all need to be subclass instances or instances of 
# QgsDataProvider
from qgis.core import QgsDataProvider

# QgsFeature is the implementing class for features as understood by QGis
from qgis.core import QgsFeature

# Exception is the base class of all exceptions
from exceptions import Exception

class SpatialPair(QObject):
    """
    A spatial pair is a composition of a two values:
    
    - A data provider which contains a pointer into the database concerned
      about the data of a particular feature
    
    - The feature itself
    """

    def __init__(self, data_provider, feature):
        """
        Initializes a spatial pair using a data provider and a feature
        """
        self.__data_provider = data_provider
        self.__feature = feature
        QObject.__init__(self)
        self.verify()

    def data_provider(self):
        """
        Returns the data provider contained in this object
        """
        return self.__data_provider
        
    def dataProvider(self): 
        """
        see SpatialPair.data_provider
        """
        return self.__data_provider
    
    def feature(self): 
        """
        Returns the feature contained in this object
        """
        return self.__feature

    def valid(self):
        """
        Returns True if the data provider and feature instances are instances
        of QgsDataProvider and QgsFeature. If this is not so, False is returned
        instead.
        """
        return (isinstance(self.__data_provider, QgsDataProvider) and
                isinstance(self.__feature, QgsFeature) and
                self.__feature.isValid())

    def verify(self):
        """
        Raises an InvalidSpatialPairException when the object is invalid, that
        is, when the `valid' method returns False
        """
        if not self.valid(): 
            raise InvalidSpatialPairException(self.__data_provider, 
                                              self.__feature)


class InvalidSpatialPairException(Exception):
    """
    An InvalidSpatialPairException is raised whenever a SpatialPair object
    is malformed by not having a proper data provider and/or feature.
    """
    
    def __init__(self, data_provider, feature):
        Exception.__init__(self)
        self.message = (
            "Could not create spatial pair with data provider: {0}"
            "and feature: {1}").format(data_provider, feature)
            
    
