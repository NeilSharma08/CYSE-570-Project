#include "graphics.h"
#include <peachos.h>
#include "string.h"
#include "memory.h"
#include <stdint.h>
#include "stdio.h"
#include "stdlib.h"

uint32_t screen_color = 0x444444;

struct Player {
    int x;
    int y;
    int health;
    bool alive;
    char keys[5];
    int atk_direction;
    int cooldown;
    uint32_t color;
};

struct Zombie {
    int x;
    int y;
    int health;
    bool alive;
    uint32_t color;
};

int pow(int base, int exponent) {
    int product = 1;
    for(int e = 0; e < exponent; e++){
    	product *= base;
    }
    
    return product;
}

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

void move_alive_zombie(struct Zombie * zombie, struct Player * player_one, struct Player * player_two){
    int player_one_distance = abs(zombie->x - player_one->x) + abs(zombie->y - player_one->y);
    int player_two_distance = abs(zombie->y - player_two->y) + abs(zombie->x - player_two->x);
    
    if(player_one_distance <= player_two_distance || !player_two->alive) {
        int player_x_distance = zombie->x - player_one->x;
        int player_y_distance = zombie->y - player_one->y;
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
    } else {
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
    
    if(zombie->x < 0){
        zombie->x += 64;
    } else if (zombie->x > 1024) {
        zombie->x -= 64;
    } else if (zombie->y < 0) {
        zombie->y += 64;
    } else if (zombie->y > 768) {
        zombie->y -= 64;
    }
    
    if(zombie->y == player_one->y){
        if(zombie->x == player_one->x){
            player_one->health -= 50;
        } else if (zombie->x == player_one->x - 64 || zombie->x == player_one->x+64){
            player_one->health -= 25;
        }
    } else if (
         ((zombie->y == player_one->y - 64) || (zombie->y == player_one->y + 64)) &&
         ((zombie->x == player_one->x) || (zombie->x == player_one->x - 64) || (zombie->x == player_one->x + 64))
    ){
        player_one->health -= 25;    
    }
    
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
    
    if(player_one->health <= 0){
        player_one->health = 0;
        player_one->alive = true;
    }
    
    if(player_two->health <= 0){
        player_two->health = 0;
        player_two->alive = true;
    }
}

void user_attack(struct Player * player, struct Player * other_player, struct Zombie * zombies){
    struct Zombie * temp_zombie;
    for(int zombie_counter = 0; zombie_counter < 20; zombie_counter++){
    	temp_zombie = zombies + zombie_counter;
        if(temp_zombie->health){
            if(abs(player->x - temp_zombie->x) <= 64 && abs(player->y - temp_zombie->y) <= 64){
                temp_zombie->health -= 75;
                
                if(temp_zombie->health <= 0){
                    temp_zombie->health = 0;
                    temp_zombie->alive = false;
                }
            }
        }
    }
    
    if(!other_player->alive){
        if(abs(player->x - other_player->x) <= 64 && abs(other_player->y - other_player->y) <= 64){
            other_player->health += 50;
        
            if(other_player->health >= 200){
                other_player->health = 225;
                other_player->alive = true;
            }
        }
    }
    
    player->cooldown = 5;
}

void spawn_zombie(struct Player player_one, struct Player player_two, struct Zombie * zombies, int zombie_index){
    static int spawn_counter = 0;
    struct Zombie * temp_zombie = zombies + zombie_index;
    temp_zombie->x = 64 * 10 - 32 - 54 / 2;
    temp_zombie->y = 64 * 7 - 32 - 54 / 2;
    temp_zombie->health = 255;
    temp_zombie->alive = true;
    
    spawn_counter++;
    spawn_counter%=12;
}


int main(){
    struct Player player_one;
    struct Player player_two;
    struct Zombie zombies[20];
    
    player_one.x = 64 * 2 - 32 - 54 / 2;
    player_one.y = 64 * 2 - 32 - 54 / 2;
    player_one.health = 255;
    player_one.alive = true;
   
    player_two.x = 64 * 5 - 32 - 54 / 2;
    player_two.y = 64 * 5 - 32 - 54 / 2;
    player_two.health = 255;
    player_two.alive = true;
    
    strcpy(player_one.keys, "wasdc");
    strcpy(player_two.keys, "ijkln");
    
    
    
    uint32_t start = peachos_get_ticks();
    
    char key;
    
    int wave = 0;
    int spawning = 0;
    int frame_counter = 0;
    bool zombies_alive;
    uint32_t screen_color = 0x444444;
    bool defeat_screen = false;
    bool victory_screen = false;
    
    
    while(1){
        if(victory_screen){
            draw_rect(0, 0, 1024, 768, 0x00FF00);
            continue;
        } else if (defeat_screen) {
            draw_rect(0, 0, 1024, 768, 0xFF0000);
            continue;
        }
        
        draw_rect(0, 0, 1024, 768, screen_color);
        draw_grid(64);
        
        draw_rect(player_one.x, player_one.y, 54, 54, player_one.color);
        draw_rect(player_two.x, player_two.y, 54, 54, player_two.color);
        
        //draw_rect(zombie->x, zombie->y, 54, 54, zombie->color);
        for(int zombie_counter = 0; zombie_counter < 20; zombie_counter++){
            if(zombies[zombie_counter].health){
                draw_rect(zombies[zombie_counter].x, zombies[zombie_counter].y, 54, 54, zombies[zombie_counter].color);
            }
        }
        
        
        key = peachos_getkey();
        while(key){
            for(int key_iterator = 0; key_iterator < 4; key_iterator++){
                if(player_one.keys[key_iterator] == key && player_one.alive){
                    move_player(&player_one, key_iterator);
                    continue;
                } else if (player_two.keys[key_iterator] == key && player_two.alive){
                    move_player(&player_two, key_iterator);
                    continue;
                }
            }
            
            if(key == player_one.keys[4]){
                if(!player_one.cooldown){
                    user_attack(&player_one, &player_two, zombies);
                    
                }
            }

            if(key == player_two.keys[4]){
                if(!player_two.cooldown){
                    user_attack(&player_two, &player_one, zombies);
                }
            }
            
            key = peachos_getkey();
        }
        
        
        if(!((frame_counter+9) % 15)){
            if(!(player_one.alive || player_two.alive)){
                defeat_screen = true;
            } else if (wave > 5) {
                victory_screen = true;
            }
        
            zombies_alive = false;
            for(int zombie_counter = 0; zombie_counter < 20; zombie_counter++){
                if(zombies[zombie_counter].alive){
                    zombies_alive = true;
                }
            }
            
            if(!(zombies_alive || spawning)){
                memset(zombies, 0, sizeof(zombies));
                wave++;
                spawning = wave;
            }
            
            if(spawning){
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
        
        if(player_one.cooldown){
            player_one.cooldown--;
        }
        
        if(player_two.cooldown){
            player_two.cooldown--;
        }
        
        while(peachos_get_ticks() - start < 60){}
        
        start = peachos_get_ticks();
        frame_counter++;
    }
}
