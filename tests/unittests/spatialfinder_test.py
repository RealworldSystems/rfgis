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

import unittest

# The unit under test
from rfgis.model.spatialfinder import *

# Requirements of the unit under test

from qgis.core import QgsFeature
from qgis.core import QgsVectorLayer
from qgis.core import QgsMapLayer
from qgis.core import QgsDataProvider
from qgis.core import QgsVectorDataProvider

import sys

class MockFooProvider(QgsVectorDataProvider):
    """
    This data provider is constructed from features of the foo layer.
    """
    def __init__(self, features):
        self.features = features
        self.current_feature_index = 0
        self.max_extent = None
    
    def select(self, indices, extent):
        self.max_extent = extent
        
    def rewind(self):
        self.current_feature_index = 0
        self.max_extent = None
    
    def nextFeature(self, result_feature):
        if self.current_feature_index >= len(self.features):
            return False
        else:
            a_feature = self.features[self.current_feature_index]
            self.current_feature_index += 1
            if a_feature.geometry().boundingBox().intersects(self.max_extent):
                result_feature.setGeometry(a_feature.geometry())
                result_feature.setValid(True)
                return True
            else:
                return self.nextFeature(result_feature)

    def attributeIndexes(self): return []

class MockFooLayer(QgsVectorLayer):
    """
    This is a layer representing a dataProvider with exactly three
    point features. The features have different positions.
    """
    
    def __aid_init_create_feature(self, geom):
        f = QgsFeature()
        f.setGeometry(geom)
        return f
    
    def __init__(self):
        QgsVectorLayer.__init__(self) # FIXME: don't know the parameters required
        geoms = map(QgsGeometry.fromPoint, [QgsPoint(0, 0),
                                            QgsPoint(100, 100),
                                            QgsPoint(200, 200)])
        self.features = map(self.__aid_init_create_feature, geoms)
        self.data_provider = MockFooProvider(self.features)

    def dataProvider(self): return self.data_provider


class BehaviorTestCase(unittest.TestCase):
    """
    A Behavior is the logic behind finding features within a given
    layer. The base behavior implements only some default methods
    if certain methods are not necessary to reimplement in a given
    subclass.
    """
    def setUp(self):
        self.behavior = Behavior()

    def test_extent(self):
        """
        The extent method should return a rectangle around
        coordinate 0;0 (so the xMaximum, xMinimum, yMaximum and yMinimum)
        """
        extent = self.behavior.extent()
        self.assertEquals(0, extent.xMaximum())
        self.assertEquals(0, extent.xMinimum())
        self.assertEquals(0, extent.yMaximum())
        self.assertEquals(0, extent.yMinimum())

    def test_filter(self):
        """
        The filter function should simply return true given any kind
        of feature.
        """
        filter = self.behavior.filter(QgsFeature())

    def test_cmp(self):
        """
        The filter function should simply return 0 given whatever features
        position, as no sorting is implemented by the base behavior.
        """
        feature1 = QgsFeature()
        feature2 = QgsFeature()
        feature1.setGeometry(QgsGeometry.fromPoint(QgsPoint(0, 0)))
        feature2.setGeometry(QgsGeometry.fromPoint(QgsPoint(100, 100)))
        
        self.assertEquals(0, self.behavior.cmp(feature1, feature2))
        self.assertEquals(0, self.behavior.cmp(feature2, feature1))
        self.assertEquals(0, self.behavior.cmp(feature1, feature1))
        self.assertEquals(0, self.behavior.cmp(feature2, feature2))

