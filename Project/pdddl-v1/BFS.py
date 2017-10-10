
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
    return [ robot[0][0]+dx, robot[0][1]+dy, robot[1], robot[2], robot[3] ]
def removePaint(robot,d1,d2):
    return [ robot[0], robot[1], robot[2]-d1, robot[3]-d2]

def getPossibles(robot,state,target):
    s = set()
    states = []

    ## checking if the adjacent cell exists and if it's clear
    ## exists -> it's in the bounds of the board
    if((robot[0][1] + 1) < len(state) and state[ robot[0][1] + 1 ][ robot[0][0] ] == 0  ):
        s.add("down")
        aux = deepcopy(state)
        aux[ robot[0][1]][robot[0][0]] = 0
        aux[ robot[0][1] + 1 ][ robot[0][0] ] = 3
        states.append([moveRobot(robot,0,1),aux ])
    if((robot[0][1] -1) >= 0 and state[ robot[0][1] - 1 ][ robot[0][0] ] == 0):
        s.add("up")
        aux = deepcopy(state)
        aux[ robot[0][1]][robot[0][0]] = 0
        aux[ robot[0][1] - 1 ][ robot[0][0] ] = 3
        states.append([ moveRobot(robot,0,-1),aux ])
    if((robot[0][0] + 1) < len(state[0]) and state[ robot[0][1] ][ robot[0][0] + 1] == 0   ):
        s.add("right")
        aux = deepcopy(state)
        aux[ robot[0][1]][robot[0][0]] = 0
        aux[ robot[0][1] ][ robot[0][0] +1 ] = 3
        states.append([ moveRobot(robot,1,0),aux ])
    if((robot[0][0] -1) >= 0 and state[robot[0][1]][robot[0][0]-1] == 0):
        s.add("left")
        aux = deepcopy(state)
        aux[ robot[0][1]][robot[0][0]] = 0
        aux[ robot[0][1] ][ robot[0][0] -1 ] = 3
        states.append([ moveRobot(robot,-1,0),aux ])

    ## if up is clear / we have paint / we need to paint it like that
    if( ("up") in s and robot[1 + robot[1]] > 0 and target[robot[0][1]-1][robot[0][0]] == robot[1]):
        s.add("paint_up")
        aux = deepcopy(state)
        aux[ robot[0][1] - 1 ][ robot[0][0] ] = robot[1]
        d1 = d2 = 0
        if(robot1[1] == 0):
            d1 = 1
        else:
            d2 = 1
        states.append( [removePaint(robot,d1,d2), aux])
    if( ("down") in s and robot[1 + robot[1]] > 0 and target[robot[0][1]+1][robot[0][0]] == robot[1]):
        s.add("paint_down")
        aux = deepcopy(state)
        aux[ robot[0][1] + 1 ][ robot[0][0] ] = robot[1]
        d1 = d2 = 0
        if(robot1[1] == 0):
            d1 = 1
        else:
            d2 = 1
        states.append( [removePaint(robot,d1,d2), aux])

    ## todo add SAT for painting
    s.add("change_paint")
    return s,states




def solve(robot, iniState):
    return

robot1 = [ [1,1], 2, 10,10]
##robot2 = 
##robots = [robot]
state = [[0,0,0], [1,0,1], [0,0,0]]
target = [[0,2,0], [1,0,1], [0,2,0]]

print(getPossibles(robot1,state,target))
    
        



