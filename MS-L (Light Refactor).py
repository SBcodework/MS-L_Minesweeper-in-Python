# Author: Samuel J. Brown, Fall 2018
# Author worked as a one-person team and wrote all code within here.
# 'OCRB' is the font for the mine and time counters.

# Code here is mostly untounced, but lightly refactored for readability. Comments also made to indicate future changes and review the code. They begin with "###". <- 6/27/2019

global setbool  ,  startbool  ,  previn         ,  superlist  ,  minelist           ### Avoid use of globals. Be more descriptive with names (Like EntryFieldLegth instead of _XE)
global prevflag ,  squarelist ,  squareselflist ,  _minecount ,  _XE     , _YE

import tkinter as tk
import random
import sys

startbool = 0           ### Consider making the main-loop / application a class, where these values are constructed in the class's constructor.
setbool   = None                        
previn    = []
prevflag  = []
squareselflist = []
squarelist     = []
_minecount     = 40
_XE = 16
_YE = 16

### This may be a relatively short program, but consider adding proper documentation practices in the future, so that a co-worker can know what "brel(...)" is instead of scrolling down hundreds of lines, for example.
### Consider using a style guide like PEP 8.

#Generate the map. The inlist contains what coordinates are to be shown. The flaglist is like the inlist, and denotes flags.
#The maplist is the whole grid. Superlist contains all three.
def genmap(mapx, mapy, mines, startx, starty):
    maplist = [['[i]' for n in range(mapx)] for i in range(mapy)]
    for i in range(3):                 #No mines in start square
        for n in range(3):
            if starty - 1 + i != -1 and startx - 1 + n !=-1:                ### Make "starty - 1 + 1" and things like it a variable in favor of the DRY principle. 
                try:                                                        ### This try-except block is an indirect mechanism for situations when the loop tries to access an invalid out-of-range element in
                    maplist[ (starty-1)+i ][ (startx-1)+n ] = '[c]'         ### a 2-D array. Instead, use conditional expressions to dictate if the element is within range. Even better, write and use a helper function.
                except IndexError:
                    pass
                
    canlist  = [[n,i] for n in range(mapx) for i in range(mapy) if maplist[i][n] != '[c]']       #Gathering all valid potential mine spots. 
    minelist = random.sample( range(0, len(canlist)), mines )                                    #RNG for mines.
    for i in range(len(minelist)):                              #Minelist now has positions of mines.
        minelist[i] = canlist[ minelist[i] ] 
    for i in range(len(minelist)):                              #Add in those mines to the maplist.
        maplist[ minelist[i][1] ][ minelist[i][0] ]='[X]'
    for i in range(mapy):                                       #Add in numbers.
        for n in range(mapx):
            if maplist[i][n] != '[X]':
                maplist[i][n] = ' ' + str( getnearnum(n, i, maplist) ) + ' '                         ### It would much more space (and time) efficent to store each square's state as a single character instead of the string the 
                                                                                                     ### GUI chooses to display.
    flaglist = []           ### Consider not returning empty lists in this function.                                                                                  
    inlist   = []
    reveal(startx, starty, maplist, inlist, flaglist)
    return [maplist,inlist,flaglist], minelist

