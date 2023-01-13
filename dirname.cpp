#include <iostream>
#include <unistd.h>
#include <cstring>

int main(int argc, char *argv[])
{
    // COMPILE EVERY CHANGE IN THE SAME DIRECTORY WITH 'terminalOS2.cpp' with: g++ -o dirname dirname.cpp
    // argv only consists of things after the dirname command in terminalOS2
    // no flags have been assigned for this function recreation
    for (short i = 1; i < argc; ++i)
    {
        int indexLS = -1;
        int len = strlen(argv[i]);
        for (short j = 0; j < len; ++j)
        {
            if (argv[i][j] == '/' && j != len - 1)
                indexLS = j;
        }
        if (indexLS == -1)
            std::cout << ".\n";
        else if (indexLS == 0)
            std::cout << "/\n";
        else
        {
            for (short j = 0; j < indexLS; ++j)
                std::cout << argv[i][j];
            std::cout << '\n';
        }
        //  other trial, ignore

        // char *indexLS = strrchr(argv[i], '/'); // index last slash
        // if (indexLS != nullptr)
        // {
        //     if (indexLS - argv[i] + 1 != strlen(argv[i]) - 1);
        //     {
        //         ;
        //     }
        //     else
        //     {
        //         for (short j = 0; j < strlen(argv[i]); ++j)
        //         {
        //             ;
        //         }
        //         return 0;
        //     }
        // }
        // else
        //     std::cout << ".\n";
    }

    return 0;
}