class ExtentBehaviorCreationTestCase(unittest.TestCase):
    """
    It should be possible to construct the `ExtentBehavior' using different
    objects for the extent, as to be flexible towards end-users.
    """
    
    def assert_behavior(self, behavior):
        """
        The behavior's `extent()' method should return an extent with
        the values -1000, -1000, 1000, 1000
        """
        extent = behavior.extent()
        self.assertEquals(-1000, extent.xMinimum())
        self.assertEquals(-1000, extent.yMinimum())
        self.assertEquals( 1000, extent.xMaximum())
        self.assertEquals( 1000, extent.yMaximum())

    def test_with_qgs_rectangle(self):
        """
        Given a QgsRectangle, it should be possible to construct an
        `ExtentBehavior' object.
        """
        b = ExtentBehavior(extent=QgsRectangle(-1000, -1000, 1000, 1000))
        self.assert_behavior(b)
    
    def test_with_list(self):
        """
        Given a list with four numeric values, it should be possible
        to construct an ExtentBehavior.
        """
        b = ExtentBehavior(extent=[-1000, -1000, 1000, 1000])
        self.assert_behavior(b)
        
    def test_with_tuple(self):
        """
        Given a tuple with four numeric values, it should be possible
        to construct an ExtentBehavior.
        """
        b = ExtentBehavior(extent=(-1000, -1000, 1000, 1000))
        self.assert_behavior(b)

    def test_wrong_object(self):
        """
        Given a wrong object, the `ExtentBehavior' should raise
        an exception on initialization.
        """
        self.assertRaises(InvalidExtentException,
                          ExtentBehavior,
                          extent="The Foobar")
        
    def test_wrong_none(self):
        """
        Given None, the `ExtentBehavior' should raise an exceoption on
        intialization.
        """
        self.assertRaises(InvalidExtentException,
                          ExtentBehavior,
                          extent=None)
    
    def test_list_too_small(self):
        """
        Given a list which is too small, the `ExtentBehavior' should
        raise an exception on initialization.
        """
        self.assertRaises(InvalidExtentException,
                          ExtentBehavior,
                          extent=[-1000, -1000, 1000])

    def test_tuple_too_small(self):
        """
        Given a tuple which is too small, the `ExtentBehavior' should
        raise an exception on initialization.
        """
        self.assertRaises(InvalidExtentException,
                          ExtentBehavior,
                          extent=(-1000, -1000, 1000))

    def test_list_wrong_objects(self):
        """
        Given a list of the proper size but the values thereof in error,
        the `ExtentBehavior' should raise an exception on initialization.
        """
        wrong_list = ["The foobar", 0, 0, 0]
        for i in range(0, len(wrong_list)):
            # Rotating the list for each iteration
            wrong_list = wrong_list[1:] + wrong_list[:1]
            self.assertRaises(InvalidExtentException,
                              ExtentBehavior,
                              extent=wrong_list)

    def test_tuple_wrong_objects(self):
        """
        Given a list of the proper size but the values thereof in error,
        the `ExtentBehavior' should raise an exception on initialization.
        """
        wrong_tuple = ["The foobar", 0, 0, 0]
        for i in range(0, len(wrong_tuple)):
            # Rotating the tuple for each iteration
            wrong_tuple = wrong_tuple[1:] + wrong_tuple[:1]
            self.assertRaises(InvalidExtentException,
                              ExtentBehavior,
                              extent=wrong_tuple)
        

class ExtentBehaviorTestCase(unittest.TestCase):
    """
    The `ExtentBehavior' is highly similar to the base `Behavior', is
    it has not specific filter nor does it have sensible sorting behavior.
    However, it implements an extent, so the results are that of the
    given extent.
    """
    def setUp(self):
        extent = QgsRectangle(-100, -100, 100, 100)
        self.behavior = ExtentBehavior(extent=extent)
    
    def test_extent(self):
        """
        The extent should match the extent given in the setUp
        """
        extent = self.behavior.extent()
        self.assertEquals( 100, extent.xMaximum())
        self.assertEquals(-100, extent.xMinimum())
        self.assertEquals( 100, extent.yMaximum())
        self.assertEquals(-100, extent.yMinimum())
        
    def test_filter(self):
        """
        The filter function should simply return true given any kind
        of feature.
        """
        filter = self.behavior.filter(QgsFeature())

