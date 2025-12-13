// CYSE 570 Project - Game that runs in usermode

#include "graphics.h"
#include <peachos.h>
#include "string.h"
#include "memory.h"
#include <stdint.h>
#include "stdio.h"
#include "stdlib.h"


// player struct
struct Player {
    int x;
    int y;
    int health;
    bool alive;
    char keys[5]; // characters go up, left, down, right, and attack
    int cooldown;
    uint32_t color;
};

// zombie struct
struct Zombie {
    int x;
    int y;
    int health;
    bool alive;
    uint32_t color;
};


// move_player, takes in a pointer to a player and a 'command' (integer from 0 to 3)
// and moves the player along the grid along one orthogonal direction depending on the command
// meant for easy use with the keyboard handler later
void move_player(struct Player * player, int command) {
    switch(command) {
        case 0:
            if(player->y > 64){
                player->y -= 64;
            }
            break;
        case 1:
            if(player->x > 64){
                player->x -= 64;
            }
            break;
        case 2:
            if(player->y + 64 < 768){
                player->y += 64;
            }
            break; 
        case 3:
            if(player->x + 64 < 1024){
                player->x += 64;
            }
            break;
        default:
            break;
    }
}

// move a zombie, takes in the pointer to the zombie but also a pointer to both players
// because the zombie calculates where it has to move
void move_alive_zombie(struct Zombie * zombie, struct Player * player_one, struct Player * player_two){
    // comparing the sum of x and y distances to determine which player is closer to go after
    int player_one_distance = abs(zombie->x - player_one->x) + abs(zombie->y - player_one->y);
    int player_two_distance = abs(zombie->y - player_two->y) + abs(zombie->x - player_two->x);

    // if player_one is closer than player two OR if player two isn't alive (no use in killing someones whos already dead)
    if(player_one_distance <= player_two_distance || !player_two->alive) {
        // do we travel along the x or y axis on our next move?
        // decision making: we calculate whether the x or y distance is greater and we step against whichever one is greater
        int player_x_distance = zombie->x - player_one->x;
        int player_y_distance = zombie->y - player_one->y;
        
        if(abs(player_x_distance) >= abs(player_y_distance)){
            if(player_x_distance < 0){ // do we move forwards or backwards? if this is negative it means player_one.x is greater than zombie.x, so we have to move forward 
                zombie->x += 64;
            } else {
                zombie->x -= 64;
            }
        } else { 
            if(player_y_distance < 0){ // identical logic as earlier
                zombie->y += 64;
            } else {
                zombie->y -= 64;
            }
        }
    } else {
        // same thing again just with player_two instead of one 
        int player_x_distance = zombie->x - player_two->x;
        int player_y_distance = zombie->y - player_two->y;
        
        if(abs(player_x_distance) >= abs(player_y_distance)){
            if(player_x_distance < 0){
                zombie->x += 64;
            } else {
                zombie->x -= 64;
            }
        } else {
            if(player_y_distance < 0){
                zombie->y += 64;
            } else {
                zombie->y -= 64;
            }
        }        
    }

    // did we go out of bounds (somehow)? if so push the zombie back
    if(zombie->x < 0){
        zombie->x += 64;
    } else if (zombie->x > 1024) {
        zombie->x -= 64;
    } else if (zombie->y < 0) {
        zombie->y += 64;
    } else if (zombie->y > 768) {
        zombie->y -= 64;
    }

    // collision checking, is the zombie within range of either of the players to hurt them?
    // zombies hurt players if theyre within a 1 block distance of them AND if and only if they move so you can like juke them out if you want to
    if(zombie->y == player_one->y){
        if(zombie->x == player_one->x){
            player_one->health -= 50; // if a zombie gets on TOP of you then yeah you're taking double damage
        } else if (zombie->x == player_one->x - 64 || zombie->x == player_one->x+64){
            player_one->health -= 25; // but otherwise its just a scrape
        }
    } else if (
         ((zombie->y == player_one->y - 64) || (zombie->y == player_one->y + 64)) &&
         ((zombie->x == player_one->x) || (zombie->x == player_one->x - 64) || (zombie->x == player_one->x + 64))
    ){
        player_one->health -= 25;    
    }

    // same thing for player two instead of one again
    if(zombie->y == player_two->y){
        if(zombie->x == player_two->x){
            player_two->health -= 50;
        } else if (zombie->x == player_two->x - 64 || zombie->x == player_two->x+64){
            player_two->health -= 25;
        }
    } else if (
         ((zombie->y == player_two->y - 64) || (zombie->y == player_two->y + 64)) &&
         ((zombie->x == player_two->x) || (zombie->x == player_two->x - 64) || (zombie->x == player_two->x + 64))
    ){
        player_two->health -= 25;    
    } 

    // make sure we dont have any negative health bc that will mess with the color calculation (later)
    // set it to 0 if under 0 and make sure they know they're dead
    if(player_one->health <= 0){
        player_one->health = 0;
        player_one->alive = false;
    }
    
    if(player_two->health <= 0){
        player_two->health = 0;
        player_two->alive = false;
    }
}


