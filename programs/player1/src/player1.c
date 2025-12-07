#include "peachos.h"
#include "stdio.h"
#include "stdlib.h"

int main(int argc, char** argv)
{
    print("Busy-wait vs sleep test\n");
    print("TEST 1: Count with busy-wait for 3 secs\n");
    
    unsigned int start_time = peachos_get_ticks();
    unsigned int end_time = start_time + 3000;
    
    unsigned int iterations1 = 0;
    while (peachos_get_ticks() < end_time) {
        iterations1++;
        //Busy-wait - spins for 2 seconds 
        peachos_system_run("busywait 2");
    }
    
    print("Work done: ");
    print(itoa(iterations1));
    print(" iterations\n");
    
    peachos_sleep(1);
    
    print("TEST 2: Count + sleep for 3 secs\n");
    
    start_time = peachos_get_ticks();
    end_time = start_time + 3000;
    
    unsigned int iterations2 = 0;
    while (peachos_get_ticks() < end_time) {
        iterations2++;
        
        // Call player2 to sleep for 2 seconds
        peachos_system_run("player2 2");
        
    }
    
    print("Work done: ");
    print(itoa(iterations2));
    print(" iterations\n");
    print("(Efficient yields)\n\n");
    print("===================\n");
    print("COMPARISON:\n");
    print("Busy-wait: ");
    print(itoa(iterations1));
    print("\n");
    print("Sleep:     ");
    print(itoa(iterations2));
    print("\n\n");
    
    if (iterations2 > iterations1) {
        unsigned int diff = iterations2 - iterations1;
        print("Sleep had ");
        print(itoa(diff));
        print(" more iterations!\n");
    }
    
    print("\nBusy-wait WASTES time\n");
    print("Sleep is EFFICIENT\n");

    return 0;
}