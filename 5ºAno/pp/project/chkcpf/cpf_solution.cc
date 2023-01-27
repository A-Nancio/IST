//jpms:bc
/*----------------------------------------------------------------------------*\
 * File:        cpf_solution.cc
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
#include <string>

using namespace std;

#include "cpf_solution.hh"

void CPFSolution::load_solution()
{
    DBG(cout << "Called CPFSolution::load_solution() ..." << endl;
	cout.flush(););
    ifstream sfile(cfg.get_sol_name().c_str());

    while(not sfile.eof()) {
	string line;
	getline(sfile, line);
	if (line.length() == 0) { break; }
	DBG(cout << line << endl;);

	IntVector* nv = new IntVector();
	IntVector& nvr = *nv;
	unsigned i=0;
	assert(line[i] == 'i');
	++i;
	assert(line[i] == '=');
	++i;
	while(line[i] != ' ' && line[i] != '\t') ++i;
	
	while (i<line.length()) {
	    while(line[i] == ' ' || line[i] == '\t') ++i;
	    DBG(cout << "CCHAR: " << line[i] << endl;);
	    
	    int num=0;
	    while(line[i] >= '0' && line[i] <= '9') {
		num = 10 * num + (line[i] - '0');
		++i;
	    }
	    DBG(cout << "NUM 1: "<<num<< endl;);

	    assert(line[i] == ':');
	    ++i;

	    num = 0;
	    while(line[i] >= '0' && line[i] <= '9') {
		num = 10 * num + (line[i] - '0');
		++i;
	    }
	    DBG(cout << "NUM 2: "<<num<< endl;);
	    nvr.push_back(num);
	    while(line[i] == ' ' || line[i] == '\t') ++i;
	}
	DBG(cout << "snapshot: ";
	    for (auto i = nvr.begin(); i != nvr.end(); ++i) {
		std::cout << *i << ' ';
	    }
	    cout << endl;);
	// Check if there are repeated positions...
	IntVector chkv = nvr;
	sort(chkv.begin(), chkv.end());
	for(unsigned i=1; i<chkv.size(); ++i) {
	    if (chkv[i-1] == chkv[i]) {
		cout << "Snapshot with repeated positions?? Terminating ...\n";
		exit(30);
	    }
	}
	snapshots.push_back(nv);
    }
    sfile.close();
    DBG(cout << "Done with solution ...\n"; cout.flush(););
}

/*----------------------------------------------------------------------------*/
