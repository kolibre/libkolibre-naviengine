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

check_PROGRAMS = navigationtest selectbyuritest selectwithgetset openclosetest toptest

TESTS = navigationtest selectbyuritest selectwithgetset openclosetest toptest

navigationtest_SOURCES = navigationtest.cpp
selectbyuritest_SOURCES = selectbyuritest.cpp
selectwithgetset_SOURCES = selectwithgetset.cpp
openclosetest_SOURCES = openclosetest.cpp
toptest_SOURCES = toptest.cpp

LDADD = -lkolibre-naviengine
AM_LDFLAGS = -L$(top_builddir)/src
AM_CPPFLAGS = -I$(top_srcdir)/src/
