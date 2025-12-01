#include "peachos.h"
#include "stdio.h"
#include "stdlib.h"

int main(int argc, char** argv)
{
    // Convert argument to integer
    int seconds = atoi(argv[1]);
    if (seconds <= 0) {
        print("Please provide a positive number of seconds.\n");
        return 1;
    }

    // Announce sleep
    print("Player Two will sleep...\n");

    // Sleep
    sleep(seconds);

    // Wake up
    print("Player Two has woken up!\n");

    return 0;
}
