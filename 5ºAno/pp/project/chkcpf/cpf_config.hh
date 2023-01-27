//jpms:bc
/*----------------------------------------------------------------------------*\
 * File:        cpf_config.hh
 *
 * Description: 
 *
 * Author:      jpms
 * 
 *                                     Copyright (c) 2015, Joao Marques-Silva
\*----------------------------------------------------------------------------*/
//jpms:ec

#ifndef _CPF_CONFIG_H
#define _CPF_CONFIG_H 1

#include <cassert>
#include <string>
#include <string.h>

using namespace std;

//jpms:bc
/*----------------------------------------------------------------------------*\
 * CPF configuration
\*----------------------------------------------------------------------------*/
//jpms:ec

class CPFConfig {

public:

    static CPFConfig& instance() { 
	if (_instance == NULL) { _instance = new CPFConfig(); }
	return *_instance;
    }

    static void release() { 
	if (_instance != NULL) { 
	    delete _instance;
	    _instance = NULL;
	}
    }

public:

    void set_verbosity(unsigned int nverb) { _verbosity = nverb; }

    int get_verbosity() { return _verbosity; }

    void set_graph_name(string gn) { _graph = gn; }

    string get_graph_name() { return _graph; }

    void set_scen_name(string sc) { _scen = sc; }

    string get_scen_name() { return _scen; }

    void set_sol_name(string sol) { _sol = sol; }

    string get_sol_name() { return _sol; }

protected:

    CPFConfig() :
	_verbosity(1), 
	_graph(""), _scen(""), _sol("") { }

    ~CPFConfig() { }

protected:

    unsigned int _verbosity;

    string _graph;

    string _scen;

    string _sol;

protected:

  static CPFConfig* _instance;  // Reference to object

};

#endif /* _CPF_CONFIG_H */

/*----------------------------------------------------------------------------*/
