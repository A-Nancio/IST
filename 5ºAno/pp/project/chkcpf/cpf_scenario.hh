//jpms:bc
/*----------------------------------------------------------------------------*\
 * File:        cpf_scenario.hh
 *
 * Description: 
 *
 * Author:      jpms
 * 
 *                                     Copyright (c) 2015, Joao Marques-Silva
\*----------------------------------------------------------------------------*/
//jpms:ec

#ifndef _CPF_SCENARIO_H
#define _CPF_SCENARIO_H 1

#include "common.hh"
#include "cpf_config.hh"


//jpms:bc
/*----------------------------------------------------------------------------*\
 * Representation of the start and goal positions of the CPF instance
\*----------------------------------------------------------------------------*/
//jpms:ec

class CPFScenario {

public:

    CPFScenario(CPFConfig& ncfg) :
	cfg(ncfg), nagent(0), start(), goal() {
	load_scenario();
    }

    ULINT num_agents() { return nagent; }

    IntVector& start_pos() { return start; }

    IntVector& goal_pos() { return goal; }

    bool start_goal_differ();

protected:

    void load_scenario();

protected:

    CPFConfig& cfg;

    ULINT nagent;

    IntVector start;

    IntVector goal;

};

#endif /* _CPF_SCENARIO_H */

/*----------------------------------------------------------------------------*/
