/* -*- mode: c++; coding: utf-8; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4; show-trailing-whitespace: t -*-

  This file is part of the Feel library

  Author(s): Christophe Prud'homme <christophe.prudhomme@feelpp.org>
       Date: 2013-12-24

  Copyright (C) 2013 Feel++ Consortium

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
/**
   \file pchv.hpp
   \author Christophe Prud'homme <christophe.prudhomme@feelpp.org>
   \date 2013-12-24
 */
#if !defined(FEELPP_PCHV_HPP)
#define FEELPP_PCHV_HPP 1

#include <feel/feeldiscr/functionspace.hpp>

namespace Feel {

/**
   Given a \p mesh, build a function space of vectorial continuous function
   which are piecewise polynomial of degree (total or in each variable) less
   than k using Lagrange basis functions
 */
template<int Order,typename MeshType>
inline
boost::shared_ptr<FunctionSpace<MeshType,bases<Lagrange<Order,Vectorial>>>>
Pchv( boost::shared_ptr<MeshType> mesh, bool buildExtendedDofTable=false  )
{
    return FunctionSpace<MeshType,bases<Lagrange<Order,Vectorial>>>::New( _mesh=mesh,
                                                                          _worldscomm=std::vector<WorldComm>( 1,mesh->worldComm() ),
                                                                          _extended_doftable=std::vector<bool>( 1,buildExtendedDofTable ) );
}

}

#endif /* FEELPP_PCHV_HPP */