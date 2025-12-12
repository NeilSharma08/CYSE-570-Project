#include "graphics.h"
#include <peachos.h>
#include <string.h>
#include <stdint.h>
#include "stdio.h"
#include "stdlib.h"



int main(){
    print("I'm about to draw rect!\n");
    draw_rect(0, 0, 1024, 768, 0xff0000);
    print("I finished drawing rect!\n");
    while(1){};
}