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
};

struct Enemy {
    int x;
    int y;
    int health;
    bool tamed;
};

void move(struct Player * player, int command) {
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

int main(){
    struct Player player_one;
    struct Player player_two;
    //struct Enemy enemies[20];
    
    player_one.x = 64 * 2 - 32 - 54 / 2;
    player_one.y = 64 * 2 - 32 - 54 / 2;
    player_one.health = 100;
    player_one.alive = true;
   
    player_two.x = 64 * 10 - 32 - 54 / 2;
    player_two.y = 64 * 10 - 32 - 54 / 2;
    player_two.alive = true;
    
    strcpy(player_one.keys, "wasdv");
    strcpy(player_two.keys, "ijkln");
    
    
    uint32_t start = peachos_get_ticks();
    
    char key;
    uint32_t cube_color = 0xff0000;
    
    while(1){
        draw_rect(0, 0, 1024, 768, 0xffffff);
        draw_grid(64);
        draw_rect(50, 30, 30, 30, 0xffff00);
        
        draw_rect(player_one.x, player_one.y, 54, 54, cube_color);
        draw_rect(player_two.x, player_two.y, 54, 54, cube_color);
        
        while(peachos_get_ticks() - start < 60){}
        
        key = peachos_getkey();
        if(key){
            for(int key_iterator = 0; key_iterator < 4; key_iterator++){
                if(player_one.keys[key_iterator] == key && player_one.alive){
                    move(&player_one, key_iterator);
                } else if (player_two.keys[key_iterator] == key && player_two.alive){
                    move(&player_two, key_iterator);
                }
            }
        }
        
        start = peachos_get_ticks();
    }
}
