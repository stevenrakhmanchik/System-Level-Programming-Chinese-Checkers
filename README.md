Final Project Proposal
Multiplayer Chinese Checkers 
Steven – Derrick – Rayyan
Period 4

––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

We are going to be developing a Chinese checkers game that will be able to function over a network connection with up to 6 players at once. For this, I will use NCurses to make the game function inside of the mac and linux terminal.

––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

We will divide our program into the following:

	- Server Portion:
		-> Will keep track of who's turn it is and will take in keystrokes from that player to make a move and change the state of the game for all other players.
		-> Will consist of the following:
			=> server file that decides who's turn it is and allows that client to submit keystrokes to server.


	- Client Portion:
		-> Will essentially display the same thing for all players
		-> Will record keystrokes for player who's turn it is and send it to the server program when asked to by the server program.
		-> Will consist of the following:
			=> gui program to display board (using NCURSES)
			=> main program to do game computations and contain game logic
			=> ui program to control dynamic gui elements such as moving selector and displaying possible paths.
			=> program to contain data structure that will be used by the main program to simulate an actual Chinese checkers board.

––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

Systems Curriculum Techniques we are Potentially Planning to Use:

	- Allocating Memory
	- Processes
	- Signals
	- Shared Memory
	- Semaphores
	- Pipes
	- Sockets (if we are able to create gameplay over network)

––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

At the very least we plan to implement a multiplayer version of the game that can be played on a single computer screen with different players taking turns. 

	- Client portion should be done in a few days after I figure out the mechanics, write my pseudocode and then code it in C.

	- The online version of the game is expected to take a week to implement after I figure out the client side of the code.

	- I am planning on creating what will essentially be my own data structure that will simulate the checkerboard. (This will be one of the client side .c files)
  
––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

Potential Challenges:
	
	- Getting multi-client multiplayer to work
	- Making the gui not get ruined if the user tries to resize the terminal window.
	- Encrypting the socket connection (so the hackers don't steal our top secret chinese checkers data ;))
	
	
––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

Online Reading Sources Used:

https://www.redblobgames.com/grids/hexagons/
