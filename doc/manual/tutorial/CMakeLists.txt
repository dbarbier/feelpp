###  TEMPLATE.txt.tpl; coding: utf-8 ---

#  Author(s): Christophe Prud'homme <christophe.prudhomme@ujf-grenoble.fr>
#       Date: 2012-02-03
#
#  Copyright (C) 2012 Universit� Joseph Fourier (Grenoble I)
#
# Distributed under the GPL(GNU Public License):
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
#

add_custom_target(
  loadmesh
  ALL
  COMMAND ${CMAKE_COMMAND} -E copy ${FEEL_SOURCE_DIR}/data/medit/Cylref.mesh
                                   ${FEEL_BUILD_DIR}/doc/manual/
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
add_docex(myapp)
add_docex(mymesh)
add_docex(loadmesh)
add_docex(myintegrals)
add_docex(geotool)
add_docex(myfunctionspace)
add_docex(laplacian)
add_docex(dar)
add_docex(stokes)
# stokes_good stokes_locking stokes_spurious
# bratu nonlinearpow mortar multi_levels threefields nirb)