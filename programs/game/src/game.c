#include <graphics.h>
#include <peachos.h>
#include <string.h>

struct Player {
    int x;
    int y;
    char keys[4];
};

void move(struct Player player, int command) {
    switch(command) {
        case 0:
            break;
        case 1:
            if(player.x + 64 < 768){
                player.x += 64;
            }
            break;
        case 2:
            if(player.y + 64 < 1024){
                player.y += 64;
            }
            break;
        case 3:
            if(player.x > 64){
                player.x -= 64;
            }
            break; 
        case 4:
            if(player.y > 64){
                player.y -= 64;
            }
            break;
        default:
            break;
    }
}

int main(){
    
    struct Player player_one;
    struct Player player_two;
    player_one.x = 64 * 2 - 32 - 54 / 2;
    player_one.y = 64 * 2 - 32 - 54 / 2;
   
    player_two.x = 64 * 10 - 32 - 54 / 2;
    player_two.y = 64 * 10 - 32 - 54 / 2;
    
    strcpy("wasd", player_one.keys);
    strcpy("ijkl", player_two.keys);
    
    char key;
    
    uint32_t screen_color = 0xFFFFFF;
    //int command;
    
    //disable_interrupts();
    uint32_t start = peachos_get_ticks();
   // uint32_t end = get_ticks();
    //enable_interrupts();
    
    while(1){
        draw_rect(0, 0, 1024, 768, screen_color);
        draw_grid(64);
        
        draw_rect(player_one.x, player_one.y, 54, 54, 0x00FF00);
        draw_rect(player_two.x, player_two.y, 54, 54, 0x0000FF);
        
        player_one.x += 64;
        
        
        //disable_interrupts();
        
        while(peachos_get_ticks() - start < 16){
        }
        
        start = peachos_get_ticks();
        //enable_interrupts();
        
        key = peachos_getkey();
        if(key){
            draw_rect(0, 0, 1024, 768, 0x000000);
            screen_color = 0xFF0000;
        }
    }
}
