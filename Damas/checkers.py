from tkinter import *

colors = ["red", "black"]
turno = True

"""
Tablero:
0 -> colors[0] (red)
1 -> colors[1] (black) (user)
2 -> empty
"""

class PosibleMove:
    def __init__(self,state,numBlack,numRed,originX=None,originY=None,destX=None,destY=None):
        self.state = state
        self.numBlack = numBlack
        self.numRed = numRed
        self.originX = originX
        self.originY = originY
        self.destX = destX
        self.destY = destY

class Level():
    def __init__(self,level,states):
        self.level = level
        self.states = states

    

class MiniMax:
    levels = []
    actualLevel = 0
    def __init__(self,initialState,maxLevel):
        posibleMove = PosibleMove(initialState,self.countPieces(1,initialState),self.countPieces(0,initialState))
        states = [posibleMove]
        level = Level(self.actualLevel,states)
        self.levels.append(level)
        self.actualLevel = self.actualLevel+1
        self.maxLevel = maxLevel

    
        



    def countPieces(self,piece,board):
        count = 0
        for i in range(0,8):
            for j in range(0,8):
                if(board[i][j]==piece):
                    count = count + 1
    
    




class Ficha():
    def __init__(self, color):
        self.color = colors[color]

    def draw(self, x,y):
        fcha = Canvas(bg = self.color, height = 15, width = 15)
        fcha.grid(row = x, column = y)


class Tablero():
    def __init__(self):
        self.tablero = [ [2 for i in range(8)] for j in range(8) ]

        for i in range(0,8,2):
            #red thingies
            self.tablero[0][i+1] = 0
            self.tablero[1][i] = 0
            self.tablero[2][i+1] = 0
            
            #black thingies
            self.tablero[5][i] = 1
            self.tablero[6][i+1] = 1
            self.tablero[7][i] = 1

        self.cur_play = False
        self.click_positions = [ 0,0 ] #pos_1, pos_2

    def play(self, row, col):
        if not self.cur_play:
            if (self.tablero[row][col] != 1):
                print(str(row) + " row, col: " + str(col))
                print(str(self.tablero[row][col]))
                print("Not your piece!")
                return

            self.click_positions[0] = row
            self.click_positions[1] = col
            self.cur_play = True
        else:
            if (self.tablero[row][col] == 1 and turno):
                print("Not a valid position, try again!")
                self.click_positions = [ 0,0 ] #pos_1, pos_2
                self.cur_play = False
                turno = False
                return
            else:
                turno = True
                return
            
            row_1 = self.click_positions[0]
            col_1 = self.click_positions[1]

            st_1 = row_1 == row+1 and col_1 == col+1
            st_2 = row_1 == row-1 and col_1 == col+1
            st_3 = row_1 == row-1 and col_1 == col-1
            st_4 = row_1 == row+1 and col_1 == col-1

            final_st = st_1 or st_2 or st_3 or st_4

            if not final_st:
                print("Not a valid move. Try again.")
                self.click_positions = [ 0,0 ] #pos_1, pos_2
                self.cur_play = False
                return

            self.tablero[row_1][col_1] = 2
            self.tablero[row][col] = 1

            self.click_positions = [ [0,0], [0,0] ] #pos_1, pos_2
            self.cur_play = False
            print("Piece moved.")
            self.draw()

    def draw_fichas(self):
        for fila in range(8):
            for columna in range(8):
                if (self.tablero[fila][columna] != 2):
                    temp_ficha = Ficha(self.tablero[fila][columna])
                    temp_ficha.draw(fila, columna)

    def draw(self):
        for fila in range(8):
            for columna in range(8):
                bt = Button()
                if fila % 2 == 0:
                    if columna % 2 == 0:
                        bt = Button(bg = "white")
                    else:
                        bt = Button(bg = "brown")
                else:   
                    if columna % 2 == 0:
                        bt = Button(bg = "brown")
                    else:
                        bt = Button(bg = "white")
                bt.configure(activebackground = "gray", height = 3,
                     width = 3, command = lambda x = fila, y = columna:
                             self.play(x, y))
                bt.grid(row = fila, column = columna)

        self.draw_fichas()
                        

root = Tk()

tablero = Tablero()
tablero.draw()

difficulty = Scale(from_=1, to=10,orient=HORIZONTAL)
difficulty.set(1)
difficulty.grid(row=9,column=0,columnspan=8)

root.mainloop()
