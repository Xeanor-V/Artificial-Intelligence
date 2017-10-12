import turtle

#Definition of robot's path color
robot_path = ["orange","blue","pink","red","yellow"]
robot_has_color = ["black","white","black","white","black"]

#Coordinates
offsetX = -200
offsetY = -300


def setInitialData(robots,initialState,how_many_robots):
    
    total_rows = len(initialState)
    total_columns = len(initialState[0])
    
    robot_column = []
    robot_row = []

    for i in range(0,how_many_robots):
        robot_row.append(0)
        robot_column.append(0)

    for i in range(0,how_many_robots):
        robot_row[i] = robots[i][0][1]
        robot_column[i] = robots[i][0][0]
        color = robots[i][1]
        if (color == 1):
            robot_has_color[i] = "white"
        else:
            robot_has_color[i] = "black"

    return robot_row, robot_column,total_rows,total_columns

def GenerateData(data,robot_row,robot_column,robot_has_color,total_rows,total_columns):

    list = []
    for i in data:
        aux = []
        robot, action = str(i).split(':')
        r = str(robot).split(' ')
        a= str(action).split()
        n_robot = 0
        if (str(robot) == 'Robot1'):
            n_robot = 1
            print("N_ROBOT1:", n_robot)
        elif (str(robot) == 'Robot2'):
            n_robot = 2
            print("N_ROBOT2:", n_robot)
        elif (str(robot) == 'Robot3'):
            n_robot = 3
        elif (str(robot) == 'Robot4'):
            n_robot = 4
        elif (str(robot) == 'Robot5'):
            n_robot = 5
        if (str(action) == ' paint_up'):
            aux.append(1)
            aux.append(n_robot)
            row = robot_row[n_robot-1] -1
            column = robot_column[n_robot-1]
            aux.append(row)
            aux.append(column)
            aux.append(0)
            aux.append(0)
            aux.append(0)
        elif (str(action) == ' paint_down'):
            aux.append(2)
            aux.append(n_robot)
            row = robot_row[n_robot] + 1
            column = robot_column[n_robot]
            aux.append(row)
            aux.append(column)
            aux.append(0)
            aux.append(0)
            aux.append(0)
        elif (str(action) == ' up'):
            aux.append(3)
            aux.append(n_robot)
            row = robot_row[n_robot-1]
            column = robot_column[n_robot-1]
            aux.append(row)
            aux.append(column)
            robot_row[n_robot-1] = robot_row[n_robot-1] -1
            row = robot_row[n_robot-1]
            aux.append(row)
            aux.append(column)
        elif (str(action) == ' down'):
            aux.append(4)
            aux.append(n_robot)
            aux.append(robot_row[n_robot-1])
            aux.append(robot_column[n_robot-1])
            robot_row[n_robot-1] = robot_row[n_robot-1] + 1
            aux.append(robot_row[n_robot-1])
            aux.append(robot_column[n_robot-1])
        elif (str(action) == ' right'):
            aux.append(5)
            aux.append(n_robot)
            aux.append(robot_row[n_robot-1])
            aux.append(robot_column[n_robot-1])
            robot_column[n_robot-1] = robot_column[n_robot-1] +1
            aux.append(robot_row[n_robot-1])
            aux.append(robot_column[n_robot-1])
        elif (str(action) == ' left'):
            aux.append(6)
            aux.append(n_robot)
            aux.append(robot_row[n_robot-1])
            aux.append(robot_column[n_robot-1])
            robot_column[n_robot-1] =  robot_column[n_robot-1] -1
            aux.append(robot_row[n_robot-1])
            aux.append(robot_column[n_robot-1])
        elif (str(action) == ' change_paint_black'):
            aux.append(0)
            aux.append(n_robot)
            aux.append(2)
        elif (str(action) == ' change_paint_white'):
            aux.append(0)
            aux.append(n_robot)
            aux.append(1)
        print("Aux is:", aux)
        list.append(aux)

    return list


def DrawBoard(rows, columns,newOffsetY,total_rows,total_columns):
    
    #Square definition
    turtle.speed(0)
    turtle.pensize(3)
    turtle.penup()
    turtle.goto(offsetX,newOffsetY)
    turtle.pendown()
    turtle.color("black")
    turtle.forward(columns*80)
    turtle.right(90)
    turtle.forward(rows*80)
    turtle.right(90)
    turtle.forward(columns*80)
    turtle.right(90)
    turtle.forward(rows*80)
    turtle.right(90)
    
    #Lets fill the board using columns and rows
    turtle.penup()
    turtle.goto(offsetX, newOffsetY)
    actualx = offsetX
    actualy = newOffsetY
    for i in range(0,rows):
        turtle.goto(actualx,actualy)
        turtle.pendown()
        turtle.forward(columns*80)
        turtle.penup()
        actualy = actualy - 80
    turtle.penup()
    actualy = newOffsetY
    actualx = offsetX
    for j in range(0,columns):
        turtle.goto(actualx,actualy)
        turtle.pendown()
        turtle.right(90)
        turtle.forward(rows*80)
        turtle.left(90)
        turtle.penup()
        actualx = actualx + 80

def setInitialBoard(rows,columns,robot_row, robot_column,newOffsetY,total_rows,total_columns,how_many_robots):
    
    global offsetX
    global offsetY
    
    newOffsetY = offsetY + (total_rows*80)
    rob_x = []
    rob_y = []
    for i in range(0,how_many_robots):
        rob_x.append(0)
        rob_y.append(0)

    for i in range(0,how_many_robots):
        rob_x[i]=(robot_column[i]*80) + 40 + offsetX
        rob_y[i] = newOffsetY - 40 - (robot_row[i]*80)
    
    for i in range(0,rows):
        for j in range(0,columns):
           paintPosition(i,j,"grey",newOffsetY)
    turtle.penup()
    for i in range(0,how_many_robots):
        turtle.penup()
        turtle.goto(rob_x[i],rob_y[i])
        turtle.color(robot_has_color[i])
        turtle.stamp()



