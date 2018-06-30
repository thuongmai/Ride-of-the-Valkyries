#RIDE OF THE VALKYRIES  
###The work presented in this paper is new and was prepared only for the course COMP 3501 Foundations of Game Programming and Computer Graphics, Fall 2016, School of Computer Science, Carleton University.

##Programmers:
* Adnan Ahmed
* Aurius Medalla
* Thuong Mai

##Setup:  
INITIALLY INCLUDED: main.cpp, Game.h, Game.cpp, CMakeFileLists.txt    
 - build with CMake  
SETUP ORGE3D in Visual Studio.  
IMPORTANT: ONLY PUSH HEADERS, SOURCE FILES, AND RESOURCES. NOT THE VISUAL STUDIO STUFF.

![Game Image](https://bitbucket.org/thuongmai92/ride-of-the-valkyries/raw/master/Images/image2.png)

**1. Content/Background**  
Ride of The Valkyries is a 3D-modeled game in which you are challenged to prove your ability to 
survive against the waves of enemies that their main priority is to blow your helicopter out 
of the sky on sight. The helicopter is provided with unlimited fire power and fuel to be able 
to take out your enemies from Tanks, stationary Anti-Air and enemy helicopters. Avoid destruction 
by maneuvering enemy rockets as well as the enemies themselves.

**2. Game Control**  
Player will have full control of the helicopter which is located in the center of the screen. 
To control it, player presses ‘W’, ‘A’, ‘S’, ‘D’ to move forward, left, right, backward, 
‘Space’ to fly up and ‘Left-Shift’ to move down. To shoot the enemy, player will use the 
‘left click mouse’ and move the mouse to turn the helicopter. The player can hold the right mouse 
button to be able to look around without the helicopter rotating towards the camera’s direction. 
The game has three various types of weaponry to play including the Missile, Mine and Bomb. The player 
can cycle through them via the ‘Q’ or ‘E’ buttons, it will take around 1 second to switch the weaponry 
and around 2-4 seconds to reload. The game features the function to play in either first
or third person view, player can press ‘0’ on the keyboard to toggle between these two modes.

**3. Gameplay**  
The objective of the game is pretty basic, player shoots down all the enemies and tries to survive 
through 3 waves. When player plays the game for the first time, there will be only one turret which 
is located in front of the helicopter. We use this one for the purpose of initializing the game and 
helping the player to get comfortable of controlling the helicopter. After the player destroys this 
turret, we give 4 seconds for the game to load the text file and then, render all the enemies in the 
next round. The game will automatically continue to the next round after player successfully destroys 
all the enemies. The player will win if s/he destroys all 3 waves and lose if they get hit 10 times 
(by shell or other enemies).

