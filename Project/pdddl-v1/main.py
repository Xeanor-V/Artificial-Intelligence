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

robot1 = []
robot2 = []
robots =[]
state = []
target = []

## this functions checks all the possible actions that we can take for a given robot and an state
## target it's our goal, we want to paint with the colors that we should

def moveRobot(robot, dx, dy):
    return [[robot[0][0] + dx, robot[0][1] + dy], robot[1], robot[2], robot[3]]


def removePaint(robot, d1, d2):
    return [robot[0], robot[1], robot[2] - d1, robot[3] - d2]


def changePaint(robot, change):
    return [robot[0], change, robot[2], robot[3]]


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


def solve(robots, iniState, target):
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
            done = True
            break;

        ### TODO right now it's working for two robots, planning to do it for N robots
        ### Get possibles for robot1 then try those as current States for robot2 and so on
        robot1Possibles, movement1 = getPossibles(current[0][0], current[1], target)
        index1 = 0
        for possible1 in robot1Possibles:
            robot2Possibles, movement2 = getPossibles(current[0][1], possible1[1], target)
            index2 = 0
            for possible2 in robot2Possibles:
                sequence = deepcopy(current[2])
                sequence.append("Robot1: " + movement1[index1])
                sequence.append("Robot2: " + movement2[index2])
                index2 += 1
                q.append([[possible1[0], possible2[0]], possible2[1], sequence])
            index1 += 1
    print(current)
    return done

if __name__ == '__main__':

    a = time.datetime.now()
    
    
    robot1, robot2, state, target = p.main()
    robot1[1]+=1
    robot2[1]+=1
    robot1[0][0] -=1
    robot2[0][0] -=1
    robots = [robot1, robot2]
    print(robots)
    print(state)
    print(target)
    print(solve(robots, state, target))

    print()
    print(time.datetime.now() - a)
    """

    robot1 = [[0, 3], 2, 1000, 1000]
    robot2 = [[1, 2], 1, 1000, 1000]

    robots = [robot1,robot2]
    state = [[0, 0, 0], [0, 0, 0], [0, 3, 0], [3, 0, 0], [0, 0, 0]]
    target = [[1, 2, 1], [2, 1, 2], [1, 2, 1], [2, 1, 2], [0, 0, 0]]
    #state = [[3, 0, 0, 0], [0, 3, 0, 0], [0, 0, 0, 0]]
    #target = [[1, 2, 0, 0], [0, 0, 1, 0], [1, 2, 0, 2]]
    #robot1 = [[1, 1], 2, 10, 10]
    #robot2 = [[0, 0], 1, 10, 10]
    #robots = [robot1, robot2]
    print("Robots: ")
    print(robots)
    print("State: ")
    print(np.array(state))
    print("Target: ")
    print(np.array(target))
    print(solve(robots, state, target))
    ### """

    """
        0 2 0
        1 0 1
        0 2 0
        3 0 0
        0 3 0
        0 0 0
    """


