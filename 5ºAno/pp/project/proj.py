from asyncio import start_server
from multiprocessing.sharedctypes import Value
import sys
import os
from tracemalloc import start
import warnings
from minizinc import Instance, Model, Solver, Status

def bfs_solve(adjacency_list, start, goal) -> int:
    explored = []
    exploration_levels = [1] * num_vertices # know at which level the nodes were discovered
    queue = [start]

    if start == goal:
        return 1 # path length is zero

    # Loop to traverse the graph
    # with the help of the queue
    while queue:
        node = queue.pop(0)

        # Condition to check if the current node is not visited
        if node not in explored :
            neighbours = adjacency_list[node-1]
            
            
            # Loop to iterate over the neighbours of the node
            for neighbour in neighbours:
                if (node != neighbour):
                    exploration_levels[neighbour-1] = exploration_levels[node-1] + 1
                    queue.append(neighbour)
                    explored.append(node)

                # Condition to check if the neighbour node is the goal
                if neighbour == goal:
                    return exploration_levels[neighbour-1]

    # Condition when the nodes are not connected
    return -1

mapf_model = Model("./MAPF_problem.mzn")
solver = Solver.lookup("chuffed")

f1 = open(sys.argv[1])
f2 = open(sys.argv[2])

line = f1.readline()
while (line[0:1] == "#"): # check comment lines for graph input file
    line = f1.readline()

num_vertices = int(line)
line = f1.readline()
num_edges = int(line)

adjacency_list = []
for node in range(num_vertices):
    adjacency_list.append({node+1})

for i in range(num_edges):
    line = f1.readline()
    v1 = int(line.split(" ")[0])
    v2 = int(line.split(" ")[1])

    adjacency_list[v1-1].add(v2)
    adjacency_list[v2-1].add(v1)


line = f2.readline()
while (line[0:1] == "#"): # check comment lines for graph input file
    line = f2.readline()

num_agents = int(line)
line = f2.readline()
start_states = []
for i in range(num_agents):
    line = f2.readline()
    start_states.append(int(line.split(" ")[1]))

line = f2.readline()
goal_states = []
for i in range(num_agents):
    line = f2.readline()
    goal_states.append(int(line.split(" ")[1]))

print(start_states)
print(goal_states)

i = 0
for agent in range(num_agents):
    distance = bfs_solve(adjacency_list, start_states[agent], goal_states[agent])
    print("from " + str(start_states[agent]) + " to " + str(goal_states[agent]) + " -> " + str(distance))
    if distance == -1: raise ValueError("Impossible problem, no possible path found")
    elif distance > i: i = distance

warnings.filterwarnings("ignore")
while True:
    print(i)
    instance = Instance(solver, mapf_model)
    instance["makespan_size"] = i
    instance["goal_states"] = goal_states
    instance["start_states"] = start_states
    instance["num_agents"] = num_agents
    instance["adjency_list"] = adjacency_list
    instance["num_vertices"] = num_vertices
    #run model
    result = instance.solve()

    if result.status != Status.UNSATISFIABLE: break
    i=i+1

print(result.solution)