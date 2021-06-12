import os
import numpy as np
#For Terminal
bright_map = [" ", ",", "-", "~", ":", ";", "=", "!", "*", "#", "$", "@"]

def element_map(f, Grid):
    '''
    input
    -----
    f: Function
    the function to map the 2D array element by element
    Grid: 2D numpy array
    
    output
    ------
    Grid: 2D array
    '''
    x_size = len(Grid[0])
    y_size = len(Grid)
    new_Grid = [[f(Grid[y][x]) for x in range(x_size)] for y in range(y_size)]
    return new_Grid


def GetChar(num):
    '''
    Input
    -----
    num: float
        0<num<1
    
    OutPut
    ------
    char 
    the character linked to the number
    
    #Based on lighting 
    '''
    if num>1 or num<0:
        print("Error, " + str(num))
    return bright_map[np.int(num*len(bright_map)-0.0001)] #-0.0001 in case norm is directly inline with light
    

    
  




def GetCharDist(num):
    '''
    Input
    -----
    num: float
    
    OutPut
    ------
    char 
    the character linked to the number
    
    based on distance
    '''
    num = min(len(bright_map)-1, num) #-0.5 as have 12 elements from 0 to 11 # Could do -1
    return bright_map[-(np.int(num)+1)]


    

def TextDisplay(Grid):
    Grid = element_map(GetChar,Grid)
    CLR = "\x1B[0K"
    sdisplay = ""#"\x1B["+str(res_y + 10)+"A" #only 9 new line characters allowed in extrastring
    for a in range(len(Grid)):
        for b in range(len(Grid[0])):
            sdisplay += Grid[a][b]
        sdisplay += CLR + "\n"
		
       
    
    #os.system('cls' if os.name == 'nt' else 'clear') #This method causes flickering
    print(sdisplay)
    #time.sleep(1/fps)
