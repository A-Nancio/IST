//jpms:bc
/*----------------------------------------------------------------------------*\
 * File:        cpf_solution.hh
 *
 * Description: 
 *
 * Author:      jpms
 * 
 *                                     Copyright (c) 2015, Joao Marques-Silva
\*----------------------------------------------------------------------------*/
//jpms:ec

#ifndef _CPF_SOLUTION_H
#define _CPF_SOLUTION_H 1

#include "common.hh"
#include "cpf_config.hh"


class CPFSolution {

public:

    CPFSolution(CPFConfig& ncfg) : cfg(ncfg), snapshots() {
	load_solution();
    }

    ~CPFSolution() {
	for(unsigned i=0; i < snapshots.size(); ++i) { delete snapshots[i]; }
	snapshots.clear();
    }

    IntVector& snapshot(unsigned i) { return *snapshots[i]; }

    unsigned makespan() { return snapshots.size(); }

protected:

    void load_solution();

protected:

    CPFConfig& cfg;

    IntVVector snapshots;

};

#endif /* _CPF_SOLUTION_H */

/*----------------------------------------------------------------------------*/
