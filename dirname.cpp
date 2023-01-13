#include <iostream>
#include <unistd.h>
#include <cstring>

#define COLOR_ERROR "\a\033[0;31;1m" // makes font red
#define COLOR_RESET "\033[0m" //resets font color

int main(int argc, char *argv[])
{
    // COMPILE EVERY CHANGE IN THE SAME DIRECTORY WITH 'terminalOS2.cpp' with: g++ -o dirname dirname.cpp
    // argv only consists of things after the dirname command in terminalOS2
    // no flags have been assigned for this function recreation

    // more detailed comments since it's smaller code
    // checking for flags
    for(short i = 1; i < argc; ++i)
        if (argv[i][0] == '-') // if the first character in this argument is '-', that means it's a flag, so we notify the user
        {
            std::cout << COLOR_ERROR << "Command does not accept flags.\n" << COLOR_RESET;
            return 1;
        }
    // going through all arguments given
    for (short i = 1; i < argc; ++i)
    {
        int indexLS = -1; // index last slash
        int len = strlen(argv[i]);
        // finds the last slash (as long as it's not the last character of the given path)
        for (short j = 0; j < len; ++j)
            if (argv[i][j] == '/' && j != len - 1)
                indexLS = j;
        // if it hasn't been modified, then just output "." and go to the next one
        if (indexLS == -1)
            std::cout << ".\n";
        else if (indexLS == 0)
            std::cout << "/\n"; // if indexLS is 0 then just output a slash and new line, since it can't have anything after it in this case
        else
        {
            // output until the last slash
            for (short j = 0; j < indexLS; ++j)
                std::cout << argv[i][j];
            std::cout << '\n';
        }
    }

    return 0;
}