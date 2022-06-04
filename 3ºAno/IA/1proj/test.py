# test.py: Podem usar este script para testar a vossa implementação. Para tal, devem correr o
# script e verificar que o output que obtêm de cada teste é igual ao apresentado no enunciado do
# projeto. Além dos testes presentes, podem acrescentar outros.

from ricochet_robots import *
from search import *
import time

def test4():
    # Ler tabuleiro do ficheiro "i1.txt":
    board1 = parse_instance("instances/i7.txt")
    board2 = parse_instance("instances/i8.txt")
    board3 = parse_instance("instances/i1.txt")
    # Criar uma instância de RicochetRobots:
    problem1 = RicochetRobots(board1)
    problem2 = RicochetRobots(board2)
    problem3 = RicochetRobots(board3)


    # Obter o nó solução usando a procura A*:
    print("testing i7")
    start_time = time.time()
    astar_search(problem1)
    time1 = time.time()
    print("done bfs: " + str((time1 - start_time)*(10**3)))
    breadth_first_tree_search(problem1)
    time2 = time.time()
    print("done greedy: " + str((time2 - time1)*(10**3)))
    greedy_search(problem1)
    time3 = time.time()
    print("done iterative: " + str((time3 - time2)*(10**3)))
    iterative_deepening_search(problem1)
    time4 = time.time()
    print("done a star: " + str((time4 - time3)*(10**3)))

    print("testing i8")
    start_time = time.time()
    astar_search(problem2)
    time1 = time.time()
    print("done bfs: " + str((time1 - start_time)*(10**3)))
    breadth_first_tree_search(problem2)
    time2 = time.time()
    print("done greedy: " + str((time2 - time1)*(10**3)))
    greedy_search(problem2)
    time3 = time.time()
    print("done iterative: " + str((time3 - time2)*(10**3)))
    iterative_deepening_search(problem2)
    time4 = time.time()
    print("done a star: " + str((time4 - time3)*(10**3)))

    print("testing 10x10")
    start_time = time.time()
    astar_search(problem3)
    time1 = time.time()
    print("done bfs: " + str((time1 - start_time)*(10**3)))
    breadth_first_tree_search(problem3)
    time2 = time.time()
    print("done greedy: " + str((time2 - time1)*(10**3)))
    greedy_search(problem3)
    time3 = time.time()
    print("done iterative: " + str((time3 - time2)*(10**3)))
    iterative_deepening_search(problem3)
    time4 = time.time()
    print("done a star: " + str((time4 - time3)*(10**3)))

    compare_searchers([problem1, problem2, problem3], ['search', 'i7', 'i8', '10x10'])


    print("done.")

if __name__ == "__main__":
    test4()

