#include "peachos.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
int main(int argc, char** argv)
{
    print("Blank program started!\n");
    print("Arguments:\n");
    for (int i = 0; i < argc; i++)
    {
        print(argv[i]);
        print("\n");
    }   
    /*
    while(1)
    {
        print(argv[0]);
        for (int i = 0; i < 1000000; i++)
        {
            
        }
    }
        */
    return 0;
}