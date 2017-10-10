
"""
 robot =[ [x y] CurrentColor amount1 amount2]
 state = [0 0 0
          0 0 0
          0 0 0] where 0 is clear, 1 is white, 2 is black, 3 is robot
 currentColor it's going to be represented as a number (0 black, 1 white)
 amount1 - black
 amount2 - white
"""
from copy import copy, deepcopy
import numpy as np

def possible(robot, state, mov):
    dy = robot[0] + mov[0]
    dx = robot[1] + mov[1]
    if(dy >= 0 and dy< len(state)):
        if(dx>=0 and dx < len(state[0])):
            return True
    return False

## this functions checks all the possible actions that we can take for a given robot and an state
## target it's our goal, we want to paint with the colors that we should

def moveRobot(robot,dx,dy):
    return [ [robot[0][0]+dx, robot[0][1]+dy], robot[1], robot[2], robot[3] ]
def removePaint(robot,d1,d2):
    return [ robot[0], robot[1], robot[2]-d1, robot[3]-d2]
def changePaint(robot,change):
    return [ robot[0], change, robot2[2], robot[3]]

def getPossibles(robot,state,target):
    s = []
    states = []
    """
    ## checking if the adjacent cell exists and if it's clear
    ## exists -> it's in the bounds of the board
    ## for each possible movement we get the the new states also
    """

    if((robot[0][1] + 1) < len(state) and state[ robot[0][1] + 1 ][ robot[0][0] ] == 0  ):
        s.append("down")
        aux = deepcopy(state)
        aux[ robot[0][1] ][robot[0][0]] = 0
        aux[ robot[0][1] + 1 ][ robot[0][0] ] = 3
        states.append([moveRobot(robot,0,1),aux ])

    if((robot[0][1] -1) >= 0 and state[ robot[0][1] - 1 ][ robot[0][0] ] == 0):
        s.append("up")
        aux = deepcopy(state)
        aux[ robot[0][1]][robot[0][0]] = 0
        aux[ robot[0][1] - 1 ][ robot[0][0] ] = 3
        states.append([ moveRobot(robot,0,-1),aux ]) 

    if((robot[0][0] + 1) < len(state[0]) and state[ robot[0][1] ][ robot[0][0] + 1] == 0   ):
        s.append("right")
        aux = deepcopy(state)
        aux[ robot[0][1]][robot[0][0]] = 0
        aux[ robot[0][1] ][ robot[0][0] +1 ] = 3
        states.append([ moveRobot(robot,1,0),aux ])
    
    if((robot[0][0] -1) >= 0 and state[robot[0][1]][robot[0][0]-1] == 0):
        s.append("left")
        aux = deepcopy(state)
        aux[ robot[0][1]][robot[0][0]] = 0
        aux[ robot[0][1] ][ robot[0][0] -1 ] = 3
        states.append([ moveRobot(robot,-1,0),aux ])

    ## if up is clear / we have paint / we need to paint it like that
    if( ("up") in s and robot[1 + robot[1]] > 0 and target[robot[0][1]-1][robot[0][0]] == robot[1]): 
        s.append("paint_up")
        aux = deepcopy(state)
        aux[ robot[0][1] - 1 ][ robot[0][0] ] = robot[1]
        d1 = d2 = 0
        if(robot1[1] == 1):
            d1 = 1
        else:
            d2 = 1
        states.append( [removePaint(robot,d1,d2), aux])
    
    if( ("down") in s and robot[1 + robot[1]] > 0 and target[robot[0][1]+1][robot[0][0]] == robot[1]):
        s.append("paint_down")
        aux = deepcopy(state)
        aux[ robot[0][1] + 1 ][ robot[0][0] ] = robot[1]
        d1 = d2 = 0
        if(robot1[1] == 1):
            d1 = 1
        else:
            d2 = 1
        states.append( [removePaint(robot,d1,d2), aux])

    ## todo append SAT for painting
    s.append("change_paint")

    s.append("still")
    ##return s
    return states,s



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
    q.append( [robots,iniState,[]])
    ### Using np arrays because of comparation function between matrices 
    targetCheck = np.array(target)
    ### flag to check if we were able to achieve the target
    done = False;
    while q:
        current = q[0]
        q.pop(0)
        currentCheck = np.array(current[1])
        ### converting the state to string for hashing
        KeyCurrent = str(current[1])
        ### checking if we have been in this state before
        if KeyCurrent in visited:
            continue
        ### marking and hashing state
        visited[str(current[1])] = True

        ### Checking if our current board it's our target
        currentCheck[currentCheck == 3] = 0
        if (currentCheck == targetCheck).all():
            print(currentCheck)
            print(current[2])
            done = True
            break;

        ### TODO right now it's working for two robots, planning to do it for N robots
        ### Get possibles for robot1 then try those as current States for robot2 and so on
        robot1Possibles,movement1 = getPossibles(current[0][0],current[1],target)
        index1 = 0
        for possible1 in robot1Possibles:
            robot2Possibles,movement2 = getPossibles(current[0][1],possible1[1],target)
            index2 = 0
            for possible2 in robot2Possibles:
                sequence = deepcopy(current[2])
                sequence.append("Robot1: " + movement1[index1])
                sequence.append("Robot2: " + movement2[index2])
                index2+=1
                q.append( [ [possible1[0],possible2[0]], possible2[1], sequence])
            index1+=1
    return done



robot1 = [ [1,1], 2, 10,10]
robot2 = [ [0 ,0],1, 10,10]

"""
        0 2 0
        1 0 1
        0 2 0

        3 0 0
        0 3 0
        0 0 0

"""

robots = [robot1,robot2]
state = [[3,0,0,0], [0,3,0,0], [0,0,0,0]]
target = [[1,2,0,0], [0,0,1,0], [1,2,0,2]]

##print(getPossibles(robot1,state,target))

print(solve(robots,state,target))
    
        



