//jpms:bc
/*----------------------------------------------------------------------------*\
 * File:        cpf_graph.hh
 *
 * Description: 
 *
 * Author:      jpms
 * 
 *                                     Copyright (c) 2015, Joao Marques-Silva
\*----------------------------------------------------------------------------*/
//jpms:ec

#ifndef _CPF_GRAPH_H
#define _CPF_GRAPH_H 1

#include "common.hh"
#include "cpf_config.hh"
//#include "cpf_types.hh"


//jpms:bc
/*----------------------------------------------------------------------------*\
 * Representation CPF instance as a graph
\*----------------------------------------------------------------------------*/
//jpms:ec

class CPFGraph {

public:

    CPFGraph(CPFConfig& ncfg) :
	cfg(ncfg), nvrtx(0), nedge(0), edges() {
	load_graph();
    }

    ~CPFGraph() { }

    ULINT num_vertices() { return nvrtx; }

    ULINT num_edges() { return nedge; }

    IntVector& adjacents(ULINT idx) { return *edges[idx]; }

protected:

    void load_graph();

protected:

    CPFConfig& cfg;

    ULINT nvrtx;

    ULINT nedge;

    IntVVector edges;

};

#endif /* _CPF_GRAPH_H */

/*----------------------------------------------------------------------------*/
