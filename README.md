# Developing the Zombie Game and optimizing a multithreaded kernel from scratch!

Please find the instructions to access and play Zombie Game below.

The Zombie Game is located on GitHub: https://github.com/NeilSharma08/CYSE-570-Project 
Access our CYSE-570-Project repository to download the game. Make a directory for the game and clone the repository. 

# Steps to install and run the game in our kernel from command line 
1.  Navigate to the folder with the game code
2.  make clean
3.	./build.sh
4.	qemu-system-i386 -hda ./bin/os.bin -vga std

# Game Controls:
Player 1 uses the W, A, S, D keys, while Player 2 uses the I, J, K, L keys 

# Zombie Game Description
The Zombie Game is a 2-player real-time arena survival game which takes place on a 16x12 grid-based board where players fight successive hordes of enemies. When the game starts, all units will have a predetermined starting location. Player units are initially depicted visually as green squares while enemy units are shown as red squares. Each player may move their unit one space up, left, down, or right one space; Player 1 uses the W, A, S, D keys, while Player 2 uses the I, J, K, L keys respectively. For enemy units, they will often head towards the nearest player (check to see if this part is true) to be in range to attack. If a player unit is within range of an enemy, an attack can be initiated by Player 1 pressing the C key and Player 2 pressing the N key. 

Both player and enemy units have a 3x3 grid attack range, with the unit at the center; however, the attack rate of the enemy units is much slower than the player units. As a result, this will incentivize the players to time their attacks and move in and out of the enemy unit’s attack range to avoid taking damage. All units in the game possess a finite amount of health (HP) which is reduced whenever units take damage from attacks from the opposing side. As both player and enemy units lose health, their color will change, with the player units turning red and the enemy units turning black. Once a player or enemy unit’s HP is reduced to zero, that unit is removed from the board. If all enemies on the board are defeated, then more enemies will appear; should all players be defeated, the game will end.