class WrongRadialBehaviorTestCase(unittest.TestCase):
    """
    The `WrongRadialBehaviorTestCase' tests that creation of `RadialBehavior'
    instances fail if the extent parameter is erroneous.
    """
    def test_coord_none(self):
        """
        Tests creation of `RadialBehavior' doesn't work if coord is `None'
        """
        self.assertRaises(InvalidBehaviorException,
                          RadialBehavior,
                          coord=None)

    def test_coord_wrong(self):
        """
        Tests creation of `RadialBehavior' doesn't work if coord is a wrong
        kind of object
        """
        self.assertRaises(InvalidBehaviorException,
                          RadialBehavior,
                          coord="The Foobar")

    def test_radius_none(self):
        """
        Tests creation of `RadialBehavior' doesn't work if radius is `None'
        """
        self.assertRaises(InvalidBehaviorException,
                          RadialBehavior,
                          radius=None)

    def test_radius_wrong(self):
        """
        Tests creation of `RadialBehavior' doesn't work if radius is a wrong
        kind of object
        """
        self.assertRaises(InvalidBehaviorException,
                          RadialBehavior,
                          radius="The Foobar")

class RadialBehaviorTestCase(unittest.TestCase):
    """
    The `RadialBehavior' is based on the `ExtentBehavior' but implements
    the `filter()' method to filter out features which are outside the
    given radius and uses the `cmp()' method to sort the features where
    the closest feature becomes the first
    """
    def setUp(self):
        radius = 50
        coord = QgsPoint(100, 100)
        self.test_id = 1
        self.behavior = RadialBehavior(radius=radius, coord=coord)

    def test_instantiation_with_list_and_tuple(self):
        lst_behavior = RadialBehavior(radius=50, coord=[100, 100])
        tuple_behavior = RadialBehavior(radius=50, coord=(100, 100))
        self.assertEquals(lst_behavior.extent(), self.behavior.extent())
        self.assertEquals(tuple_behavior.extent(), self.behavior.extent())

    def test_extent(self):
        """
        The extent returned by the `RadialBehavior' instance should
        be a rectangle with the values: [50, 50, 150, 150]
        """
        ext = self.behavior.extent()
        lst = [ext.xMinimum(), ext.yMinimum(), ext.xMaximum(), ext.yMaximum()]
        self.assertEquals([50, 50, 150, 150], lst)

    def __make_test_feature(self, x, y):
        f = QgsFeature()
        wkt = QgsPoint(x, y).wellKnownText()
        geom = QgsGeometry.fromWkt(wkt)
        f.setGeometry(geom)
        f.setFeatureId(self.test_id)
        self.test_id += 1
        f.setValid(True)
        return f

    def test_filter(self):
        """
        Given features which have coordinates within the radius 50 of
        coordinate 100,100 should be accepted. This means that
        the features on the corners should not be accepted,
        but features on the side should. This boils down to the following
        points (x = invalid, . = valid, # = invalid region, -/| = extent)
        
        150 x-------.-------x
            |#####     #####|
            |##           ##|
            |               |
        100 .       .       .
            |               |
            |##           ##|
            |#####     #####|
         50 x-------.-------x
            50     100     150
        """
        self.assertFalse(self.behavior.filter(self.__make_test_feature( 50,  50)))
        self.assertFalse(self.behavior.filter(self.__make_test_feature(150, 150)))
        self.assertFalse(self.behavior.filter(self.__make_test_feature( 50, 150)))
        self.assertFalse(self.behavior.filter(self.__make_test_feature(150,  50)))
        self.assertTrue(self.behavior.filter(self.__make_test_feature( 50, 100)))
        self.assertTrue(self.behavior.filter(self.__make_test_feature(150, 100)))
        self.assertTrue(self.behavior.filter(self.__make_test_feature(100,  50)))
        self.assertTrue(self.behavior.filter(self.__make_test_feature(100, 150)))
        self.assertTrue(self.behavior.filter(self.__make_test_feature(100, 100)))
        
    def test_cmp(self):
        """
        Given two features, the feature closest to the center coordinate should
        be first.
        """
        lst = [self.__make_test_feature(50, 100),
               self.__make_test_feature(100, 100)]
        self.assertEquals(1, self.behavior.cmp(lst[0], lst[1]))
        self.assertEquals(-1, self.behavior.cmp(lst[1], lst[0]))
        self.assertEquals(0,self.behavior.cmp(lst[0], lst[0]))
        self.assertEquals(0,self.behavior.cmp(lst[1], lst[1]))

        lst2 = [self.__make_test_feature(50, 100),
               self.__make_test_feature(100, 100)]
        self.assertEquals(1, self.behavior.cmp(lst2[0], lst2[1]))
        self.assertEquals(-1, self.behavior.cmp(lst2[1], lst2[0]))
        self.assertEquals(0,self.behavior.cmp(lst2[0], lst2[0]))
        self.assertEquals(0,self.behavior.cmp(lst2[1], lst2[1]))

        lst3 = [self.__make_test_feature(50, 100),
               self.__make_test_feature(100, 100)]
        expected_lst = [self.__make_test_feature(100, 100),
                        self.__make_test_feature(50, 100)]
        lst3.sort(self.behavior.cmp)

        make_wkt = lambda f: f.geometry().exportToWkt()
        lst3_wkt = map(make_wkt, lst3)
        expected_lst_wkt = map(make_wkt, expected_lst)

        self.assertEquals(expected_lst_wkt, lst3_wkt)

