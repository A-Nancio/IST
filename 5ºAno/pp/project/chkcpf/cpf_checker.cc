//jpms:bc
/*----------------------------------------------------------------------------*\
 * File:        cpf_checker.cc
 *
 * Description: 
 *
 * Author:      jpms
 *
 *                                     Copyright (c) 2015, Joao Marques-Silva
\*----------------------------------------------------------------------------*/
//jpms:ec

#include "common.hh"
#include "cpf_checker.hh"

bool CPFChecker::run()
{
    // 1. Validate start position
    if (cfg.get_verbosity() >= 1) { cout << "Checking time step 0" << endl; }
    tstep = 0;
    set_start_state();
    if (not check_start_state()) {
	cout << "Check failed at time step 0" << endl;
	return false;
    }
    update_state();
    reset_nonempty();
    update_nonempty();

    // 2. validate moves
    for(tstep=1; tstep<sol.makespan(); ++tstep) {
	if (cfg.get_verbosity() >= 1) {
	    cout << "Checking time step " << tstep << endl; }
	if (not check_move_state()) {
	    cout << "Checking failed at time step: " << tstep << endl;
	    return false;
	}
	update_state();
	reset_nonempty();
	update_nonempty();
    }
    if (not check_goal_state()) {
	cout << "Checking failed at final time step" << endl;
	return false;
    }
    cout << "Solution checked. Congratulations!" << endl;
    return true;
}

void CPFChecker::set_start_state()
{
    assert(tstep == 0);
    IntVector& start_pos = scen.start_pos();
    for(unsigned i=0; i<start_pos.size(); ++i) {
	cstate[i] = start_pos[i]+1;
    }
}

bool CPFChecker::check_start_state()
{
    IntVector& snapshot = sol.snapshot(tstep);
    for(unsigned i=0; i<snapshot.size(); ++i) {
	if (cstate[i] != snapshot[i]) {
	    cout << "Diff for agent " << i+1 << ": ";
	    cout << "state: " << cstate[i];
	    cout << " vs snapshot: " << snapshot[i] << endl;
	    return false;
	}
    }
    return true;
}

void CPFChecker::update_state()
{
    IntVector& snapshot = sol.snapshot(tstep);
    for(unsigned i=0; i<snapshot.size(); ++i) {
	cstate[i] = snapshot[i];
    }
}

bool CPFChecker::check_move_state()
{
    IntVector& snapshot = sol.snapshot(tstep);
    for(unsigned i=0; i<cstate.size(); ++i) {
	// If no move, then continue
	if (cstate[i] == snapshot[i]) { continue; }

	// Check if moving to vertex that is not empty
	if (nonempty[snapshot[i]-1]) { 
	    cout << "Agent " << i+1 << " moving to non-empty position: ";
	    cout << snapshot[i] << endl;
	    return false;
	}
	// Check if moving to non adjacent vertex
	IntVector& adj = graph.adjacents(cstate[i]-1);
	bool isadj = false;
	for(unsigned j=0; j<adj.size(); ++j) {
	    if (adj[j]+1 == snapshot[i]) { isadj = true; break; }
	}
	if (not isadj) {
	    cout << "Agent " << i+1 << " moving to non-adjacent vertex: ";
	    cout << snapshot[i] << endl;
	    return false;
	}
    }
    return true;
}

bool CPFChecker::check_goal_state()
{
    IntVector& goal_pos = scen.goal_pos();
    for(unsigned i=0; i<goal_pos.size(); ++i) {
	if (cstate[i] != goal_pos[i]+1) {
	    cout << "Goal state not matched for agent: " << i+1 << endl;
	    return false;
	}
    }
    return true;
}

void CPFChecker::update_nonempty()
{
    for(unsigned i=0; i<cstate.size(); ++i) {
	nonempty[cstate[i]-1] = 1;
    }
}

void CPFChecker::reset_nonempty()
{
    for(unsigned i=0; i<nonempty.size(); ++i) {
	nonempty[i] = 0;
    }
}

/*----------------------------------------------------------------------------*/
