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

# Author: Sjoerd van Leent

# QObject is the parent object of all Qt related objects
from PyQt4.QtCore import QObject

# QgsDataProvider is the implementing class used for data providers. Each 
# instance of QgsDataProvider can either be a QgsVectorDataProvider or 
# otherwise, but all need to be subclass instances or instances of 
# QgsDataProvider
from qgis.core import QgsDataProvider

# QgsFeature is the implementing class for features as understood by QGis
from qgis.core import QgsFeature

# Geometries of `QgsFeature' instances are created with the `QgsGeometry'
# helper class
from qgis.core import QgsGeometry

# QgsPoint is the implementing class for points, used primarily for
# coordinates.
from qgis.core import QgsPoint

# QgsRectangle is the implementing class for rectangles, such as extents
from qgis.core import QgsRectangle

# `QgsMapLayer' instances are necessary for the spatialfinder to be able
# to run a given behavior for a `SpatialFinder' instance's `find()' method
from qgis.core import QgsMapLayer

# QgsPoint requires a valid number to be used, convertible to the float type
from numbers import Number

# Exception is the base class of all exceptions
from exceptions import Exception

# The SpatialPair is used for binding a data provider and a feature
from rfgis.model.spatialpair import SpatialPair

class Behavior(QObject):
    """
    This behavior does not define any real behavior, but can be
    used as superclass for actual behavior implementation.
    """
    
    def __init__(self):
        """
        Initializes the behavior. This behavior does not actually
        implement anything.
        """
        QObject.__init__(self)
        self.verify()
        

    def extent(self):
        """
        Defines a default extent simply returning a rectangle
        around the 0;0 coordinate.
        """
        return QgsRectangle()

    def filter(self, feature):
        """
        Defines a default filter simply returning `True'
        """
        return True

    def cmp(self, feature1, feature2):
        """
        Defines a default comparison method, returning 0
        """
        return 0

    def valid(self): 
        """
        A `Behavior' instance is always valid
        """
        return True
        
    def verify(self):
        """
        Checks if a behavior is properly created
        """
        if not self.valid():
            raise InvalidBehaviorException(self)

class ExtentBehavior(Behavior):
    """
    This behavior defines behavior to find objects given a specific
    extent. The default extent is a rectangle around the 0;0 coordinate.
    """
    
    def __init__(self, extent=QgsRectangle()):
        """
        Initializes the `ExtentBehavior' based on any of the following
        values as extent:
        
        - An instance of QgsRectangle
        - A list or tuple with four numeric values, converted to floating 
          point value, representing x-minimum, y-minimum, x-maximum, y-maximum
        """
        self.__extent = self.__make_extent(extent)
        Behavior.__init__(self)

    def __make_extent(self, extent):
        """
        See `__init__()' for more explanation. If the extent is a list
        or tuple, the implementation is flexible if the list contains
        more than four values.
        """
        if ((type(extent) is list or type(extent) is tuple) and
            len(extent) >= 4 and
            reduce(lambda s, v: isinstance(v, Number) and s, extent, True)):
            return QgsRectangle(*map(float, extent))
        else:
            return extent
              

    def valid(self):
        """
        The `ExtentBehavior' is valid if the extent is an instance of
        `QgsRectangle' or any of it's subclasses.
        """
        return isinstance(self.__extent, QgsRectangle)


    def verify(self):
        """
        Complements the `Behavior' to throw the `InvalidExtentException'
        if the `ExtentBehavior' could not coerce the extent parameter.
        """
        if not self.valid():
            raise InvalidExtentException(self.__extent)

    def extent(self):
        """
        Returns the extent as set by the given extent
        """
        return self.__extent
    
class RadialBehavior(ExtentBehavior):
    """
    This behavior builds on top of the extent behavior. There are
    two available keywords. The radius defaults to 50 and the 
    coordinate (coord) defaults to a QgsPoint of 0 by 0. It is valid 
    to specify a tuple with two values or a list with two values instead.
    """

    def __init__(self, radius=50, coord=QgsPoint(0,0)):
        """
        Given a radius and coordinate, create a behavior which finds
        features within the given radius from a specific point.
        """
        self.__coord = self.__convert_coord(coord)
        self.__radius = radius
        if not self.__coord is None and isinstance(radius, Number):
            extent = QgsRectangle(self.__coord.x() - radius,
                                  self.__coord.y() - radius,
                                  self.__coord.x() + radius,
                                  self.__coord.y() + radius)
        else:
            extent = None
        ExtentBehavior.__init__(self, extent=extent)

    def valid(self):
        """
        Returns True if the base class (`ExtentBehavior') is valid,
        the resulting coordinate is a QgsPoint instance and the radius
        is a number convertible by float.
        """
        base_is_valid = ExtentBehavior.valid(self)
        return (base_is_valid and isinstance(self.__radius, Number) and
                isinstance(self.__coord, QgsPoint))

    def verify(self):
        """
        Verifies if the coordinate and radius are properly defined, if
        not, an InvalidBehaviorException is thrown.
        """
        if not self.valid(): raise InvalidBehaviorException(self)
            

    def __convert_coord(self, coord):
        """
        Given a coord, sees if the coord is expressed as list,
        tuple or QgsPoint. If the latter is the case, return the
        coord as is, if it is either the first or second, convert
        it to a QgsPoint.
        """
        if isinstance(coord, QgsPoint): 
            return coord
        elif (type(coord) is list or 
              type(coord) is tuple) and (len(coord) >= 2 and 
                                         isinstance(coord[0], Number) and
                                         isinstance(coord[1], Number)):
            return QgsPoint(float(coord[0]), float(coord[1]))
        else:
            return None
            

    def __sqrdist_from_feature_radius(self, feature):
        """
        Given a feature get the position in relation to the given center
        coordinate
        """
        geometry = feature.geometry()
        return geometry.closestVertex(self.__coord)[-1]
        

    def filter(self, feature):
        """
        Filters a feature out of the result if the feature's distance
        > radius. In other words, if the radius is bigger than or equal
        to the position in relation to the center coordinate, the feature
        should be kept.
        """
        
        self.__sqrdist_from_feature_radius(feature)
        return (self.__radius ** 2) >= self.__sqrdist_from_feature_radius(feature)

    def cmp(self, feature_1, feature_2):
        """
        Given two features, returns the comparison based on their position
        towards the center coordinate.
        """
        sqrdists = map(self.__sqrdist_from_feature_radius, [feature_1, feature_2])
        return cmp(*sqrdists)
        

