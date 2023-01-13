#include <iostream>
#include <unistd.h>
#include <cstring>

#define COLOR_ERROR "\a\033[0;31;1m" // makes font red
#define COLOR_SUCCESS "\a\033[0;32;1m" // makes font green
#define COLOR_RESET "\033[0m" //resets font color

int main(int argc, char *argv[])
{
    // COMPILE EVERY CHANGE IN THE SAME DIRECTORY WITH 'terminalOS2.cpp' with: g++ -o cp cp.cpp
    // argv only consists of things after the dirname command in terminalOS2

    short c;
    bool iFlag = false, rFlag = false, tFlag = false, vFlag = false, hFlag = false;
    char tDir[30];
    while((c = getopt(argc, argv, "irRt:v")) != -1)
    {
        switch(c)
        {
            case 'i':
                iFlag = 1;
                break;
            case 'r':
                rFlag = 1;
                break;
            case 'R':
                rFlag = 1;
                break;
            case 't':
                tFlag = 1;
                strcpy(tDir, optarg);
                // delete the cout after testing
                std::cout << "tDir: " << tDir << '\n';
                break;
            case 'v':
                vFlag = 1;
                break;
            default:
                std::cout << COLOR_ERROR << "Flag '" << c << "' not found." << COLOR_RESET;
                break;
        }
    }
    
    return 0;
}