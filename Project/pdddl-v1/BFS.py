
"""
 robot =[ [x y] CurrentColor amount1 amount2]
 state = [0 0 0
          0 0 0
          0 0 0] where 0 is clear, 1 is white, 2 is black, 3 is robot
 currentColor it's going to be represented as a number (0 black, 1 white)
 amount1 - black
 amount2 - white
"""


def possible(robot, state, mov):
    dy = robot[0] + mov[0]
    dx = robot[1] + mov[1]
    if(dy >= 0 and dy< len(state)):
        if(dx>=0 and dx < len(state[0])):
            return True
    return False

## this functions checks all the possible actions that we can take for a given robot and an state
## target it's our goal, we want to paint with the colors that we should
def getPossibles(robot,state,target):
    s = set();

    ## checking if the adjacent cell exists and if it's clear
    ## exists -> it's in the bounds of the board
    if((robot[0][1] + 1) < len(state) and state[ robot[0][1] + 1 ][ robot[0][0] ] == 0  ):
        s.add("down")
    if((robot[0][1] -1) >= 0 and state[ robot[0][1] - 1 ][ robot[0][0] ] == 0):
        s.add("up")
    if((robot[0][0] + 1) < len(state[0]) and state[ robot[0][1] ][ robot[0][0] + 1] == 0   ):
        s.add("right")
    if((robot[0][0] -1) >= 0 and state[robot[0][1]][robot[0][0]-1] == 0):
        s.add("left")

    ## if up is clear / we have paint / we need to paint it like that
    if( ("up") in s and robot[2 + robot[1]] > 0 and target[robot[0][1]-1][robot[0][0]] == robot[1]):
        s.add("paint_up")
    if( ("down") in s and robot[2 + robot[1]] > 0 and target[robot[0][1]+1][robot[0][0]] == robot[1]):
        s.add("paint_down")

    ## todo add SAT for painting
    s.add("change_paint")
    return s


def solve(robot, iniState):
    return

robot = [ [1,1], 0, 10,10]
state = [[0,0,0], [1,0,1], [0,0,0]]
target = [[0,0,0], [1,0,1], [0,0,0]]

print(getPossibles(robot,state,target))
    
        