class SpatialFinder(QObject):
    """
    This class implements behavior patterns to help finding spatially
    related elements given a certain behavior function. This function
    will operate given a number of parameters on a given set of
    layers.
    """
    
    def __init__(self, layers, 
                 trim=15, behavior_class=ExtentBehavior, **kwargs):
        """
        Initializes a spatial finder given a certain behavior. The
        behavior deals with the manner of how the spatial finder should
        acquire it's components.
        
        The trim argument specifies the maximum of objects to be gathered.
        It defaults to 15. If set to None, it will return all found features.
        
        The behavior pattern should implement two methods: it should
        calculate an extent with the method `extent()'. This is followed
        with a `filter()' method, which can filter features within the
        given extent. The filter is called per feature. It should return a
        boolean indicating to include or exclude the elements respectively
        with `True' and `False' . The last method, `cmp()', receives
        two features and is used to order the given features if necessary.
        The value should return a typical `cmp()' result.
        """

        QObject.__init__(self)
        if behavior_class is None: 
            self.__behavior = None
        else: 
            self.__behavior = behavior_class(**kwargs)
        self.__layers = layers
        self.__trim = trim
        self.verify()

    def valid(self):
        """
        Returns True if the behavior is a valid `Behavior' or subclass thereof,
        the layers are valid layers (`QgsMapLayer') and the trim factor is
        a positive integer or None.
        """
        trim_ok = (self.__trim == None or 
                   (type(self.__trim) == int and self.__trim >= 0))

        behavior_ok =  isinstance(self.__behavior, Behavior)

        instances_ok = reduce(lambda s, l: isinstance(l, QgsMapLayer) and s,
                              self.__layers,
                              True)

        return trim_ok and behavior_ok and instances_ok

    def verify(self):
        """
        Raises an exception of the spatial finder is not `valid()'
        """
        if not self.valid():
            raise InvalidSpatialFinderException(self.__layers,
                                                self.__behavior,
                                                self.__trim)

    def __extract_features(self, layer, data_provider):
        """
        Extracts features by looping over the data provider and returning
        each found feature
        """
        feature = QgsFeature()
        pairs = []
        while data_provider.nextFeature(feature):
            safeFeature = QgsFeature(feature)
            pairs.append(SpatialPair(layer, safeFeature))
        return pairs

    def __get_pairs_from_layer(self, layer):
        """
        Given a data provider, we need to select the features we
        want to have from the data provider and start extracting
        all the features. The data provider then needs to be put to
        it's original position.
        """
        data_provider = layer.dataProvider()
        attribute_indexes = data_provider.attributeIndexes()
        data_provider.select(attribute_indexes, self.__behavior.extent())
        pairs = self.__extract_features(layer, data_provider)
        data_provider.rewind()
        return pairs

    def __filter_pair(self, pair):
        """
        given a pair, calls the filter of the behavior to drop
        the pair or keep the pair
        """
        return pair if self.__behavior.filter(pair.feature()) else None

    def __filter(self, pairs):
        """
        Given pairs, filter the pairs and remove all entries with
        None
        """
        filtered = map(self.__filter_pair, pairs)
        return [v for v in filtered if v != None]


    def __compare(self, pair1, pair2):
        """
        Calls the `cmp()' method on the behavior which should return
        a result usable by the sort method on a list
        """
        feature1 = pair1.feature()
        feature2 = pair2.feature()
        return self.__behavior.cmp(feature1, feature2)

    def find(self):
        """
        Uses the behavior to acquire the wanted features from the
        given layers. 
        """
        seq = map(self.__get_pairs_from_layer, self.__layers)
        pairs = [pair for list in seq for pair in list]
        filtered = self.__filter(pairs)
        filtered.sort(self.__compare)
        if not self.__trim is None: filtered = filtered[: self.__trim]
        return [pair.feature() for pair in filtered]

        
class InvalidBehaviorException(Exception):
    """
    Base class for improperly defined behaviors
    """
    def __init__(self, a_behavior):
        Exception.__init__(self)
        self.message = "Could not instantiate behavior {0}".format(a_behavior)

class InvalidExtentException(Exception):
    """
    An `InvalidExtentException' is raised whenever the `ExtentBehavior' receives
    an extent which is not a subclass of QgsRectangle.
    """
    def __init__(self, extent):
        Exception.__init__(self)
        self.message = "Could not use: {0} as extent".format(extent)

class InvalidSpatialFinderException(Exception):
    """
    An `InvalidSpatialFinderException' is raised whenever a `SpatialFinder'
    is improperly constructed by not having the proper values.
    """
    
    def __init__(self, layers, behavior, trim):
        Exception.__init__(self)
        self.message = (
            "Could not create a spatial finder with layers: {0}, "
            "behavior {1} and trim value {2}").format(layers, behavior, trim)
        