// when a user presses the attack keybind
// takes in the player who pressed it, the other player, and a pointer to the zombies
void user_attack(struct Player * player, struct Player * other_player, struct Zombie * zombies){
    // use a temp zombie pointer to iterate over the zombies
    struct Zombie * temp_zombie;
    for(int zombie_counter = 0; zombie_counter < 20; zombie_counter++){
        // pointer magic
    	temp_zombie = zombies + zombie_counter;
        if(temp_zombie->health){ // not worth beating a dead zombie
            // is the player around the zombie (area of effect attack), within one cell (64 pixels)
            if(abs(player->x - temp_zombie->x) <= 64 && abs(player->y - temp_zombie->y) <= 64){
                temp_zombie->health -= 75; // crush that zombie
                
                if(temp_zombie->health <= 0){ // same reason to zero out zombies health the way we do with players
                    temp_zombie->health = 0;
                    temp_zombie->alive = false; // they also die
                }
            }
        }
    }

    // whats cool is that if the other player isn't alive you can actually resurrect them
    if(!other_player->alive){
        // are you within 1 block of the other player?
        if(abs(player->x - other_player->x) <= 64 && abs(other_player->y - other_player->y) <= 64){
            other_player->health += 50; // they get a 50 health boost but are still immobile until...
        
            if(other_player->health >= 200){ // their health hits 200
                other_player->health = 225;
                other_player->alive = true; // then they're actually alive
            }
        }
    }

    // you cant spam this or the game wouldnt be fun we'll make you wait a few frames so you gotta learn to dodge
    player->cooldown = 5;
}

// spawn a zombie like during waves 
void spawn_zombie(struct Zombie * zombies, int zombie_index){
    struct Zombie * temp_zombie = zombies + zombie_index;
    // coordinates (10, 7) on the grid. the arithimetic is just to convert it into the topleft pixel that we draw the rect at
    temp_zombie->x = 64 * 10 - 32 - 54 / 2;
    temp_zombie->y = 64 * 7 - 32 - 54 / 2;
    temp_zombie->health = 255;
    temp_zombie->alive = true;
}


