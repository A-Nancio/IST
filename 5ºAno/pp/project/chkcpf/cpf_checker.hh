//jpms:bc
/*----------------------------------------------------------------------------*\
 * File:        cpf_checker.hh
 *
 * Description: 
 *
 * Author:      jpms
 * 
 *                                     Copyright (c) 2015, Joao Marques-Silva
\*----------------------------------------------------------------------------*/
//jpms:ec

#ifndef _CPF_CHECKER_H
#define _CPF_CHECKER_H 1

#include "cpf_config.hh"
#include "cpf_graph.hh"
#include "cpf_scenario.hh"
#include "cpf_solution.hh"


class CPFChecker {

public:
    CPFChecker(CPFConfig& ncfg,
	       CPFGraph& gr, CPFScenario& sc, CPFSolution& sl) :
	cfg(ncfg), graph(gr), scen(sc), sol(sl),
	cstate(), tstep(0), nonempty()
    {
	cstate.resize(scen.num_agents(), 0);
	nonempty.resize(graph.num_vertices(), 0);
    }

    bool run();

protected:

    void set_start_state();

    bool check_start_state();

    void update_state();

    bool check_move_state();

    bool check_goal_state();

    void update_nonempty();

    void reset_nonempty();

protected:

    CPFConfig& cfg;

    CPFGraph& graph;

    CPFScenario& scen;

    CPFSolution& sol;

    vector<int> cstate;  // Current state: where agents are positioned

    unsigned tstep;

    vector<int> nonempty;   // Occupied vertices (=1)

};

#endif /* _CPF_CHECKER_H */

/*----------------------------------------------------------------------------*/