class SpatialFinderTestCase(unittest.TestCase):
    """
    A `SpatialFinder' finds objects in given layers using a given behavior and
    a trim factor. The trim factor is used to eliminate objects which are
    passed the given amount in the expected result.
    """
    
    def setUp(self):
        self.layers = [MockFooLayer()]

    def assert_result(self, result):
        map(lambda r: self.assertTrue(isinstance(r, QgsFeature)), result)

    def test_with_base_behavior(self):
        """
        Given a `MockFooLayer' instance, the base `Behavior' should result
        in finding exactly three elements in any particular order.
        """
        sf = SpatialFinder(self.layers, behavior_class=Behavior)
        result = sf.find()
        self.assert_result(result)
        self.assertEquals(1, len(result))

    def test_with_extent_behavior(self):
        """
        Given a `MockFooLayer' instance, the `ExtentBehavior', which is the
        default behaviour, should find 1 feature.
        """
        sf = SpatialFinder(self.layers, extent=QgsRectangle(50, 50, 150, 150))
        result = sf.find()
        self.assert_result(result)
        self.assertEquals(1, len(result))

    def test_with_radial_behavior(self):
        sf = SpatialFinder(self.layers, behavior_class=RadialBehavior, 
                           radius=200, coord=QgsPoint(0, 0))
        result = sf.find()
        self.assert_result(result)
        self.assertEquals(2, len(result))
        self.assertEquals(QgsPoint(0, 0), result[0].geometry().asPoint())
        
    def test_with_extent_and_trim_2(self):
        sf = SpatialFinder(self.layers, trim=2, extent=[-1000, -1000, 1000, 1000])
        result = sf.find()
        self.assert_result(result)
        self.assertEquals(2, len(result))
        
    def test_with_extent_and_trim_none(self):
        # Each Foo Layer has 3 objects, 6 x 3 = 18
        many_layers = [MockFooLayer(),MockFooLayer(),MockFooLayer(),
                       MockFooLayer(),MockFooLayer(),MockFooLayer()]
        sf = SpatialFinder(many_layers, trim=None, extent=[-1000, -1000, 1000, 1000])
        result = sf.find()
        self.assert_result(result)
        self.assertEquals(18, len(result))

    def test_with_extent_and_trim_default(self):
        # Each Foo Layer has 3 objects, 6 x 3 = 18
        many_layers = [MockFooLayer(),MockFooLayer(),MockFooLayer(),
                       MockFooLayer(),MockFooLayer(),MockFooLayer()]
        sf = SpatialFinder(many_layers, extent=[-1000, -1000, 1000, 1000])
        result = sf.find()
        self.assert_result(result)
        self.assertEquals(15, len(result))
    
    def test_with_wrong_trim(self):
        for v in ["Abc", -1]:
            self.assertRaises(InvalidSpatialFinderException,
                              SpatialFinder,
                              self.layers, trim=v)
        
    def test_with_wrong_behavior(self):
        self.assertRaises(InvalidSpatialFinderException,
                          SpatialFinder,
                          self.layers, behavior_class=None)

    def test_with_wrong_layers(self):
        self.assertRaises(InvalidSpatialFinderException,
                          SpatialFinder,
                          ["foo", "bar"])
        
    
unittest.main()
