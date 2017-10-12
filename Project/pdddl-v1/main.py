"""
 robot =[ [x y] CurrentColor amount1 amount2]
 state = [0 0 0
          0 0 0
          0 0 0] where 0 is clear, 1 is white, 2 is black, 3 is robot, 4 robot can stand
 currentColor it's going to be represented as a number (2 black, 1 white)
 amount1 - black
 amount2 - white
"""

from copy import copy, deepcopy
import datetime as time
import numpy as np
import parse as p
import Interface2 as gui

robot1 = []
robot2 = []
robots =[]
state = []
target = []
columns = 0
rows = 0


#Path as a global variable
path = []

# Sequence
sequenceStates = []

# Robots
sequenceRobots =[]

# Keeping track during DP
sequenceMovements = []

## trying all the possible combinations between N robots
def getSequence(robots,index,currState,seqMov,seqRobots,target):
    if(index == len(robots)):
        global sequenceStates,sequenceRobots,sequenceMovements
        sequenceMovements.append(seqMov)
        sequenceStates.append(currState)
        sequenceRobots.append(seqRobots)
        return
    possibles,movement = getPossiblesFOC(robots[index],currState,target)
    for i in range(len(possibles)):
        auxMov = deepcopy(seqMov)
        auxMov.append("Robot"+str(index+1) +": " +str(movement[i]))
        auxRobots= deepcopy(seqRobots)
        auxRobots.append(possibles[i][0])
        getSequence(robots, index+1, possibles[i][1],auxMov,auxRobots,target)
    return


## this functions checks all the possible actions that we can take for a given robot and an state
## target it's our goal, we want to paint with the colors that we should

def moveRobot(robot, dx, dy):
    return [[robot[0][0] + dx, robot[0][1] + dy], robot[1], robot[2], robot[3]]


def removePaint(robot, d1, d2):
    return [robot[0], robot[1], robot[2] - d1, robot[3] - d2]


def changePaint(robot, change):
    return [robot[0], change, robot[2], robot[3]]



def getPossiblesFOC(robot, state, target):
    s = []
    states = []
    """
        ## In that case, we will add a FOC ( adding preconditions). Using this constraints we are saying that paint_down is not a possible action.
        
        """
    if ((robot[0][1] + 1) < len(state) and state[robot[0][1] + 1][robot[0][0]] == 0):
        row = robot[0][1]
        column = robot[0][0]
        painted = True
        if (row != 0):
            for i in range(0,row):
                for j in range(0, columns):
                    if ((state[i][j] != 2) and (state[i][j] != 1)):
                        painted = False
        if (painted == True):
            s.append("down")
            aux = deepcopy(state)
            aux[robot[0][1]][robot[0][0]] = 0
            aux[robot[0][1] + 1][robot[0][0]] = 3
            states.append([moveRobot(robot, 0, 1), aux])

    if ((robot[0][1] - 1) >= 0 and state[robot[0][1] - 1][robot[0][0]] == 0):
        s.append("up")
        aux = deepcopy(state)
        aux[robot[0][1]][robot[0][0]] = 0
        aux[robot[0][1] - 1][robot[0][0]] = 3
        states.append([moveRobot(robot, 0, -1), aux])
    
    if ((robot[0][0] + 1) < len(state[0]) and state[robot[0][1]][robot[0][0] + 1] == 0):
        s.append("right")
        aux = deepcopy(state)
        aux[robot[0][1]][robot[0][0]] = 0
        aux[robot[0][1]][robot[0][0] + 1] = 3
        states.append([moveRobot(robot, 1, 0), aux])
    
    if ((robot[0][0] - 1) >= 0 and state[robot[0][1]][robot[0][0] - 1] == 0):
        s.append("left")
        aux = deepcopy(state)
        aux[robot[0][1]][robot[0][0]] = 0
        aux[robot[0][1]][robot[0][0] - 1] = 3
        states.append([moveRobot(robot, -1, 0), aux])
    
    ## if up is clear / we have paint / we need to paint it like that / and the "up"- row is already painted..
    if (("up") in s and robot[1 + robot[1]] > 0 and target[robot[0][1] - 1][robot[0][0]] == robot[1]):
        #Where you want to paint
        row_robot = robot[0][1]-1
        column_robot = robot[0][0]
        painted = True
        if (row_robot != 0):
            for i in range(0,row_robot):
                for j in range(0,columns):
                    if ((state[i][j] != 2) and (state[i][j] != 1)):
                        painted = False
        if (painted == True):
            s.append("paint_up")
            aux = deepcopy(state)
            aux[robot[0][1] - 1][robot[0][0]] = robot[1]
            d1 = d2 = 0
            if (robot[1] == 1):
                d1 = 1
            else:
                d2 = 1
            states.append([removePaint(robot, d1, d2), aux])
    ## todo append SAT for painting
    if(robot[ robot[1] +  1 ] >0):
        if(robot[1] == 1):
            auxR = changePaint(robot,2)
            s.append("change_paint_black")
        else:
            auxR = changePaint(robot,1)
            s.append("change_paint_white")
        states.append([auxR, state])
    s.append("still")
    states.append( [robot, state])
    return states, s


