import pddlpy


"""
PARSING COMPLETED

initRob1 = [[1, 3], 0, 100, 100] DONE
initRob2 = [[1, 2], 1, 100, 100] DONE
initState = [[0, 0, 0], [0, 0, 0], [0, 0, 0], [0, 3, 0]]
targetState = [[1, 2, 1], [2, 2, 2], [2, 2, 2], [4, 4, 4]] DONE
"""


def main():
    
    amount = 1000
    domprob = pddlpy.DomainProblem('domain-04.pddl', 'problem-12.pddl')
    initRob = []
    initState = []
    targetState = []
    
    max_robot = -300
    for o in domprob.initialstate():
        if ((str(o)).split(',')[0] == "('robot-at'"):
            robot = ((str(o)).split(',')[1])[7]
            Row =(((str(o)).split(',')[2]).split('-')[0]).split('_')[1]
            Column = (((str(o)).split(',')[2]).split('-')[1]).split("'")[0]
            element = [(int(Column)),(int(Row))]
            
            #In order to know how many robots we have
            if (int(robot) > max_robot):
                max_robot = int(robot)
    
    for i in range(0,max_robot):
        element = []
        initRob.append(None)

    for o in domprob.initialstate():
        if ((str(o)).split(',')[0] == "('robot-at'"):
            robot = ((str(o)).split(',')[1])[7]
            Row =(((str(o)).split(',')[2]).split('-')[0]).split('_')[1]
            Column = (((str(o)).split(',')[2]).split('-')[1]).split("'")[0]
            element = [(int(Column)),(int(Row))]
            element2 = []
            element2.append(element)
            element2.append(0)
            element2.append(amount)
            element2.append(amount)
            initRob[int(robot)-1] = element2

    for o in domprob.initialstate():
        if ((str(o)).split(',')[0] == "('robot-has'"):
            robot = ((str(o)).split(',')[1])[7]
            color = ((str(o)).split(',')[2])
            if(str(color) == " 'black')"):
                initRob[int(robot)-1][1]= 0
            else:
                initRob[int(robot)-1][1]= 1

    max_column= -3000
    max_rows= -3000
    for o in domprob.worldobjects():
        if ('tile') in o:
            x,column =  (str(o)).split('-')
            if (int(column)>= int(max_column)):
                max_column = column
            z,row = (str(x)).split('_')
            if(int(row)>= int(max_rows)+1):
                max_rows = row

    for i in range((int(max_rows)) +1):
        list = []
        list2= []
        for j in range(int(max_column)):
            list.append(0)
            list2.append(0)
        initState.append(list)
        targetState.append(list2)


    for i in range(0,max_robot):
        row_robot, column_robot = initRob[i][0]
        initState[column_robot][row_robot-1] = 3

    for g in domprob.goals():

        goal_row = (((str(g)).split(',')[1]).split('-')[0]).split('_')[1]
        goal_column = (((str(g)).split(',')[1]).split('-')[1]).split("'")[0]
        goal_color = ((str(g)).split(',')[2])
        
        if (str(goal_color) == " 'black')"):
            num_color = 2
        else:
            num_color = 1

        targetState[int(goal_row)-1][int(goal_column)-1] = int(num_color)

    return initRob, initState, targetState