#Single click, middle click, and flag.
def reveal(x ,y ,maplist, inlist, flaglist, mode = 0):            ### Consider splitting this function up into helper functions like singleClick(), flag(), etc, and make this function choose between them.
    #Root mode, which is called when the x,y state is a 0.
    if mode == 0:                                                 ### Minor tip, consider using "!mode" instead. 
        for i in range(3):
            for n in range(3):
                loopy = y-1+i                                     ### We see this loop structure again from genmap(...). Consider making it into a function in favor of the DRY principle. 
                loopx = x-1+n
                if loopy != -1 and loopx != -1:
                    try:
                        #This is how nearby squares are gathered without being out-of-bounds.
                        state = maplist[loopy][loopx]
                    except IndexError:
                        continue
                    
                    if [loopx,loopy] not in inlist:
                        if state == ' 0 ':
                            zeroapinl(loopx, loopy, inlist, flaglist)              ### Consider using more descriptive function names. I want to have a decent idea of what functions do based off of their name.
                            try:
                                #Recursion is how a 'chain-reaction' of zeroes is handled.
                                reveal(loopx, loopy, maplist, inlist, flaglist)
                            except RecursionError:
                                sys.setrecursionlimit(70000) #For very large grids and few mines            ### I understand that, in the light of saftey, increasing the recursion depth then decreasing may be good practice.
                                reveal(loopx, loopy, maplist, inlist, flaglist)                             ### But doing it for each square with a state of ' 0 ' can be very long. The worst case of this is around O( X*Y ),
                                sys.setrecursionlimit(1000)                                                 ### in a grid full of 0's. Consider raising it and lowering it outside of the loop.
                        else:
                            zeroapinl(loopx, loopy, inlist, flaglist)
    #Single-click. Also, 0 is returned when a mine is encountered. This eventually looses the game.
    elif mode == 1:                                             ### It is hard to infer whenever "mode == 1" means flag-mode, single-click-mode, etc. Consider using a character or short string, like 'f' or 's'.
        state = maplist[y][x]
        if [x,y] not in inlist and [x,y] not in flaglist:
            if state== '[X]':
                return 0
            elif state== ' 0 ':
                #If a 0 is found, call root mode.
                reveal(x, y, maplist, inlist, flaglist)
            else:
                inlist.append([x,y])                            ### Consider researching a more efficent data structure on data management. 
    #Flag.
    elif mode == 3:
        if [x,y] not in inlist:
            if [x,y] not in flaglist:
                flaglist.append([x,y])
            elif [x,y] in flaglist:
                flaglist.remove([x,y])
    #Middle-click.
    elif mode == 2:
        state = maplist[y][x]
        if [x,y] in inlist and state != ' 0 ':
            flags   = 0
            states  = []
            poslist = []
            for i in range(3):
                for n in range(3):
                    loopy=y-1+i
                    loopx=x-1+n
                    if loopy!=-1 and loopx!=-1:
                        try:
                            state=maplist[loopy][loopx]
                        except IndexError:
                            continue
                        if [loopx,loopy] not in inlist:
                            if [loopx,loopy] not in flaglist:
                                states.append(state)
                                poslist.append([loopx,loopy])
                            else:
                                flags += 1
            state = maplist[y][x]
            if flags == int(state):
                if '[X]' in states:
                    return 0
                else:
                    for items in poslist:
                        #If a 0 is found, call root mode.
                        reveal(items[0], items[1], maplist, inlist, flaglist,1)
                        
#Flags a random mine. Mines near a number are considered first.
def hint(superlist, minelist):
    flaglist = superlist[2]
    inlist   = superlist[1]
    maplist  = superlist[0]
    cont     = 1
    #Hint requires that all flags must a mine under them. Else, it returns 0, eventually ending the game.
    for flag in flaglist:
        if flag not in minelist:
            cont = 0
            return 0
            break
    if cont == 1:           ### Consider using "if cont:" instead.
        #This code block handles finding mines that 'touch' a number. Hintlist contains positions of canditates. 
        hintlist = []
        for spot in inlist:
            x = spot[0]
            y = spot[1]
            if maplist[y][x] != ' 0 ':
                for i in range(3):
                    for n in range(3):
                        loopy = y-1+i
                        loopx = x-1+n
                        if loopy != -1 and loopx != -1:
                            try:
                                state = maplist[loopy][loopx]
                            except IndexError:
                                continue
                            if state == '[X]' and [loopx,loopy] not in flaglist:
                                hintlist.append([loopx,loopy])
                                
        #If there are items in the hintlist, choose a random one, then reveal it.
        if len(hintlist) != 0:
            hintpos = hintlist[ random.randint(0, len(hintlist)) -1 ]                         ### Consider this: make the statement within brackets varables, and calculate before you use them in the same line. This could make
            reveal(hintpos[0], hintpos[1], superlist[0], superlist[1], superlist[2], 3)      ### it easier when running a degugger.
            
        #If there are no items in hintlist, either all mines left do not touch a number or there is nothing to be hinted.
        if hintlist == []:
            #Gathering un-flagged mines and adding them to hintlist.
            hintlist = [mine for mine in minelist if mine not in flaglist]
            #If there is something to be hinted, run the code block.
            if hintlist != []:
                hintpos = hintlist[ random.randint(0, len(hintlist))-1 ]
                reveal(hintpos[0], hintpos[1], superlist[0], superlist[1], superlist[2], 3)
                