def WriteLine(robot,initial_row, initial_column, target_row, target_column,action,newOffsetY):
    
    global offsetX
    global offsetY
    
    x_initial = (initial_column*80) + 40 + offsetX
    y_initial = newOffsetY - 40 - ((initial_row)*80)
    x_target = (target_column*80) + 40 + offsetX
    y_target = newOffsetY - 40 - (target_row*80)
    n_robot = robot
    turtle.color(robot_path[n_robot-1])
    turtle.penup()
    turtle.goto(x_initial,y_initial)
    turtle.pendown()
    if (action == 0):
        turtle.left(90)
        turtle.forward(80)
        turtle.right(90)
    elif(action == 1):
        turtle.right(90)
        turtle.forward(80)
        turtle.left(90)
    elif(action == 2):
        turtle.forward(80)
    elif(action == 3):
        turtle.backward(80)

def paintPosition(row,column,color,newOffsetY):
    
    global offsetX
    global offsetY
    turtle.penup()
    
    x_position = (column*80) + offsetX +2
    y_position = newOffsetY - (row*80) +2 -80
    
    turtle.goto(x_position,y_position)
    turtle.color(color)
    turtle.begin_fill()
    for k in range(4):
        turtle.forward(76)
        turtle.left(90)
    turtle.end_fill()


def DrawEndPoint(robot,row,column,newOffsetY):
    
    global offsetY
    global offsetX
    turtle.penup()
    if (robot == 1):
        color = robot_path[0]
    elif(robot ==2):
        color = robot_path[1]
    elif(robot == 3):
        color = robot_path[2]
    elif(robot == 4):
        color = robot_path[3]
    elif (robot == 5):
        color = robot_path[4]
    x = (column*80) + 40 + offsetX
    y = newOffsetY - 40 - (row*80)
    turtle.goto(x,y)
    turtle.dot(10, color)


" Path information:"
" [Action, parameter1, parameter2, parameter3,parameter4,parameter5]"
" Action { 0 = Change-color, 1 = Paint-up, 2 = Paint-down, 3 = Up, 4 = Down, 5 = Right, 6 = Left, 7 = Stop }"
" Parameter1 -> Which robot { 1 = Robot1, 2 = Robot2]}"
" Change-Color = [0,robot,color,0,0,0] Color{0 = black, 1 = white}"
" Paint-up = [1,robot,row,column, 0,0,0]"
" Paint-down = [2,robot,row,column,0,0,0]"
" Up = [3,robot,row_initial,column_initial,row_final,column_final] "
" Down = [4,robot,row_initial,column_initial,row_final,column_final] "
" Right = [5,robot,row_initial,column_initial,row_final,column_final] "
" Left = [6,robot,row_initial,column_initial,row_final,column_final] "
" Stop = [7,robot,row,column,0,0,0] "

def AnalyzingData(list,newOffsetY):
    
    global robot_has_color
    
    for e in list:
        n_robot = 0
        if (e[1] == 1):
            n_robot = 1
        elif(e[1] ==2):
            n_robot = 2
        elif(e[1] == 3):
            n_robot = 3
        elif(e[1] == 4):
            n_robot = 4
        elif(e[1] == 5):
            n_robot = 5
        if (e[0] == 0):
            if(e[2] == 1):
                robot_has_color[n_robot-1] = "white"
            elif(e[2] ==2):
                robot_has_color[n_robot-1] = "black"
        elif(e[0] == 1):
            color = robot_has_color[n_robot-1]
            paintPosition(e[2],e[3],color,newOffsetY)
        elif(e[0] == 2):
            color = robot_has_color[n_robot-1]
            paintPosition(e[2],e[3],color,newOffsetY)
        elif(e[0] == 3):
            WriteLine(e[1],e[2],e[3],e[4],e[5],0,newOffsetY)
            DrawEndPoint(e[1],e[4],e[5],newOffsetY)
        elif(e[0] == 4):
            WriteLine(e[1],e[2],e[3],e[4],e[5],1,newOffsetY)
            DrawEndPoint(e[1],e[4],e[5],newOffsetY)
        elif(e[0] == 5):
            WriteLine(e[1],e[2],e[3],e[4],e[5],2,newOffsetY)
            DrawEndPoint(e[1],e[4],e[5],newOffsetY)
        elif(e[0] == 6):
            WriteLine(e[1],e[2],e[3],e[4],e[5],3,newOffsetY)
            DrawEndPoint(e[1],e[4],e[5],newOffsetY)
        elif(e[0] == 7):
            DrawEndPoint(e[1],e[2],e[3],newOffsetY)


def Draw(robots, state, path):

    print(path)
    turtle.setup(800,800)
    turtle.speed(0)
    how_many_robots = len(robots)
    robot_row,robot_column,total_rows, total_columns = setInitialData(robots,state,how_many_robots)
    newOffsetY = offsetY + (total_rows*80)
    DrawBoard(total_rows, total_columns,newOffsetY,total_rows,total_columns)
    setInitialBoard(total_rows,total_columns,robot_row,robot_column,newOffsetY,total_rows,total_columns,how_many_robots)
    data = GenerateData(path,robot_row, robot_column, robot_has_color,total_rows,total_columns)
    turtle.speed(0)
    AnalyzingData(data, newOffsetY)
    turtle.hideturtle()
    turtle.done()


