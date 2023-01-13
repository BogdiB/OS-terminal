#include <iostream>
#include <unistd.h>
#include <cstring>

#define COLOR_ERROR "\a\033[0;31;1m" // makes font red
#define COLOR_RESET "\033[0m" //resets font color

int main(int argc, char *argv[])
{
    // COMPILE EVERY CHANGE IN THE SAME DIRECTORY WITH 'terminalOS2.cpp' with: g++ -o mv mv.cpp
    // argv only consists of things after the dirname command in terminalOS2
    
    short c;
    bool iFlag = false, tFlag = false, SFlag = false;
    char tDir[30];
    while((c = getopt(argc, argv, "it:S")) != -1)
    {
        switch(c)
        {
            case 'i':
                iFlag = 1;
                break;
            case 't':
                tFlag = 1;
                strcpy(tDir, optarg);
                // delete the cout after testing
                std::cout << "tDir: " << tDir << '\n';
                break;
            case 'S':
                // with -b flag
                SFlag = 1;
                break;
            default:
                std::cout << COLOR_ERROR << "Flag '" << c << "' not found.\n" << COLOR_RESET;
                break;
        }
    }
    return 0;
}