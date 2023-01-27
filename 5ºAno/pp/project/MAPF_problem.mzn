% ------------------------ input constants ------------------------------------

int: num_vertices;
int: num_agents;
int: makespan_size;
 
set of int: VERTICES = 1..num_vertices;
set of int: AGENTS = 1..num_agents;
set of int: MAKESPAN = 1..makespan_size;
array[VERTICES] of set of int: adjency_list;

array[AGENTS] of int: start_states;
array[AGENTS] of int: goal_states;

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

constraint % make sure their start and goal states match
  forall(agent in AGENTS)
  (
    makespans[agent, 1] = start_states[agent] /\
    makespans[agent, makespan_size] = goal_states[agent] 
  );

constraint % make sure paths are adjacent
  forall(agent in AGENTS)
  (
      forall(time in MAKESPAN where time != 1)
      (
        makespans[agent, time] in adjency_list[makespans[agent, time - 1]]
      )
  );

constraint % makespans without any changes to position are redundant
  redundant_constraint(forall(time in MAKESPAN where time != 1)
  (
    [makespans[agent, time] | agent in AGENTS] != [makespans[agent, time-1] | agent in AGENTS]
  ));
    
solve :: int_search([makespans[agent,time] | time in MAKESPAN, agent in AGENTS], input_order, indomain_min)
  satisfy;

% TIRAR PROVEITO DO int_search

% ------------------------ output ----------------------------------------

output ["i=\(time - 1)\t" ++ 
        concat(["\(agent):\(makespans[agent, time])" ++ if agent = num_agents then "\n" else " " endif 
        | agent in AGENTS])
        | time in MAKESPAN];