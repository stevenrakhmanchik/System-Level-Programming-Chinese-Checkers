# Final Project : Chinese Checkers
**Created by Steven Rakhmanchik, Derrick Lin, and Rayyan Bhuiyan**

**Period 4**

**Scroll down for proposal information**

## Broad Description of Project
Our group has recreated the classic board game chinese checkers designed to be played in your terminal. Our game supports 2,3,4, or 6 players total and only Mac or Linux. Who needs mouses, our game is completely controlled by your keyboard and be sure to have your audio on as we have sound effects to add to the experience.

## Required libraries  
* Ncurse
  * To install : sudo apt-get install libncurses5-dev libncursesw5-dev
* ON MAC: afplay
  * To install : brew install afplay
* ON LINUX: aplay
  * To install : sudo apt-get install alsa-utils

## How to play

0.  **Know how to play chinese checkers first:** If you dont know how to play watch this really short and simple video [click here please](https://www.youtube.com/watch?v=kVEAfbecmo0&ab_channel=TripleSGames).
1.  **Game Setup:** Please make your terminal full screen. After, please compile and run our program. make -> make run.
2. **Go for it!:** We designed our game to be very user friendly and intuitive, please use your arrow keys and the enter button to navigate through the game. You may now go play if you don't want to read further instruction, you got this, trust yourself. See 4. for further instruction if needed.
3. **q to quit!:** When you are ready, press q to be brought to a menu to quit the program.

4. **Detailed Instructions on how to navigate the game:** Feeling unconfident? No  worries, we got you with further instruction.

  ### Use the up and down arrow to hover your choices. Press enter to select. ###
  * Select to play (umm unless you really don't want to, then press quit)
  * Choose a number of players (2,3,4,6).
  * Name your players! After typing a satisfactory name, please press enter and a color to represent yourself. You will then be prompted to repeat the process until all of your players are named; don't choose the same names or same colors.
  * Game time! Using the arrow keys, navigate the board. The current "hand" is on the highlightest spot, select the piece of your color to move and you will then see possible locations to move your piece to. Make your decision, highlight the spot you want to move your piece to, and then press enter again.
  * Fight for your life. After the first player goes, the next player will then go. Duke it out with them (or yourself if you are playing alone) and enjoy the sound effects (implemented using fork() processes which are controlled with signals).

## Technical Features of Our Programming
1. Data structure to simulate hexagonal checker board grid
2. Implenting gui using Ncurses
3. User Interface program to control elements such as position selector
4. Sound effects (really cool ones)
5. Main program to dictate game logic and perform computations ex moving pieces

## Concepts Used
1. Allocating Memory
2. Processes
3. Signals
4. Pipes
5. Sockets (kind of explained later)

## Bugs
* When you press q while in the middle of the game to exit the game, the terminal gets messed up after the quitting game noise plays. 

## Websites Used
- https://www.redblobgames.com/grids/hexagons/
- https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/
- https://w3.cs.jmu.edu/kirkpams/OpenCSF/Books/csf/html/index.html
- https://www.stuycs.org/systems-dw/notes/

## HOW WE TRIED TO IMPLEMENT MULTIPLAYER SOCKETS WITH NETCAT:

### On the Server Side:
 Created two pipes by running:
  mkfifo pipe1 pipe2
 
 Started netcat server in the background, and connected it's stdin/stdout to the pipes:
  netcat -l -p 5555 > pipe1 < pipe2 &

 Then started the game and connected it's stdout/stdin to the pipes:
  ./play < pipe1 > pipe2

This pretty much starts the server side of the process, so to recap:
 
* 'netcat' is started up in the background listening for the TCP connection on Port 5555 (that's what argumnets -l -p 5555 are doing), and it's stdin/stdout are connected to pipes: pipe1 and pipe2
 
* game process is also started and it's stdout/stdin are also connected to pipe1 and pipe2

### On the Client Side:
 
 From a different terminal, or it can be done from a adifferent machine, We run command:
  stty -echo; netcat <b>Local or Remote IP Here</b> 5555; stty echo;


This starts netcat in a client mode and connects to TCP/IP provided with port 5555

<b>We didnt get to implement this but this is how we were going to</b>

</br></br>
# Proposal

**Below contains our proposal. Note that some features have been omitted (mainly the network connection feature) and others have been added (sound) from our original plans.**

## Technical Design:
### Description of Project

We are going to be developing a Chinese checkers game that will be able to function over a network connection with up to 6 players at once. For this, we will use NCurses to make the game function.

### Breakdown of the Project (plan to overall work collaboratively on everything)
* Steven : hexagonal grid data structure, GUI, ui program and game logic, server stuff that didnt really work out completely
* Derrick : GUI and authenticity of game experience, artistic design
* Rayyan : artistic design, implementing user input, and GUI

We will divide our program into the following:

* Server Portion:
  * Will keep track of who's turn it is and will take in keystrokes from that player to make a move and change the state of the game for all other players.
 Will consist of the following:
  *  Server file that decides who's turn it is and allows that client to submit keystrokes to server.

* Client Portion:
  * Will essentially display the same thing for all players.
  * Will record keystrokes for player who's turn it is and send it to the server program when asked to by the server program.
  * Will consist of the following:

    1. gui program to display board (using NCURSES)
    2. main program to do game computations and contain game logic
    3. ui program to control dynamic gui elements such as moving selector and displaying possible paths.
    4. program to contain data structure that will be used by the main program to simulate an actual Chinese checkers board.


### Data Structures to be used and how
* We will make a hexagonal grid data structure to simulate a real chinese checkers board.

### Systems Curriculum Techniques we are Potentially Planning to Use:

- Allocating Memory
- Processes
- Signals
- Shared Memory
- Pipes
- Sockets (if we are able to create gameplay over network)

### Goals
At the very least we plan to implement a multiplayer version of the game that can be played on a single computer screen with different players taking turns.

- Client portion should be done in a few days after we figure out the mechanics, write our pseudocode and then code it in C.

- The online version of the game is expected to take many days to implement after we figure out the client side of the code.

- We plan on creating what will essentially be iyr own data structure that will simulate the checkerboard. (This will be one of the client side .c files)

### Potential Challenges

- Getting multi-client multiplayer to work
- Making the gui not get ruined if the user tries to resize the terminal window.
- Encrypting the socket connection (so the hackers don't steal our top secret chinese checkers data ;))