def getPossibles(robot, state, target):
    s = []
    states = []
    """
    ## checking if the adjacent cell exists and if it's clear
    ## exists -> it's in the bounds of the board
    ## for each possible movement we get the the new states also
    """

    if ((robot[0][1] + 1) < len(state) and state[robot[0][1] + 1][robot[0][0]] == 0):
        s.append("down")
        aux = deepcopy(state)
        aux[robot[0][1]][robot[0][0]] = 0
        aux[robot[0][1] + 1][robot[0][0]] = 3
        states.append([moveRobot(robot, 0, 1), aux])

    if ((robot[0][1] - 1) >= 0 and state[robot[0][1] - 1][robot[0][0]] == 0):
        s.append("up")
        aux = deepcopy(state)
        aux[robot[0][1]][robot[0][0]] = 0
        aux[robot[0][1] - 1][robot[0][0]] = 3
        states.append([moveRobot(robot, 0, -1), aux])

    if ((robot[0][0] + 1) < len(state[0]) and state[robot[0][1]][robot[0][0] + 1] == 0):
        s.append("right")
        aux = deepcopy(state)
        aux[robot[0][1]][robot[0][0]] = 0
        aux[robot[0][1]][robot[0][0] + 1] = 3
        states.append([moveRobot(robot, 1, 0), aux])

    if ((robot[0][0] - 1) >= 0 and state[robot[0][1]][robot[0][0] - 1] == 0):
        s.append("left")
        aux = deepcopy(state)
        aux[robot[0][1]][robot[0][0]] = 0
        aux[robot[0][1]][robot[0][0] - 1] = 3
        states.append([moveRobot(robot, -1, 0), aux])

    ## if up is clear / we have paint / we need to paint it like that
    if (("up") in s and robot[1 + robot[1]] > 0 and target[robot[0][1] - 1][robot[0][0]] == robot[1]):
        s.append("paint_up")
        aux = deepcopy(state)
        aux[robot[0][1] - 1][robot[0][0]] = robot[1]
        d1 = d2 = 0
        if (robot[1] == 1):
            d1 = 1
        else:
            d2 = 1
        states.append([removePaint(robot, d1, d2), aux])

    if (("down") in s and robot[1 + robot[1]] > 0 and target[robot[0][1] + 1][robot[0][0]] == robot[1]):
        s.append("paint_down")
        aux = deepcopy(state)
        aux[robot[0][1] + 1][robot[0][0]] = robot[1]
        d1 = d2 = 0
        if (robot[1] == 1):
            d1 = 1
        else:
            d2 = 1
        states.append([removePaint(robot, d1, d2), aux])

    ## todo append SAT for painting
    if(robot[ robot[1] +  1 ] >0):
        if(robot[1] == 1):
            auxR = changePaint(robot,2)
            s.append("change_paint_black")
        else:
            auxR = changePaint(robot,1)
            s.append("change_paint_white")
        states.append([auxR, state])


    s.append("still")
    states.append( [robot, state])
    return states, s
    ##return s



""" BFS algorithm to solve the problem
    @robots - list of robots defined as [ [x,y], color, color1Remaining, color2Remaining]
    @iniState - matrix that defines the initial configuration
    @target - matrix that defines the target configuration
"""


def solve(how_many_robots,robots, iniState, target):
    ### using list as queue and dictionary as hashmap
    q = []
    visited = {}
    ### initializing the queue for the BFS
    ### the state it's represented as current state of robots
    ### current state of the board
    ### and the list of movements
    q.append([robots, iniState, []])
    ### Using np arrays because of comparation function between matrices
    targetCheck = np.array(target)
    ### flag to check if we were able to achieve the target
    done = False;
    current = None
    robotPossibles = []
    robotMovement = []
    index  = []
    for i in range(0,how_many_robots):
        element = []
        num = 0
        robotPossibles.append(element)
        index.append(num)
        robotMovement.append(element)
    
    while q:
        current = q[0]
        q.pop(0)
        currentCheck = np.array(current[1])
        ### converting the state to string for hashing
        keyState = str(current[1])
        KeyRobots = str(current[0])
        ### checking if we have been in this state before
        value1 = visited.get(keyState)
        value2 = visited.get(KeyRobots)
        if value1!= None: ##value1 != None and value2 != None:
            continue
        ### marking and hashing state
        visited[keyState] = True
        visited[KeyRobots] = True

        ### Checking if our current board it's our target
        currentCheck[currentCheck == 3] = 0
        if (currentCheck == targetCheck).all():
            print(currentCheck)
            print(current[2])
            global path
            path = current[2]
            done = True
            break;

        ### TODO right now it's working for two robots, planning to do it for N robots
        ### Get possibles for robot1 then try those as current States for robot2 and so on
        global sequenceRobots,sequenceStates,sequenceMovements
        # Sequence
        sequenceStates = []
        # Robots
        sequenceRobots =[]
        # Keeping track during DP
        sequenceMovements = []

        getSequence(current[0],0,current[1],[],[],target)
        for i in range(len(sequenceStates)):
            aux = deepcopy(current[2])
            aux.extend(sequenceMovements[i])
            q.append( [sequenceRobots[i], sequenceStates[i], aux]    )
    return done



        


if __name__ == '__main__':

    a = time.datetime.now()
    robots,state,target = p.main()
 
    how_many_robots = len(robots)
    for i in range(0,how_many_robots):
        robots[i][1] +=1
        robots[i][0][0] -=1
    rows = len(state)
    columns = len(state[0])
    solution = False
    solution = solve(how_many_robots,robots, state, target)
    print("Time: ")
    print(time.datetime.now() - a)
    if (solution == True):
        print("Solved")
        gui.Draw(robots,state,path)
    else:
        print("To solve")

