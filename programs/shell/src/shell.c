#include "shell.h"
#include "stdio.h"
#include "stdlib.h"
#include "peachos.h"
int main(int argc, char** argv)
{
    print("PeachOS v1.0.0\n");
    print("Turn-based Game Demo\n");
    print("Commands:\n");
    print("  player1.elf (seconds to sleep)\n");
    print("  player2.elf (seconds to sleep)\n\n");

    while(1) 
    {
        print("> ");
        char buf[1024];
        peachos_terminal_readline(buf, sizeof(buf), true);
        print("\n");
        peachos_system_run(buf);
        
        print("\n");
    }
    return 0;
}