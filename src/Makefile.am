## Copyright (C) 2012 Kolibre
#
# This file is part of kolibre-naviengine.
#
# Kolibre-naviengine is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 2.1 of the License, or
# (at your option) any later version.
#
# Kolibre-naviengine is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with kolibre-naviengine. If not, see <http://www.gnu.org/licenses/>.
#

AUTOMAKE_OPTIONS = foreign

SUBDIRS = Nodes

# Install the headers in a versioned directory - e.g. examplelib-1.0:
library_includedir=$(includedir)/libkolibre/naviengine-$(PACKAGE_VERSION)
library_include_HEADERS = NaviEngine.h

lib_LTLIBRARIES = libkolibre-naviengine.la

libkolibre_naviengine_la_SOURCES = NaviEngine.cpp Nodes/MenuNode.cpp Nodes/VirtualMenuNode.cpp
libkolibre_naviengine_la_LDFLAGS = -version-info $(VERSION_INFO)
libkolibre_naviengine_la_CPPFLAGS =

