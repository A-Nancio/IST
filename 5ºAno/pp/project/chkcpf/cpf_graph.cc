//jpms:bc
/*----------------------------------------------------------------------------*\
 * File:        cpf_graph.cc
 *
 * Description: 
 *
 * Author:      jpms
 *
 *                                     Copyright (c) 2015, Joao Marques-Silva
\*----------------------------------------------------------------------------*/
//jpms:ec

#include <algorithm>
#include <iostream>

using namespace std;

#include "cpf_graph.hh"

static const ULINT GLINESZ = 2014;


void CPFGraph::load_graph()
{
    DBG(cout << "Called CPFGraph::load_graph() ..." << endl; cout.flush(););
    ifstream gfile(cfg.get_graph_name().c_str());

    // Remove comment lines
    char comment[GLINESZ];
    while (gfile.peek() == '#') {
	gfile.getline(comment, GLINESZ, '\n');
	//cout << "Read line: " << comment << endl;
    }
    // Read in the number of vertices
    gfile >> nvrtx;
    edges.resize(nvrtx, 0);
    for(ULINT i=0; i<nvrtx; ++i) {
	edges[i] = new IntVector();
    }
    // Read in the number of edges
    gfile >> nedge;

    for(ULINT i=0; i<nedge; ++i) {
	ULINT v1, v2;
	gfile >> v1;
	gfile >> v2;
	if (v1 > v2) { swap(v1, v2); }
	edges[v1-1]->push_back(v2-1);   // Shift vertex IDs by 1
	edges[v2-1]->push_back(v1-1);   // Duplicate edges internally! See below
	//cout << "Read edge: " << v1 << " and " << v2 << endl; cout.flush();
    }
    DBG(for(ULINT i=0; i<nvrtx; ++i) {
	    cout << i+1 << " has " << edges[i]->size() << " edges" << endl;
	}
	cout.flush(););
    gfile.close();
    DBG(cout << "Done with graph..." << endl; cout.flush(););
}

//jpms:bc
/*----------------------------------------------------------------------------*\
 * NOTE: Duplication is necessary to represent all edges for each vertex.
\*----------------------------------------------------------------------------*/
//jpms:ec

/*----------------------------------------------------------------------------*/
