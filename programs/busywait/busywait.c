#include "peachos.h"
#include "stdlib.h"

// Busy-wait program - spins CPU for specified seconds
int main(int argc, char** argv){
    // Convert argument to integer
    int seconds = atoi(argv[1]);
    if (seconds <= 0) {
        print("Please provide a positive number of seconds.\n");
        return 1;
    }

    // Announce busy-wait
    print("Busywait: Starting...\n");

    // Busy-wait - spin CPU for the specified number of seconds
    unsigned int busy_start = peachos_get_ticks();
    while (peachos_get_ticks() - busy_start < seconds * 1000) {
        // Spinning CPU - blocking others!
    }

    // Done
    print("Busywait: Done!\n");

    return 0;
}