#Helper function to root mode. If root mode is called, and a flag is under an area to be cleared, it removes the flag and moves on as usual.
def zeroapinl(x, y ,inlist, flaglist):
    if [x,y] in flaglist:
        flaglist.remove([x,y])
    inlist.append([x,y])
    
def indexconvert(mapx, mapy, index, xl, yl):
    return [index % xl, index // yl]

def getnearnum(x, y, maplist):
    number = 0
    for i in range(3):
        for n in range(3):
            if y-1+i!=-1 and x-1+n!=-1:                 ### Consider putting spaces between your expressions' members for readability. I've been doing it in this refactor, but sometimes I leave things is to show you the difference.
                try:
                    if maplist[ (y-1)+i ][ (x-1)+n ] == '[X]':
                        number += 1
                except:
                    pass
    return number

def checkwin(superlist, mines):
    condition = 0
    #Line below checks if number of flags are equal to number of mines, and if the number of empty spaces equal the actual number of empty spaces.
    if len(superlist[2]) == mines and len(superlist[1]) == len(superlist[0]) * len(superlist[0][0]) - mines:
        condition = 1
        for item in superlist[2]:
            if superlist[0][ item[1] ][ item[0] ] != '[X]':
                condition = 0
    if condition == 1 :
        return 1
    
#---tKinter, GUI.
#Some functions are ordered the way they are in order to work.
#'OCRB' is the font for the mine and time counters. 

#This is how the toolbar shows and hides the settings. It uses global setbool to remember if it is shown.
def _ShowSettings():
    global setbool
    if setbool == None or setbool == 0:
        Settings.configure(relief = 'sunken')
        setbool = 1
        Setbar.grid(row = 2, column = 0)
    else:
        Settings.configure(relief = 'raised')
        setbool = 0
        Setbar.grid_remove()
        
def _Newgame():
    global Minegrid , squarelist, squareselflist
    global superlist, startbool , _minecount    ,   setbool
    #Below line is to make things look more presentable.
    Toolbar.grid_remove()
    superlist = []
    minelist  = []
    startbool = 0
    #Hides the 'pop-in' of the toolbar for presentation. It does re-appear.
    if setbool == 0:
        _ShowSettings()
    #Destroys all buttons.              
    for i in range(len(squarelist)):                    ### Optimization note: there probally is a more efficent way of managing a grid of buttons than using a list full of objects representing squares.
        squarelist[i][1].destroy()
    for i in squareselflist:                            ### Consider making this small deletion routine a function in the class Square.
        del i
        
    #Destroys the minegrid. 
    Minegrid.ref.destroy()                              ### See above.
    del Minegrid
    
    squareselflist = []
    squarelist     = []
    #setbool=None - For possible future ref
    #Make a new minegrid, which makes squares.
    Minegrid = MainGrid(App)
    Minegrid.ref.grid(row = 3, column = 0)
    Toolbar.grid()
    if setbool == 1:
        _ShowSettings()
    MineCounter['text'] = _getME()
    Newgame['text'] = ':)'
    _XE        = _getXE()
    _YE        = _getYE()
    _minecount = _getME()
    #Reset the time counter's text.
    TimeCounter['text'] = 0                         ### Consider making a timer class.
    #If the time counter is still running, end it.
    try:
        TimeCounter.after_cancel(timeid)
    except NameError:
        pass
#Handler function from the hint button to the hint function.
def _Hint():
    global superlist, minelist,_minecount
    try:
        hint(superlist, minelist)
        MineCounter['text'] = _minecount - len(superlist[2])
        Disp()
    #Below handles pressing hint when there is no map to hint on.
    except NameError:
        pass
    except IndexError:
        pass
#Below functions are how values are taken from the text fields of the settings. They also handle default values, which are chosen on invalid entries.
def _getXE():
    string = XEntry.get()
    return _gethelper(string, 16)

def _getYE():
    string = YEntry.get()
    return _gethelper(string, 16)

def _getME():
    string = MEntry.get()
    return _gethelper(string, 40)

def _gethelper(string, default): 
    error = 0
    try:
        string = int(string)
    except ValueError:
        error = 1
        return default
    if error == 0:
        if string >= 0:
            return string
    else:
        return default
#Helper function to run the timer. Timeid is needed to recognize what the timer is when needing to stop it.
    
def _tcin():
    global timeid
    TimeCounter['text'] += 1
    timeid = TimeCounter.after(1000, _tcin)                          ### Consider using a different model for keeping track of time, namely, consider not using recursion to keep track of time.

#Function to start the timer. Remember to make timeid global or a newgame won't reset it.
def _timecounterstart():
    global timeid
    TimeCounter['text'] = 000
    timeid = TimeCounter.after(1000, _tcin)
    
App = tk.Tk()                                               ### Search up on 'if __name__ == "__main__"'. It provides a standardized way of running your application.
App.title("MS-L")
#Toolbar - Houses Settings, Newgame, and Hint widgets. 
Toolbar = tk.Frame(App, relief = 'groove')                  ### Consider using variables to configure settings, like which relief the toolbar should be.
Toolbar.grid(row = 0, column = 0)      

Settings = tk.Button(Toolbar, command = _ShowSettings, height = 2, width = 4, bg = 'cyan', text = '...')    
Settings.grid(row = 0, column = 1)                                                                
Setbar = tk.Frame(App, relief = 'groove')
XEntry = tk.Entry(Setbar, width = 4)
XEntry.grid(row = 0,column = 0)
YEntry = tk.Entry(Setbar, width = 4)
YEntry.grid(row = 0, column = 1)
MEntry = tk.Entry(Setbar, width = 4)
MEntry.grid(row = 0, column = 2)

Newgame = tk.Button(Toolbar, command = _Newgame, height = 2, width = 4, bg = 'yellow', text = ':)')
Newgame.grid(row = 0, column = 2)

Hint = tk.Button(Toolbar, command = _Hint, height = 2, width = 4, bg = 'orange', text = '{?}')
Hint.grid(row = 0, column = 3)

TimeCounter = tk.Label(Toolbar, relief = 'groove', bg = 'red', fg = 'black', text = 000, height = 2, width = 4, font = ('OCRB', '11', 'bold'))   ### Consider this: text = 000. Should 000 be a string? It'll likely evaluate to 0.
TimeCounter.grid(row = 0, column = 4)

#The minegrid. When initialized, it makes squares as needed.
class MainGrid():                                           ### Consider enscapulating "superlist" and other data members into this class, making things more organized and towards OOP more.
    def __init__(self, master):
        self.ref = tk.Frame(master, relief = 'groove')
        self.gridnewgame()
        
    def gridnewgame(self):
        global _XE,_YE
        x   = _getXE()
        _XE = x
        y   = _getYE()
        _YE = y
        for i in range(y):
            for n in range(x):
                #(n,i) is used for the self.input trick.
                button = Square(self.ref, (n,i))
                button.ref.grid(column = n, row = i)
        #Used by brel to store the position of what button was clicked last.
        self.input = None
        
#Short for button release. 
def brel(action):
    #Set the self.input of the minegrid to its position and its button-click-number, represented by action.num.
    Minegrid.input = (action.widget.Mpos, action.num)
    action.widget.config(relief = 'sunken')
    global startbool
    global superlist
    global minelist
    global timeid
    global _minecount
    #If the game has already started.
    if startbool == 1:
        if reveal(*action.widget.Mpos, *superlist, mode = action.num) == 0:                 ### Consider using list/tuple unpacking more for readability in other places.
            TimeCounter.after_cancel(timeid)
            _gamelost()                                                                     
        MineCounter['text'] = _minecount - len(superlist[2])
        Disp()
    #If the game just started.
    if startbool == 0:
        startbool = 1
        global pastin
        #Resets pastin, which is used in Disp().
        pastin = []
        #Generate the map.
        superlist, minelist = genmap(_XE, _YE, _minecount, *Minegrid.input[0])
        #Used to be mode=0, is removed
        Disp()
        _timecounterstart()
    if action.num == 1:
        action.widget['relief'] = 'sunken'
    if action.num == 2 and list(action.widget.Mpos) not in superlist[1]:
        action.widget['relief'] = 'raised'
        
def _gamelost():
    Newgame['text'] = ':X'
    global squareselflist
    global superlist
    for i in squareselflist:
        i.ref['bg'] = 'magenta'
        i.ref.config(relief = 'sunken', text = superlist[0][ i.ref.Mpos[1] ][ i.ref.Mpos[0] ])
        #Game can't be played if the game is lost. Winning the game is handled in Disp().
        i.ref['command'] = None
        i.ref.unbind('<ButtonRelease>')
    
def Disp():
    #Also, finds changes between inlist and flaglist and updates the GUI accordingly. pastin is used for that purpose.
    global pastin
    global superlist
    global squarelist,_minecount
    inlist   = superlist[1]
    flaglist = superlist[2]
    #Handle the inlist.
    for i in range(len(inlist)):
        if inlist[i] not in pastin:
            for n in range(len(squarelist)):
                #The tuple conversion is because a previous version relied on lists for position tracking. Ideally, they should be tuples, but the conversion is probally too long for now.
                if tuple(inlist[i]) in squarelist[n]:
                    squarelist[n][1].configure(relief = 'sunken', text = superlist[0][ inlist[i][1] ][ inlist[i][0] ])
                    pastin.append([ inlist[i][0], inlist[i][1] ])
    #Handle the flaglist.
    for i in range(len(flaglist)):
        for n in range(len(squarelist)):
            if tuple(flaglist[i]) in squarelist[n]:
                squarelist[n][1].configure(text = 'F', bg = 'red', relief = 'sunken')
    for n in range(len(squarelist)):
        if squarelist[n][1]['bg'] == 'red' and list(squarelist[n][0]) not in flaglist:
            squarelist[n][1].configure(text = '', bg = 'SystemButtonFace', relief = 'raised')
    #Handles game winning.
    if checkwin(superlist, _minecount) == 1:
        TimeCounter.after_cancel(timeid)
        Newgame['text'] = ':D'
        Newgame.flash()
        for i in squareselflist:
            i.ref['bg']      = 'green'
            i.ref['command'] = None
            i.ref.unbind('<ButtonRelease>')

class Square():
    def __init__(self, master, position):
        global squarelist
        global squareselflist
        self.ref = tk.Button(master, height = 1, width = 2, command = self.handler)
        self.ref.bind('<ButtonRelease>', brel)
        self.ref.Mpos = position
        #The squarelist contains the references for the button part of square(). I could've made a widget another more common way, but tkinter was hard for me to learn.
        squarelist.append((position, self.ref))
        squareselflist.append(self)
    def handler(self):
        self.ref['relief'] = 'sunken'   #Fixes an elusive bug where pressing the button wouldn't change relief

Minegrid = MainGrid(App)
Minegrid.ref.grid(row = 3, column = 0)
MineCounter = tk.Label(Toolbar, relief = 'groove', bg = 'red', fg = 'black', text = _minecount, height = 2, width = 4, font = ('OCRB', '11', 'bold'))           
MineCounter.grid(row = 0, column = 0)
App.mainloop()

"""    
BSD 2-Clause License

Copyright (c) 2019, SBcodework
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
"""
