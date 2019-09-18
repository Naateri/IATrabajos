from tkinter import *
from copy import copy, deepcopy

colors = ["red", "black"]
"""
Tablero:
0 -> colors[0] (red)
1 -> colors[1] (black) (user)
2 -> empty
"""

class Node():
    def __init__(self,boardState,level,numPieces=None,posibleMoves=[],father=None):
        self.boardState = boardState
        self.posibleMoves = posibleMoves
        self.father = father
        self.level = level
        self.numPieces = numPieces

class MiniMax():
    level = 0
    def __init__(self,initialBoard,maxDeph):
        self.initialBoard = initialBoard
        self.maxDeph = maxDeph
        self.root = Node(self.initialBoard,self.level)
        turn = 0
        actualDeph = 0
        queue = []
        queue.append(self.root)
        while queue and actualDeph<self.maxDeph:
            n = queue.pop(0)
            n.posibleMoves = self.getPosibleMovesOfANode(n.boardState,turn,self.level,n)
            
            for i in range(0,len(n.posibleMoves)):
                queue.append(n.posibleMoves[i])
            
            if(turn == 0):
                turn = 1
            else:
                turn = 0
            print(len(n.posibleMoves))
            print(self.level)
            actualDeph = actualDeph + 1
            self.level = self.level +1 
        
        while queue:
            n = queue.pop(0)
            n.posibleMoves = self.getPosibleMovesOfANode(n.boardState,turn,self.level,n)
            if(turn == 0):
                turn = 1
            else:
                turn = 0
            
            print(len(n.posibleMoves))
            print(self.level)
        stack = []
        stack.append(self.root)
        for i in range(0,len(self.root.posibleMoves)):
            stack.append(self.root.posibleMoves[i])
        
        while stack:
            n = stack.pop()
            
            for i in range(0,len(n.posibleMoves)):
                stack.append(n.posibleMoves[i])
            
            if(n.level%2 == 0):
                turn = 1
            else:
                turn = 0
            print(len(n.posibleMoves))
            print(self.level)
    
    def printTree(self):
        actualDeph = 0
        queue = []
        queue.append(self.root)
        while queue and actualDeph<self.maxDeph:
            n = queue.pop(0)
            print(n.level)
            for i in range(0,len(n.posibleMoves)):
                queue.append(n.posibleMoves[i])
            actualDeph = actualDeph + 1
        

            
        
    
    def getPosibleMovesOfANode(self,board,piece,level,father=None):
        moves = []
        tempBoard = board
        for i in range(0,8):
            for j in range(0,8):
                if(tempBoard[i][j]==piece):
                    if(i+1<8 and j+1<8 and board[i+1][j+1]!=piece):
                        board[i][j]=2
                        board[i+1][j+1] = piece
                        moves.append(Node(board,level,[],father))
                        board = tempBoard
                    if(i-1>=0 and j+1<8 and board[i-1][j+1]!=piece):
                        board[i][j]=2
                        board[i-1][j+1] = piece
                        moves.append(Node(board,level,[],father))
                        board = tempBoard
                    if(i+1<8 and j-1>=0 and board[i+1][j-1]!=piece):
                        board[i][j]=2
                        board[i+1][j-1] = piece
                        moves.append(Node(board,level,[],father))
                        board = tempBoard
                    if(i-1>=0 and j-1>=0 and board[i-1][j-1]!=piece):
                        board[i][j]=2
                        board[i-1][j-1] = piece
                        moves.append(Node(board,level,[],father))
                        board = tempBoard
        return moves


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
            if (self.tablero[row][col] == 1):
                print("Not a valid position, try again!")
                self.click_positions = [ 0,0 ] #pos_1, pos_2
                self.cur_play = False
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
            temp = deepcopy(self.tablero)
            print(temp)
            mm = MiniMax(self.tablero,3)
            print(temp)
            self.tablero = temp
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
