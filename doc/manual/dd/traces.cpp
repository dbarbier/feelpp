/* -*- mode: c++; coding: utf-8; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4; show-trailing-whitespace: t  -*-

  This file is part of the Feel library

  Author(s): Christophe Prud'homme <prudhomme@unistra.fr>
       Date: 2013-10-11

  Copyright (C) 2013 Université de Strasbourg

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
   \file traces.cpp
   \author Christophe Prud'homme <prudhomme@unistra.fr>
   \date 2013-10-11
 */
#include <feel/feel.hpp>

namespace Feel
{
template<int Dim>
class Traces : public Simget
{
    typedef Simget super;
public:
    /**
     * Constructor
     */
    Traces()
        :
        super()
    {
    }

    void run();
}; // Traces

template<int Dim>
void
Traces<Dim>::run()
{
    auto mesh = loadMesh(_mesh=new Mesh<Simplex<Dim>>);
    // auto Vh = Pch<1>( mesh );
    // auto u = Vh->element();
    // auto e = exporter( _mesh=mesh, _name = (boost::format( "%1%-%2%" ) % this->about().appName() % Dim ).str() );
    auto localMesh = createSubmesh( mesh, elements(mesh), Environment::worldComm().subWorldCommSeq() );
    saveGMSHMesh( _filename=(boost::format( "local-%1%-%2%.msh" ) % Dim % Environment::worldComm().globalRank()).str(),
                  _mesh=localMesh );

    const unsigned short nbNeighbors = mesh->faceNeighborSubdomains().size();
    unsigned int* recv = new unsigned int[2 * nbNeighbors];
    unsigned int* send = recv + nbNeighbors;
    MPI_Request* rq = new MPI_Request[2 * nbNeighbors];
    int i = 0;
    for( const size_type neighbor_subdomain : mesh->faceNeighborSubdomains() )
    {
        LOG(INFO) << "Extracting trace mesh from neighbor : " << neighbor_subdomain;
        auto trace = createSubmesh( mesh, interprocessfaces(mesh, neighbor_subdomain ),
                                    Environment::worldComm().subWorldCommSeq() );

        LOG(INFO) << "number of elements in trace mesh " << nelements(elements(trace)) <<      " (" << Environment::worldComm().globalRank() << " vs. " << neighbor_subdomain << ")";
        LOG(INFO) << "number of all elements in trace mesh " << nelements(allelements(trace)) <<      " (" << Environment::worldComm().globalRank() << " vs. " << neighbor_subdomain << ")";
        LOG(INFO) << "number of faces in trace mesh    " << nelements(boundaryfaces(trace)) << " (" << Environment::worldComm().globalRank() << " vs. " << neighbor_subdomain << ")";

        auto Xh = FunctionSpace<typename Mesh<Simplex<Dim>>::trace_mesh_type, bases<Lagrange<2,Scalar>>>::New( _mesh=trace, _worldscomm=Environment::worldsCommSeq(1) );
        auto l = Xh->element();
        CHECK( Xh->nDof() == Xh->nLocalDof() && l.size() == l.localSize() ) << "problem : " << Xh->nDof() << " != " << Xh->nLocalDof() << " || " <<  l.size() << " != " << l.localSize();
        l = vf::project( Xh, elements(trace), cst( double(1.0) ) );
        // auto op = opInterpolation( _domainSpace = Xh,
        //                            _imageSpace = Vh);
        // auto curr = op->operator()(l);
        // u += curr;
        l.printMatlab( (boost::format( "l%1%-%2%" ) % Dim % Environment::worldComm().globalRank()).str() );
        if ( nelements(elements(trace)) > 0 )
        {
            saveGMSHMesh( _filename=(boost::format( "trace-%1%-%2%-%3%.msh" ) % Dim % Environment::worldComm().globalRank() % neighbor_subdomain).str(),
                          _mesh=trace );
            auto m = mean(_range=elements(trace), _expr=idv(l),_worldcomm=Environment::worldComm().subWorldCommSeq() )(0,0);
            CHECK( math::abs( m -  double(1.) ) < 1e-14 ) << "problem : " << m << " != " << neighbor_subdomain;
        }
        else
            LOG(WARNING) << neighbor_subdomain << " is not a true neighbor of " << Environment::worldComm().globalRank();
        send[i] = nelements(elements(trace));
        MPI_Isend(send + i, 1, MPI_UNSIGNED, neighbor_subdomain, 0, Environment::worldComm(), rq + i);
        MPI_Irecv(recv + i, 1, MPI_UNSIGNED, neighbor_subdomain, 0, Environment::worldComm(), rq + nbNeighbors + i);
        ++i;
    }
    MPI_Waitall(nbNeighbors * 2, rq, MPI_STATUSES_IGNORE);
    for (i = 0; i < nbNeighbors; ++i)
        CHECK( send[i] == recv[i] ) << "problem : " << send[i] << " != " << recv[i];
    delete [] rq;
    delete [] recv;
    // e->add( "u", u );
    // e->save();
} // Traces::run

} // Feel

int main(int argc, char** argv) {
    using namespace Feel;
    /**
     * Initialize Feel++ Environment
     */
    Environment env( _argc=argc, _argv=argv,
                     _desc=feel_options(),
                     _about=about(_name="doc_traces",
                                  _author="Feel++ Consortium",
                                  _email="feelpp-devel@feelpp.org"));
    Application app;
    app.add(new Traces<2>());
    app.add(new Traces<3>());
    app.run();
}