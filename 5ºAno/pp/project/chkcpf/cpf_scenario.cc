//jpms:bc
/*----------------------------------------------------------------------------*\
 * File:        cpf_scenario.cc
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

#include "cpf_scenario.hh"

static const ULINT SLINESZ = 2014;


bool CPFScenario::start_goal_differ()
{
    for(ULINT i=0; i<start.size(); ++i) {
	if (start[i] != goal[i]) { return true; }
    }
    return false;
}

void CPFScenario::load_scenario()
{
    DBG(cout << "Called CPFGraph::load_scenario() ..." << endl; cout.flush(););
    ifstream sfile(cfg.get_scen_name().c_str());

    // Remove comment lines
    char comment[SLINESZ];
    while (sfile.peek() == '#') {
	sfile.getline(comment, SLINESZ, '\n');
	//cout << "Read line: " << comment << endl;
    }

    string wstr;
    // Read in the number of vertices
    sfile >> nagent;
    //cout << "Num agents: " << nagent << endl;

    start.resize(nagent, 0);
    goal.resize(nagent, 0);

    // Clear START:
    sfile >> wstr;
    // Read start position
    for(ULINT i=0; i<nagent; ++i) {
	ULINT v1, v2;
	sfile >> v1;
	sfile >> v2;
	start[v1-1] = v2-1;  // Shift agent/vertex IDs by 1
	//cout << "Read pos: " << v1 << " at " << v2 << endl; cout.flush();
    }

    // Clear GOAL:
    sfile >> wstr;
    // Read goal position
    for(ULINT i=0; i<nagent; ++i) {
	ULINT v1, v2;
	sfile >> v1;
	sfile >> v2;
	goal[v1-1] = v2-1;
	//cout << "Read pos: " << v1 << " at " << v2 << endl; cout.flush();
    }
    sfile.close();
    assert(start.size() == goal.size());
    DBG(cout << "Done with scenario ...\n"; cout.flush(););
}

/*----------------------------------------------------------------------------*/
