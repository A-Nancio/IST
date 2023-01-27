//jpms:bc
/*----------------------------------------------------------------------------*\
 * File:        tchkcpf.cc
 *
 * Description: Checker for solutions of CPF instances.
 *
 * Author:      jpms
 *
 *                                     Copyright (c) 2015, Joao Marques-Silva
\*----------------------------------------------------------------------------*/
//jpms:ec

#include <iostream>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
#include <signal.h>
#include <unistd.h>

#include "common.hh"
#include "toolcfg.hh"
#include "cpf_config.hh"
#include "cpf_graph.hh"
#include "cpf_scenario.hh"
#include "cpf_solution.hh"
#include "cpf_checker.hh"

using namespace std;

void print_header(CPFConfig& config);
void parse_cmdline_options(CPFConfig& cfg, int argc, char** argv);


/*----------------------------------------------------------------------------*\
 * Purpose: The chkcpf tool
\*----------------------------------------------------------------------------*/

int main(int argc, char** argv)
{
    // 0. Parse command line arguments, parse input files, create
    //    internal data structures
    CPFConfig& config = CPFConfig::instance();
    parse_cmdline_options(config, argc, argv);

    if (config.get_verbosity() >= 0) { print_header(config); }

    CPFGraph graph(config);
    CPFScenario scen(config);
    CPFSolution sol(config);

    // 1. Invoke CPF checker
    CPFChecker checker(config, graph, scen, sol);
    bool stok = checker.run();
    if (not stok) {
	exit(10);
    } else {
	exit(0);
    }
    // 2. Print sequence of moves
    // ...

    return 0;
}


//jpms:bc
/*----------------------------------------------------------------------------*\
 * Utility functions
\*----------------------------------------------------------------------------*/
//jpms:ec

static const string emptystr = "";

static const string TOOL_HELP_HEADER =                                    \
  toolname + ": CPF solution checker\n" +                                 \
  "\n" + "usage: " + toolname +					          \
  " [ <option> ... ] \n" +					          \
  "where <option> is one of the following:\n";

static const string TOOL_HELP_DEF_SWITCHES =				  \
  emptystr +								  \
  "  -h         prints this help and exits\n" +				  \
  "  -v   NNN   verbosity level [default: -v 1]\n" +			  \
  "" ;

static const string TOOL_HELP_STD_SWITCHES =				  \
  emptystr +								  \
  "  -graph FFF load graph from file [required]\n" +                      \
  "  -scen  FFF load scenario [required]\n" +                             \
  "  -sol   FFF load solution [required]\n" +                             \
  "" ;

static const string TOOL_HELP_EXT_SWITCHES =                              \
    emptystr +                                                            \
    "" ;

static const string TOOL_HELP_EXP_SWITCHES = "";

static const string TOOL_HELP_TAIL =                                    \
    emptystr +			                                        \
    "additional info: see distribution README \n";


void prt_help()
{
  cout << TOOL_HELP_HEADER;
  cout << TOOL_HELP_DEF_SWITCHES;
  cout << TOOL_HELP_STD_SWITCHES;
  cout << TOOL_HELP_TAIL;
}


void parse_cmdline_options(CPFConfig& cfg, int argc, char** argv)
{
  DBG(cout << "ARGC: " << argc << endl; cout.flush(););
  if (argc == 1) { prt_help(); exit(1); }
  DBG(for(int i=1;i<argc-1;++i) { cfg.append_cmdstr((const char*) argv[i]); });
  for(int i=1; i<argc;) {
    NDBG(cout << "Current argv: " << argv[i] << endl;);
    if (!strcmp(argv[i], "-h")) { prt_help(); exit(1); }
    else if(!strcmp(argv[i], "-v")) { ++i; cfg.set_verbosity(atoi(argv[i])); }
    else if(!strcmp(argv[i], "-graph")) { ++i; cfg.set_graph_name(argv[i]); }
    else if(!strcmp(argv[i], "-scen"))  { ++i; cfg.set_scen_name(argv[i]); }
    else if(!strcmp(argv[i], "-sol"))   { ++i; cfg.set_sol_name(argv[i]); }
    else {
	string errmsg = string("Unknown string ") +
	    string(argv[i]) + string(". Check tool help.");
	cout << errmsg << endl;
	exit(20);
    }
    ++i;
  }
  if (cfg.get_graph_name() == "" || cfg.get_scen_name() == "" ||
      cfg.get_sol_name() == "") {
      cout<<"Must specify a graph, a scenario and a solution to check. ";
      cout<<"Check tool help." << endl;
      exit(20);
  }
}


//jpms:bc
/*----------------------------------------------------------------------------*\
 * Purpose: Print runtime header when executing mcsls.
\*----------------------------------------------------------------------------*/
//jpms:ec

void print_header(CPFConfig& config)
{
  cout<<"*** "<<toolname<<": a CPF checker ***" << endl;
  cout<<"*** author: "<<authorname<<" ***"<<endl;
  cout<<endl;
}

/*----------------------------------------------------------------------------*/
