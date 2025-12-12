#include "graphics.h"
#include <peachos.h>
#include <string.h>
#include <stdint.h>
#include "stdio.h"
#include "stdlib.h"

struct Player {
    int x;
    int y;
    int health;
    bool alive;
    char keys[5];
    int atk_direction;
};

struct Zombie {
    int x;
    int y;
    int health;
    bool tamed;
    int cooldown;
};

int pow(int base, int exponent) {
    int product = 1;
    for(int e = 0; e < exponent; e++){
    	product *= base;
    }
    
    return product;
}

int random_number() {
    return pow(peachos_get_ticks() % 20, peachos_get_ticks() / 20 % 10) % 1000;
}

void move_to_open_spot(struct Zombie * zombie, struct Player player_one, struct Player player_two, struct Zombie * zombies[]){
    
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

void move_alive_zombie(struct Zombie * zombie, struct Player player_one, struct Player player_two){
    int player_one_distance = abs(zombie->x - player_one.x) + abs(zombie->y - player_one.y);
    int player_two_distance = abs(zombie->y - player_two.y) + abs(zombie->x - player_two.x);
    
    if(player_one_distance <= player_two_distance || !player_two.alive) {
        int player_x_distance = zombie->x - player_one.x;
        int player_y_distance = zombie->y - player_one.y;
        if(abs(player_x_distance) >= abs(player_y_distance)){
            if(player_x_distance < 0){
                if(random_number() > -1){
                    zombie->x += 64;
                } else {
                    zombie->x -= 64;
                }
            } else {
                if(random_number() > -1){
                    zombie->x -= 64;
                } else {
                    zombie->x += 64;
                }            
            }
        } else { 
            if(player_y_distance < 0){
                if(random_number() > -1){
                    zombie->y += 64;
                } else {
                    zombie->y -= 64;
                }
            } else {
                if(random_number() > -1){
                    zombie->y -= 64;
                } else {
                    zombie->y += 64;
                }            
            }
        }
    } else {
        int player_x_distance = zombie->x - player_two.x;
        int player_y_distance = zombie->y - player_two.y;
        if(abs(player_x_distance) >= abs(player_y_distance)){
            if(player_x_distance < 0){
                if(random_number() > -1){
                    zombie->x += 64;
                } else {
                    zombie->x -= 64;
                }
            } else {
                if(random_number() > -1){
                    zombie->x -= 64;
                } else {
                    zombie->x += 64;
                }            
            }
        } else { 
            if(player_y_distance < 0){
                if(random_number() > -1){
                    zombie->y += 64;
                } else {
                    zombie->y -= 64;
                }
            } else {
                if(random_number() > -1){
                    zombie->y -= 64;
                } else {
                    zombie->y += 64;
                }            
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
}

int main(){
    struct Player player_one;
    struct Player player_two;
    struct Zombie zombie;
    
    player_one.x = 64 * 2 - 32 - 54 / 2;
    player_one.y = 64 * 2 - 32 - 54 / 2;
    player_one.health = 100;
    player_one.alive = true;
    player_one.atk_direction = 2;
   
    player_two.x = 64 * 10 - 32 - 54 / 2;
    player_two.y = 64 * 10 - 32 - 54 / 2;
    player_one.health = 100;
    player_two.alive = true;
    player_one.atk_direction = 1;
    
    strcpy(player_one.keys, "wasdv");
    strcpy(player_two.keys, "ijkln");
    
    zombie.x = 64 * 6 - 32 - 54 / 2;
    zombie.y = 64 * 6 - 32 - 54 / 2;
    
    
    uint32_t start = peachos_get_ticks();
    
    char key;
    uint32_t cube_color = 0x00ff00;
    
    //int wave = 0;
    int frame_counter = 0;
    
    
    while(1){
<<<<<<< HEAD
        //draw_rect(0, 0, 1024, 768, 0xffffff);
        //draw_grid(64);
        draw_rect(50, 30, 30, 30, 0xffff00);
        
        draw_rect(player_one.x, player_one.y, 54, 54, cube_color);
        draw_rect(player_two.x, player_two.y, 54, 54, cube_color);
        
        while(peachos_get_ticks() - start < 1000){}
=======
        draw_rect(0, 0, 1024, 768, 0x444444);
        //draw_grid(64);
        //draw_rect(50, 30, 30, 30, 0xffff00);
        
        draw_rect(player_one.x, player_one.y, 54, 54, cube_color);
        draw_rect(player_two.x, player_two.y, 54, 54, cube_color);
        draw_rect(zombie.x, zombie.y, 54, 54, 0xff0000);
>>>>>>> 1199eff (further dev to the game)
        
        key = peachos_getkey();
        if(key){
            for(int key_iterator = 0; key_iterator < 4; key_iterator++){
                if(player_one.keys[key_iterator] == key && player_one.alive){
                    move_player(&player_one, key_iterator);
                } else if (player_two.keys[key_iterator] == key && player_two.alive){
                    move_player(&player_two, key_iterator);
                }
            }
        }
        
        if(!(frame_counter % 5)){
             move_alive_zombie(&zombie, player_one, player_two);
        }
        
        while(peachos_get_ticks() - start < 60){}
        
        start = peachos_get_ticks();
        frame_counter++;
    }
}