int main(){
    struct Player player_one;
    struct Player player_two;
    struct Zombie zombies[20];

    // player one spawns at (2, 2) on the grid same thing with the arithmetic
    player_one.x = 64 * 2 - 32 - 54 / 2;
    player_one.y = 64 * 2 - 32 - 54 / 2;
    player_one.health = 255;
    player_one.alive = true;


    // player two spawns at 5 5
    player_two.x = 64 * 5 - 32 - 54 / 2;
    player_two.y = 64 * 5 - 32 - 54 / 2;
    player_two.health = 255;
    player_two.alive = true;
    
    strcpy(player_one.keys, "wasdc"); // WASD, c to attack
    strcpy(player_two.keys, "ijkln"); // IJKL, n to attack
    
    
    
    uint32_t start = peachos_get_ticks(); // way of tracking time, set to 1000Hz by the PIT
    
    char key; // key that will be pressed by the player
    
    int wave = 0; // what wave of zombies are we on 
    int spawning = 0; // what/how many zombies are spawning
    int frame_counter = 0; // how many frames have elapsed (used for executing something periodically)
    bool zombies_alive; // are there zombies alive right now?
    uint32_t screen_color = 0x444444; // screen color
    bool defeat_screen = false; // have we died yet?
    bool victory_screen = false; // have we won yet (impressive)
    
    
    while(1){
        // i thought it would be funny to just blind you and not let you do anything if you won or lost
        if(victory_screen){
            draw_rect(0, 0, 1024, 768, 0x00FF00);
            continue;
        } else if (defeat_screen) {
            draw_rect(0, 0, 1024, 768, 0xFF0000);
            continue;
        }

        // gray background
        draw_rect(0, 0, 1024, 768, screen_color);
        draw_grid(64); // grid with 64 pixel spacing between the lines (the screen dimensions are 1024 x 768 which are both divisible by 64)

        // make entities just slightly smaller than the grid (54 instead of 64) so it doesnt clash with the lines on the grid
        draw_rect(player_one.x, player_one.y, 54, 54, player_one.color);
        draw_rect(player_two.x, player_two.y, 54, 54, player_two.color);

        // for the zombies that are alive, draw them!
        for(int zombie_counter = 0; zombie_counter < 20; zombie_counter++){
            if(zombies[zombie_counter].health){
                draw_rect(zombies[zombie_counter].x, zombies[zombie_counter].y, 54, 54, zombies[zombie_counter].color);
            }
        }
        
        
        key = peachos_getkey();
        // in case multiple keys are pressed in a frame
        while(key){
            // iterates over player_one and player_twos assigned movement keys
            // if it falls under it, call move_player with the iterator variable
            // (move_player is optimized to make it easier for the key_iterator to be passed directly)
            for(int key_iterator = 0; key_iterator < 4; key_iterator++){
                if(player_one.keys[key_iterator] == key && player_one.alive){
                    move_player(&player_one, key_iterator);
                    continue;
                } else if (player_two.keys[key_iterator] == key && player_two.alive){
                    move_player(&player_two, key_iterator);
                    continue;
                }
            }

            // but if its the last key, attack!
            if(key == player_one.keys[4]){
                if(!player_one.cooldown){ // but only if they havent attacked recently
                    user_attack(&player_one, &player_two, zombies);
                }
            }

            if(key == player_two.keys[4]){
                if(!player_two.cooldown){
                    user_attack(&player_two, &player_one, zombies);
                }
            }

            // in case theres more keys left in the queue
            key = peachos_getkey();
        }
        
        // once every 15 frames (starting at the 9th) check for some conditions 
        if(!((frame_counter+9) % 15)){ 
            if(!(player_one.alive || player_two.alive)){ // are both players dead?
                defeat_screen = true; // jumpscare them
            } else if (wave > 5) { // did they win?
                victory_screen = true; // also jumpscare them
            }

            // are all the zombies dead (should we start the next wave)?
            zombies_alive = false;
            for(int zombie_counter = 0; zombie_counter < 20; zombie_counter++){
                if(zombies[zombie_counter].alive){
                    zombies_alive = true;
                }
            }

            // if all zombies are dead (and theres not more incoming right now)
            if(!(zombies_alive || spawning)){ 
                memset(zombies, 0, sizeof(zombies)); // zero out the zombies array to get rid of garbage
                wave++; // increment the wave
                spawning = wave; // you spawn as many zombies as the wave is (at the beginning its 1, then 2, etc) 
            }

            // after a few loops, every 15 frames, a zombie will spawn again.
            if(spawning){
                // spawn zombie and decrement
                spawn_zombie(player_one, player_two, zombies, wave - spawning);
                spawning--;
            }
        }
        
        
        for(int zombie_counter = 0; zombie_counter < 20; zombie_counter++){
            if(zombies[zombie_counter].alive){
                zombies[zombie_counter].color = zombies[zombie_counter].health << 16;
                if(!(frame_counter % 5)){
                    move_alive_zombie(&zombies[zombie_counter], &player_one, &player_two);
                }
            }
        }
        
        player_one.color = ((255-player_one.health) << 16) + (player_one.health << 8);
        player_two.color = ((255-player_two.health) << 16) + (player_two.health << 8);

        // if attacks are cooling down let it cooldown, they can only attack again when theyre at 0
        if(player_one.cooldown){
            player_one.cooldown--;
        }
        
        if(player_two.cooldown){
            player_two.cooldown--;
        }
        
        while(peachos_get_ticks() - start < 60){} // count until 60 ticks (mainloop runs about 1000/60 = ~16 times a second)
        
        start = peachos_get_ticks(); 
        frame_counter++;
    }
}
