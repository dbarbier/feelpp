# -*- mode: cmake; coding: utf-8 -*-
#
#  Author(s): Christophe Prud'homme <christophe.prudhomme@ujf-grenoble.fr>
#       Date: 2009-11-29
#
#  Copyright (C) 2009-2010 Université Joseph Fourier (Grenoble I)
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
IF ( FEELPP_ENABLE_INSTANTIATION_MODE )
  SET( FEELDISCR_INST_SRCS mesh1.cpp mesh2.cpp mesh3.cpp ) #partitioner.cpp ) #ale.cpp meshhighorder.cpp )
endif()



SET( SRC
  mesh.cpp stencil.cpp oseendata.cpp ${FEELDISCR_INST_SRCS}
  )

file(GLOB HDR RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.hpp)
feel_append_src(feeldiscr "${SRC};${HDR}")
