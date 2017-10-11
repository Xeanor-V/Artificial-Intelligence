import pddlpy


"""
PARSING COMPLETED

initRob1 = [[1, 3], 0, 100, 100] DONE
initRob2 = [[1, 2], 1, 100, 100] DONE
initState = [[0, 0, 0], [0, 0, 0], [0, 0, 0], [0, 3, 0]]
targetState = [[1, 2, 1], [2, 2, 2], [2, 2, 2], [4, 4, 4]] DONE
"""

# only works with 2 robots

amount = 1000
domprob = pddlpy.DomainProblem('domain-04.pddl', 'problem-04.pddl')

initRob1 = []
initRob2 = []
initState = []
targetState = []

def main():
    for o in domprob.initialstate():
        if ((str(o)).split(',')[0] == "('robot-at'"):
            robot = ((str(o)).split(',')[1])[7]
            Row =(((str(o)).split(',')[2]).split('-')[0]).split('_')[1]
            Column = (((str(o)).split(',')[2]).split('-')[1]).split("'")[0]
            element = [(int(Column)),(int(Row))]
            if (int(robot) == 1):
                initRob1.append(element)
                initRob1.append(0)
                initRob1.append(amount)
                initRob1.append(amount)
            elif(int(robot) == 2):
                initRob2.append(element)
                initRob2.append(0)
                initRob2.append(amount)
                initRob2.append(amount)

    for o in domprob.initialstate():
        if ((str(o)).split(',')[0] == "('robot-has'"):
            robot = ((str(o)).split(',')[1])[7]
            color = ((str(o)).split(',')[2])
            if(str(color) == " 'black')"):
                if (int(robot) == 2):
                    initRob2[1] = 0
                elif (int(robot) == 1):
                    initRob1[1] = 0
            else:
                if (int(robot) == 2):
                    initRob2[1] = 1
                elif (int(robot) == 1):
                    initRob1[1] = 1

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
    rob1_row, rob1_column = initRob1[0]
    rob2_row, rob2_column = initRob2[0]

    initState[rob1_column][rob1_row-1] = 3
    initState[rob2_column][rob2_row-1] = 3

    for g in domprob.goals():

        goal_row = (((str(g)).split(',')[1]).split('-')[0]).split('_')[1]
        goal_column = (((str(g)).split(',')[1]).split('-')[1]).split("'")[0]
        goal_color = ((str(g)).split(',')[2])
        if (str(goal_color) == " 'black')"):
            num_color = 2
        else:
            num_color = 1

        targetState[int(goal_row)-1][int(goal_column)-1] = int(num_color)

    return initRob1, initRob2, initState, targetState

