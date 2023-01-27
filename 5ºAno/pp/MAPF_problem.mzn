% ------------------------ input constants ------------------------------------

int: num_vertices;
int: num_agents;
int: makespan_size;
 
set of int: VERTICES = 1..num_vertices;
set of int: AGENTS = 1..num_agents;
set of int: MAKESPAN = 1..makespan_size;
array[VERTICES, VERTICES] of 0..1: adjency_list;      % TENTAR ARRAY[SET of int], se trabalhar com o shuf solver

array[AGENTS] of VERTICES: start_states;
array[AGENTS] of VERTICES: goal_states;

% ------------------------ decision variables ---------------------------------

array[AGENTS, MAKESPAN] of var VERTICES: makespans;

% ------------------------ constraints ----------------------------------------
constraint % Agents can't collide with each other
  forall(agent1, agent2 in AGENTS where agent1 != agent2)          % COLOCAR PRIMEIRO AS CONSTRAINTS QUE MAIS RESTRINGEM AS VARIÁVEIS
  (
    forall(time in MAKESPAN where time != 1)
    (
      makespans[agent1, time] != makespans[agent2, time - 1] /\
      makespans[agent1, time] != makespans[agent2, time]        
    )
  );

constraint % make sure paths are adjacent
  forall(agent in AGENTS)
  (
      forall(time in MAKESPAN where time != 1)
      (
        adjency_list[makespans[agent, time], makespans[agent, time - 1]] = 1
      )
  );
  
constraint % make sure their start and goal states match
  forall(agent in AGENTS)
  (
    makespans[agent, 1] = start_states[agent] /\
    makespans[agent, makespan_size] = goal_states[agent] 
  );
  
solve :: int_search(makespans, first_fail, indomain_random)
  satisfy;

% TIRAR PROVEITO DO int_search

% ------------------------ output ----------------------------------------

output ["i=\(time - 1)\t" ++ 
        concat(["\(agent):\(makespans[agent, time])" ++ if agent = num_agents then "\n" else " " endif 
        | agent in AGENTS])
        | time in MAKESPAN];