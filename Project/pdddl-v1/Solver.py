import sys
from  pddlpy import DomainProblem


def possible_Movements(action,H,W,domprob):
    W+=1
    movex = 0
    movey = 0
    if(action == "up"):
        movey = 1
    elif(action == "down"):
        movey = -1
    elif(action == "left"):
        movex = -1
    else:
        movex = 1
    movements = {}
    indexes = list(reversed(list(range(H))))
    print(indexes) 
    for i in indexes:
        for j in range(1,W):
            if( i+movey >= 0 and i+movey < H ):
                if(j+movex >= 0 and j+movex < W):
                    currentTile = "tile_" + str(i) +"-" + str(j)
                    targetTile = "tile_" + str(i + movey) +"-" + str(j + movex)
                    movements[currentTile] = []
                    print(currentTile,targetTile)
                    for o in domprob.ground_operator(action):
                        if(action,targetTile,currentTile) in o.precondition_pos:
                            movements[currentTile].append(o.precondition_pos)
    return movements

def possible_Paint(H,W,domprob):
    W+=1
    movex = 0
    movey = 1
    movements_up = {}
    movements_down = {}
    action = "up"
    indexes = list(reversed(list(range(H))))
    print(indexes) 
    for i in indexes:
        for j in range(1,W):
            if( i+movey >= 0 and i+movey < H ):
                if(j+movex >= 0 and j+movex < W):
                    currentTile = "tile_" + str(i) +"-" + str(j)
                    targetTile = "tile_" + str(i + movey) +"-" + str(j + movex)
                    movements_up[currentTile] = []
                    print(currentTile,targetTile)
                    for o in domprob.ground_operator(action):
                        if(action,targetTile,currentTile) in o.precondition_pos:
                            movements_up[currentTile].append(o.precondition_pos)
    action = "paint-down"
    movey = -1
    print("######")
    for i in indexes:
        for j in range(1,W):
            if( i+movey >= 0 and i+movey < H ):
                if(j+movex >= 0 and j+movex < W):
                    currentTile = "tile_" + str(i) +"-" + str(j)
                    targetTile = "tile_" + str(i + movey) +"-" + str(j + movex)
                    movements_down[currentTile] = []
                    print(currentTile,targetTile)
                    for o in domprob.ground_operator(action):
                        if(action,targetTile,currentTile) in o.precondition_pos:
                            movements_down[currentTile].append(o.precondition_pos)
    return [movements_up,movements_down]



##robot = [ x,y,color,color1,color2]

robot = [0,1,"black",10,10]

    


def main(argv):
    ##demonumber = int(argv[1])
    domainfile = "domain-04.pddl" ##% demonumber
    problemfile = "problem-04.pddl" ##% demonumber
    domprob = DomainProblem(domainfile, problemfile)
    H = 5
    W = 3
    res = possible_Movements("up",H,W,domprob)
    print(res)
    print("-------")
    res = possible_Paint(H,W,domprob)
    print(res[0])
    print(res[1])

    


    
 


if __name__ == '__main__':
    main(sys.argv)