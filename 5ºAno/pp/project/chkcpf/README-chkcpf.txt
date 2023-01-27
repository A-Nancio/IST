### README-chkcpf.txt --- 

## Author:   jpms
## Keywords: 
## Copyright (c) 2015, Joao Marques-Silva

chpcpf is a utility to validate solutions of CPF solving tools. chpcpf
is implemented in C++.

The expected usage is:
./chkcpf -v 1 -graph <graph> -scen <scenario> -sol <solution>

Given the expected usage, chkcpf validates the snapshots reported in
solution against the input graph and scenario.

To build the executable, execute the following commands:
% make deps
% make

In case you want to rebuild, start by executing the command:
% make clean

### README ends here
