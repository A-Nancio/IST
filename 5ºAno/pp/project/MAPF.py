from minizinc import Instance, Model, Solver, Status

from asyncio import start_server
import sys
import os

from minizinc import Instance, Model, Solver

mapf_model = Model("./MAPF_problem.mzn")
solver = Solver.lookup("gecode")

instance = Instance(solver, mapf_model)

graph_file = open(sys.argv[1])
scenario_file = open(sys.argv[2])


# -------------- parse Graph file
line = graph_file.readline()
while (line[0:1] == "#"): # check comment lines for graph input file
  line = graph_file.readline()

instance["num_vertices"] = int(line)
num_edges = int(graph_file.readline())

instance["adjancecy_list"] = [[0]*instance["num_vertices"]]*instance["num_vertices"]

for i in range(num_edges):
  input = graph_file.readline().split()
  if (len(input) != 2): raise ValueError("Syntax Error")
  instance["adjancecy_list"][int(input[0])-1][int(input[1])-1] = 1   # input = [vertex1, vertex2]
  instance["adjancecy_list"][int(input[1])-1][int(input[0])-1] = 1

# ------------- parse Scenario file
line = scenario_file.readline()
while (line[0:1] == "#"): # check comment lines for graph input file
  line = scenario_file.readline()

instance["num_agents"] = int(line)
instance["start_states"] = [0]*instance["num_agents"]
instance["goal_states"] = [0]*instance["num_agents"]

if (scenario_file.readline() != "START:\n"): raise ValueError("Syntax Error")
for agent in range(instance["num_agents"]):
  input = scenario_file.readline().split()
  if (len(input) != 2): raise ValueError("Syntax Error")
  instance["start_states"][int(input[0])-1] = int(input[1])

if (scenario_file.readline() != "GOAL:\n"): raise ValueError("Syntax Error")
for agent in range(instance["num_agents"]):
  input = scenario_file.readline().split()
  if (len(input) != 2): raise ValueError("Syntax Error")
  instance["goal_states"][int(input[0])-1] = int(input[1])

for makespan_size in range(instance["num_vertices"]**2):
  print("for makespan_size = " + str(makespan_size))
  instance["makespan_size"] = makespan_size
  result = instance.solve()
  if result.status == Status.SATISFIED:
    print(result.solution)





#mapf_model = Model("./MAPF_problem.mzn")
#solver = Solver.lookup("gecode")
#
#instance = Instance(solver, mapf_model)
#
#
## set inputs
#instance["adjancecy_list"] = [[1,1,0,0,0],
#                             [1,1,1,0,1],
#                             [0,1,1,1,0],
#                             [0,0,1,1,0],
#                             [0,1,0,0,1]]
#instance["start_states"] = [1,2,5]
#instance["goal_states"] = [5,2,1]
#instance["makespan_size"] = 9
#instance["num_vertices"] = 5
#instance["num_agents"] = 3
#
##run model
#result = instance.solve()
#
#print(result.solution)
