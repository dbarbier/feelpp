/* -*- mode: c++; coding: utf-8; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4; show-trailing-whitespace: t -*- vim:fenc=utf-8:ft=tcl:et:sw=4:ts=4:sts=4 

  This file is part of the Feel library

  Author(s): Samuel Quinodoz
             Christophe Prud'homme <christophe.prudhomme@ujf-grenoble.fr>
       Date: 2009-02-25

  Copyright (C) 2007 Samuel Quinodoz
  Copyright (C) 2009 Université Joseph Fourier (Grenoble I)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3.0 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef __Convection_H
#define __Convection_H 1

/**
   \file convection.cpp
   \author Christophe Prud'homme <christophe.prudhomme@ujf-grenoble.fr>
   \author Samuel Quinodoz
   \date 2009-02-25
 */
#include <feel/options.hpp>
#include <feel/feelcore/applicationxml.hpp>

// (non)linear algebra backend
#include <feel/feelalg/backend.hpp>

// quadrature rules
#include <feel/feelpoly/im.hpp>

// function space
#include <feel/feeldiscr/functionspace.hpp>

// linear operators
#include <feel/feeldiscr/operatorlinear.hpp>

// exporter
#include <feel/feelfilters/exporter.hpp>

// use the Feel namespace
using namespace Feel;
using namespace Feel::vf;



template<typename A, uint16_type i>
class mytag : public A
{
public:
    static const uint16_type TAG = i;

};

/**
 * \class Convection
 * The class derives from the Application class
 * the template arguments are :
 * \tparam Order_s velocity polynomial order
 * \tparam Order_t temperature polynomial order
 * \tparam Order_p pressure polynomial order
 */
template< int Order_s, int Order_p, int Order_t >
class Convection : public ApplicationXML
{
    typedef ApplicationXML super;
public:

    typedef Convection<Order_s, Order_p, Order_t> self_type;

	// Definitions pour mesh
	typedef Simplex<2> entity_type;
	typedef Mesh<entity_type> mesh_type;
	typedef boost::shared_ptr<mesh_type> mesh_ptrtype;

    typedef Backend<double> backend_type;
    typedef boost::shared_ptr<backend_type> backend_ptrtype;

	typedef backend_type::sparse_matrix_ptrtype sparse_matrix_ptrtype;
    typedef backend_type::vector_ptrtype vector_ptrtype;

	// space and associated elements definitions
	typedef mytag<Lagrange<Order_s, Vectorial>,0> basis_u_type; // velocity space
	typedef mytag<Lagrange<Order_p, Scalar>,1> basis_p_type; // pressure space
	typedef mytag<Lagrange<Order_t, Scalar>,2> basis_t_type; // temperature space
    typedef mytag<Lagrange<0, Scalar>,3> basis_l_type; // multipliers for pressure space

	typedef fusion::vector< basis_u_type , basis_p_type , basis_t_type,basis_l_type> basis_type;


	typedef FunctionSpace<mesh_type, basis_type> space_type;
    typedef boost::shared_ptr<space_type> space_ptrtype;
    typedef typename space_type::element_type element_type;
    typedef typename element_type::template sub_element<0>::type element_0_type;
	typedef typename element_type::template sub_element<1>::type element_1_type;
	typedef typename element_type::template sub_element<2>::type element_2_type;
    typedef typename element_type::template sub_element<3>::type element_3_type;

    typedef OperatorLinear<space_type,space_type> oplin_type;
    typedef boost::shared_ptr<oplin_type> oplin_ptrtype;
    typedef FsFunctionalLinear<space_type> funlin_type;
    typedef boost::shared_ptr<funlin_type> funlin_ptrtype;

	// Definition pour les exportations
	typedef Exporter<mesh_type> export_type;

	// Constructeur
	Convection( int argc , char** argv , AboutData const& , po::options_description const& );

    // destructor
    ~Convection() {}

    // generate the mesh
    mesh_ptrtype createMesh();

	// Definition de la procedure pour faire tourner le code
	void run();

    void updateResidual( const vector_ptrtype& X, vector_ptrtype& R );
    void updateJacobian( const vector_ptrtype& X, sparse_matrix_ptrtype& J);


	// Definition de la procedure pour resoudre le systeme lineaire
    void solve( sparse_matrix_ptrtype& D, element_type& u, vector_ptrtype& F );

	// Definition de la procedure pour exporter les solutions
	void exportResults( boost::format, element_type& U, double t );

private:
    void initLinearOperator( sparse_matrix_ptrtype& L );
    void initLinearOperator2( sparse_matrix_ptrtype& L );
    void updateJacobian1( const vector_ptrtype& X, sparse_matrix_ptrtype& J);
    void updateJacobian2( const vector_ptrtype& X, sparse_matrix_ptrtype& J);
private:

    backend_ptrtype M_backend;

    space_ptrtype Xh;

    oplin_ptrtype M_oplin;
    funlin_ptrtype M_lf;

    //sparse_matrix_ptrtype L;
    sparse_matrix_ptrtype D;
    vector_ptrtype F;


	// Exporters
	boost::shared_ptr<export_type> exporter;

	// Timers
	std::map<std::string,std::pair<boost::timer,double> > timers;

    std::vector <double> Grashofs;
    double M_current_Grashofs;
    double M_current_Prandtl;
};
#endif /* __Convection_H */