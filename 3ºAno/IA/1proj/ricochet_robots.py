# ricochet_robots.py: Template para implementação do 1º projeto de Inteligência Artificial 2020/2021.
# Devem alterar as classes e funções neste ficheiro de acordo com as instruções do enunciado.
# Além das funções e classes já definidas, podem acrescentar outras que considerem pertinentes.

# Grupo 14:
# 93689 António Venâncio
# 93711 Francisco Rodrigues

from search import Problem, Node, astar_search, breadth_first_tree_search, \
    depth_first_tree_search, greedy_search, iterative_deepening_search
import sys, copy

class RRState:
    state_id = 0

    def __init__(self, board):
        self.board = board
        self.id = RRState.state_id
        RRState.state_id += 1

    def __lt__(self, other):
        """ Este método é utilizado em caso de empate na gestão da lista
        de abertos nas procuras informadas. """
        return self.id < other.id


class Board:
    """ Representacao interna de um tabuleiro de Ricochet Robots. """
    orientations = {'l': (0, -1), 'r': (0, 1), 'u': (-1, 0), 'd': (1, 0)}

    def __init__(self, size, target, robots, barriers):
        self.size = size
        self.robots = robots
        self.barriers = barriers
        self.target = target

    def robot_position(self, robot: str):
        """ Devolve a posição atual do robô passado como argumento. """
        for position in self.robots:
            if self.robots[position] == robot:
                return position

    def checkObstacles(self, start_position, direction: str) -> bool:
        """ Recebendo uma posição uma direção, verifica
        se pode deslocar para a posição adjacente sem
        encontrar um obstáculo
        (i.e: robot, barreira, fronteira)"""
        adjPosition = self.getAdjPosition(start_position, direction)
        if not self.checkBoundaries(adjPosition) or self.robots.get(adjPosition) is not None:
            return False
        elif start_position in self.barriers and direction in self.barriers.get(start_position):
            return False
        elif adjPosition in self.barriers and self.get_opposite_direction(direction) in self.barriers.get(adjPosition):
            return False
        else:
            return True

    def checkBoundaries(self, position):
        """check if position is inside the board"""
        return 1 <= position[0] <= self.size and 1 <= position[1] <= self.size

    @staticmethod
    def getAdjPosition(position, direction) -> tuple:
        """return the adjacent position in the given direction"""
        return (position[0] + Board.orientations.get(direction)[0], position[1] + Board.orientations.get(direction)[1])

    @staticmethod
    def get_opposite_direction(direction):
        if direction == 'l': return 'r'
        if direction == 'r': return 'l'
        if direction == 'u': return 'd'
        if direction == 'd': return 'u'


def parse_instance(filename: str) -> Board:
    """ Lê o ficheiro cujo caminho é passado como argumento e retorna
    uma instância da classe Board. """
    f = open(filename, "r")
    size_board = int(f.readline())
    barriers = {}
    robots = {}

    for i in range(4):
        robot_info = f.readline()
        robots[(int(robot_info[2]), int(robot_info[4]))] = robot_info[0]

    robot_info = f.readline()
    robot_goal = (robot_info[0], (int(robot_info[2]), int(robot_info[4])))

    num_barriers = int(f.readline())
    for i in range(num_barriers):
        obstacle = f.readline()
        if (int(obstacle[0]), int(obstacle[2])) not in barriers:
            barriers[(int(obstacle[0]), int(obstacle[2]))] = obstacle[4]
        else:
            barriers[(int(obstacle[0]), int(obstacle[2]))] += obstacle[4]

    return Board(size_board, robot_goal, robots, barriers)


class RicochetRobots(Problem):
    def __init__(self, board: Board):
        """ O construtor especifica o estado inicial. """
        super().__init__(RRState(board))

    def actions(self, state: RRState):
        """ Retorna uma lista de ações que podem ser executadas a
        partir do estado passado como argumento. """
        moves = ()
        for position in state.board.robots:
            color = state.board.robots.get(position)

            if state.board.checkObstacles(position, 'l'): moves += ((color, 'l'),)
            if state.board.checkObstacles(position, 'r'): moves += ((color, 'r'),)
            if state.board.checkObstacles(position, 'u'): moves += ((color, 'u'),)
            if state.board.checkObstacles(position, 'd'): moves += ((color, 'd'),)
        return moves

    def result(self, state: RRState, action):
        """ Retorna o estado resultante de executar a 'action' sobre
        'state' passado como argumento. A ação retornada deve ser uma
        das presentes na lista obtida pela execução de
        self.actions(state). """
        board = state.board
        if action not in self.actions(state):
            return state
        else:
            old_position = board.robot_position(action[0])
            new_position = old_position
            direction = action[1]
            while (board.checkObstacles(new_position, direction)):
                new_position = Board.getAdjPosition(new_position, direction)

            new_state = RRState(Board(board.size, board.target, board.robots, board.barriers))
            new_state.board.robots = state.board.robots.copy()
            new_state.board.robots[new_position] = new_state.board.robots.pop(old_position)
            return new_state

    def goal_test(self, state: RRState):
        """ Retorna True se e só se o estado passado como argumento é
        um estado objetivo. Deve verificar se o alvo e o robô da
        mesma cor ocupam a mesma célula no tabuleiro. """
        color = state.board.target[0]
        final = state.board.robots.get(state.board.target[1])
        return final is not None and final == color

    def h(self, node: Node):
        #Função heuristica utilizada para a procura A*.
        board = node.state.board

        target = board.target
        robot_position = board.robot_position(target[0])
        target_row, target_column = target[1]
        dx = abs(robot_position[0] - target_row)
        dy = abs(robot_position[1] - target_column)
        return dx + dy

if __name__ == "__main__":
    # Ler o ficheiro de input de sys.argv[1],
    board = parse_instance(sys.argv[1])
    # Usar uma técnica de procura para resolver a instância,
    problem = RicochetRobots(board)
    node = iterative_deepening_search(problem)
    # Retirar a solução a partir do nó resultante,
    actions = node.solution()
    # Imprimir para o standard output no formato indicado.
    print(len(actions))
    for action in actions:
        print(action[0] + ' ' + action[1])