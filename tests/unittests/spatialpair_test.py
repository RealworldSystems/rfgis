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

import unittest

# The unit under test
from rfgis.model.spatialpair import *

# Requirements of the unit under test
from qgis.core import QgsDataProvider
from qgis.core import QgsFeature

class MockFooProvider(QgsDataProvider):
    """
    A non-abstract subclass of QgsDataProvider with no further implementation
    """
    pass

class SpatialPairTestCase(unittest.TestCase):
    """
    A SpatialPair is considered to be a binding between a feature
    and a descriptor in the form of a data provider. The data
    provider describes which columns and data attributes are available
    whereas the feature describes how it is presented on a map canvas
    """
    
    def setUp(self):
        """
        Creates a mock data provider of type Foo to use for testing.
        Also creates a valid `feature'.
        """
        self.foo_provider = MockFooProvider()
        self.feature = QgsFeature()
        self.feature.setGeometry(QgsGeometry.fromPoint(QgsPoint(0, 0)))
        self.feature.setValid(True)

    def test_fail_no_dataprovider_no_feature(self):
        """
        Creation of a spatial pair should fail when the given spatial pair
        does not get the proper objects. The test asserts a number of
        trivial faults to ensure the test works as expected.
        
        Assertion 1: Tests with None objects
        Assertion 2: Tests with integers
        Assertion 3: Tests with objects reversed
        """
        self.assertRaises(InvalidSpatialPairException,
                          SpatialPair,
                          None, None)

        self.assertRaises(InvalidSpatialPairException,
                          SpatialPair,
                          1, 1)

        self.assertRaises(InvalidSpatialPairException,
                          SpatialPair, 
                          self.feature, self.foo_provider)

    def test_fail_no_feature(self):
        """
        Creation of a spatial pair should fail when the given spatial pair
        does not get a feature
        """
        self.assertRaises(InvalidSpatialPairException,
                          SpatialPair,
                          self.foo_provider, None)

    def test_fail_no_data_provider(self):
        """
        Creation of a spatial pair should fail when the given spatial pair
        does not get a data provider
        """
        self.assertRaises(InvalidSpatialPairException,
                          SpatialPair,
                          None, self.feature)

    def test_fail_invalid_feature(self):
        """
        Creation of a spatial pair should fail when the given spatial pair
        does not get a valid feature
        """
        self.assertRaises(InvalidSpatialPairException,
                          SpatialPair,
                          self.foo_provider, QgsFeature())

    def test_get_dataprovider(self):
        """
        A valid spatial pair should return the given data provider properly
        by both methods `data_provider' and `dataProvider'. The latter is
        implemented for naming conventions of Qt.
        """
        pair = SpatialPair(self.foo_provider, self.feature)
        self.assertEquals(pair.data_provider(), self.foo_provider)
        self.assertEquals(pair.dataProvider(), self.foo_provider)

    def test_get_feature(self):
        """
        A valid spatial pair should return the given feature properly using
        the `feature' method.
        """
        pair = SpatialPair(self.foo_provider, self.feature)
        self.assertEquals(pair.feature(), self.feature)

    
        
