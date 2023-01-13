#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstring>
#include <readline/readline.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <errno.h>
#include <libgen.h>

#define COLOR_ERROR "\a\033[0;31;1m" // makes font red
#define COLOR_RESET "\033[0m" //resets font color

bool iFlag = false, tFlag = false, SFlag = false;
char *fD[50], currentPath[PATH_MAX];
// char tDir[30];
short fDNr = 0;

bool prompt()
{
    // prompt for overwriting, if no, then don't copy
    std::cout << "Overwrite '" << fD[1] << "'?(y/n): ";
    char yn = getc(stdin);
    // any other input besides 'y' and 'Y' gets treated as no
    if (yn == 'y' || yn == 'Y')
    {
        std::cin.clear();
        std::cin.ignore();
        return true;
    }
    std::cin.clear();
    std::cin.ignore();
    return false;
}

void mvS()
{
    ;
}

void mvt()
{
    char path[PATH_MAX], cPath[PATH_MAX];
    // the moving
    if (fDNr > 2)
    {
        // fD[i], fD[fDNr - 1]
        // getting the main directory path
        strcpy(path, currentPath);
        if (fD[fDNr - 1][0] == '.')
        {
            // getting rid of the '.'
            *(fD[fDNr - 1]) = *(fD[fDNr - 1] + 1);
            // for (short i = 0; i < strlen(fD[fDNr - 1]) - 1; ++i)
                // fD[fDNr - 1][i] = fD[fDNr - 1][i + 1];
            if (fD[fDNr - 1][0] != '/')
                strcat(path, "/");
            strcat(path, fD[fDNr - 1]);
        }
        else
        {
            if (fD[fDNr - 1][0] != '/')
                strcat(path, "/");
            strcat(path, fD[fDNr - 1]);
        }

        for (short i = 0; i < fDNr - 1; ++i) // fDNr - 1 because the last one will be the path  // fD[i], fD[fDNr - 1]
        {
            strcpy(cPath, currentPath);
            if (fD[fDNr - 1][0] == '.')
            {
                // getting rid of the '.'
                *(fD[fDNr - 1]) = *(fD[fDNr - 1] + 1);
                // for (short i = 0; i < strlen(fD[fDNr - 1]) - 1; ++i)
                    // fD[fDNr - 1][i] = fD[fDNr - 1][i + 1];
                if (fD[fDNr - 1][0] != '/')
                    strcat(cPath, "/");
                strcat(cPath, fD[fDNr - 1]);
            }
            else
            {
                if (fD[fDNr - 1][0] != '/')
                    strcat(cPath, "/");
                strcat(cPath, fD[fDNr - 1]);
            }

            std::ifstream ifs(cPath, std::ios::in | std::ios::binary);
            std::ofstream ofs(path, std::ios::out | std::ios::binary);
            ofs << ifs.rdbuf();
            ifs.close();
            ofs.close();
            remove(cPath);
        }
            // if (rename(fD[i], fD[fDNr - 1]) != 0) // renames file, and moves it if need be
            //     perror("Error renaming/moving file");
    }
    else
    {
        // fD[0], fD[1]
        // getting the main directory path
        strcpy(path, currentPath);
        if (fD[fDNr - 1][0] == '.')
        {
            // getting rid of the '.'
            *(fD[fDNr - 1]) = *(fD[fDNr - 1] + 1);
            // for (short i = 0; i < strlen(fD[fDNr - 1]) - 1; ++i)
                // fD[fDNr - 1][i] = fD[fDNr - 1][i + 1];
            if (fD[fDNr - 1][0] != '/')
                strcat(path, "/");
            strcat(path, fD[fDNr - 1]);
        }
        else
        {
            if (fD[fDNr - 1][0] != '/')
                strcat(path, "/");
            strcat(path, fD[fDNr - 1]);
        }

        strcpy(cPath, currentPath);
        if (fD[fDNr - 1][0] == '.')
        {
            // getting rid of the '.'
            *(fD[fDNr - 1]) = *(fD[fDNr - 1] + 1);
            // for (short i = 0; i < strlen(fD[fDNr - 1]) - 1; ++i)
                // fD[fDNr - 1][i] = fD[fDNr - 1][i + 1];
            if (fD[fDNr - 1][0] != '/')
                strcat(cPath, "/");
            strcat(cPath, fD[fDNr - 1]);
        }
        else
        {
            if (fD[fDNr - 1][0] != '/')
                strcat(cPath, "/");
            strcat(cPath, fD[fDNr - 1]);
        }

        std::ifstream ifs(cPath, std::ios::in | std::ios::binary);
        std::ofstream ofs(path, std::ios::out | std::ios::binary);
        ofs << ifs.rdbuf();
        ifs.close();
        ofs.close();
        remove(cPath);
    }
}

// other way to do mv
void mv()
{
    char path[PATH_MAX], cPath[PATH_MAX];
    // the moving
    if (fDNr > 2)
    {
        // fD[i], fD[fDNr - 1]
        // getting the main directory path
        strcpy(path, currentPath);
        if (fD[fDNr - 1][0] == '.')
        {
            // getting rid of the '.'
            *(fD[fDNr - 1]) = *(fD[fDNr - 1] + 1);
            // for (short i = 0; i < strlen(fD[fDNr - 1]) - 1; ++i)
                // fD[fDNr - 1][i] = fD[fDNr - 1][i + 1];
            if (fD[fDNr - 1][0] != '/')
                strcat(path, "/");
            strcat(path, fD[fDNr - 1]);
        }
        else
        {
            if (fD[fDNr - 1][0] != '/')
                strcat(path, "/");
            strcat(path, fD[fDNr - 1]);
        }

        for (short i = 0; i < fDNr - 1; ++i) // fDNr - 1 because the last one will be the path  // fD[i], fD[fDNr - 1]
        {
            strcpy(cPath, currentPath);
            if (fD[fDNr - 1][0] == '.')
            {
                // getting rid of the '.'
                *(fD[fDNr - 1]) = *(fD[fDNr - 1] + 1);
                // for (short i = 0; i < strlen(fD[fDNr - 1]) - 1; ++i)
                    // fD[fDNr - 1][i] = fD[fDNr - 1][i + 1];
                if (fD[fDNr - 1][0] != '/')
                    strcat(cPath, "/");
                strcat(cPath, fD[fDNr - 1]);
            }
            else
            {
                if (fD[fDNr - 1][0] != '/')
                    strcat(cPath, "/");
                strcat(cPath, fD[fDNr - 1]);
            }

            std::ifstream ifs(cPath, std::ios::in | std::ios::binary);
            std::ofstream ofs(path, std::ios::out | std::ios::binary);
            ofs << ifs.rdbuf();
            ifs.close();
            ofs.close();
            remove(cPath);
        }
            // if (rename(fD[i], fD[fDNr - 1]) != 0) // renames file, and moves it if need be
            //     perror("Error renaming/moving file");
    }
    else
    {
        // fD[0], fD[1]
        // getting the main directory path
        strcpy(path, currentPath);
        if (fD[fDNr - 1][0] == '.')
        {
            // getting rid of the '.'
            *(fD[fDNr - 1]) = *(fD[fDNr - 1] + 1);
            // for (short i = 0; i < strlen(fD[fDNr - 1]) - 1; ++i)
                // fD[fDNr - 1][i] = fD[fDNr - 1][i + 1];
            if (fD[fDNr - 1][0] != '/')
                strcat(path, "/");
            strcat(path, fD[fDNr - 1]);
        }
        else
        {
            if (fD[fDNr - 1][0] != '/')
                strcat(path, "/");
            strcat(path, fD[fDNr - 1]);
        }

        strcpy(cPath, currentPath);
        if (fD[fDNr - 1][0] == '.')
        {
            // getting rid of the '.'
            *(fD[fDNr - 1]) = *(fD[fDNr - 1] + 1);
            // for (short i = 0; i < strlen(fD[fDNr - 1]) - 1; ++i)
                // fD[fDNr - 1][i] = fD[fDNr - 1][i + 1];
            if (fD[fDNr - 1][0] != '/')
                strcat(cPath, "/");
            strcat(cPath, fD[fDNr - 1]);
        }
        else
        {
            if (fD[fDNr - 1][0] != '/')
                strcat(cPath, "/");
            strcat(cPath, fD[fDNr - 1]);
        }

        std::ifstream ifs(cPath, std::ios::in | std::ios::binary);
        std::ofstream ofs(path, std::ios::out | std::ios::binary);
        ofs << ifs.rdbuf();
        ifs.close();
        ofs.close();
        remove(cPath);
    }
        // if (rename(fD[0], fD[1]) != 0) // renames file, and moves it if need be
        //     perror("Error renaming/moving file");

    // std::ifstream ifs(oldname, std::ios::in | std::ios::binary);
    // std::ofstream ofs(newname, std::ios::out | std::ios::binary);
    // ofs << ifs.rdbuf();
    // ifs.close();
    // ofs.close();
    // remove(oldname);
}

int main(int argc, char *argv[])
{
    // COMPILE EVERY CHANGE IN THE SAME DIRECTORY WITH 'terminalOS2.cpp' with: g++ -o mv mv.cpp
    // argv only consists of things after the dirname command in terminalOS2

    if (getcwd(currentPath, sizeof(currentPath)) == NULL)
    {
        perror("Error getting the current path");
        return 1;
    }

    std::cout << "You have to be in the same directory or higher.\n";

    // keeping everything except argv[0] and flags in fD for later use
    for(short i = 1; i < argc; ++i)
        if (argv[i][0] != '-') // if the first character in this argument is '-', that means it's a flag, so we skip over it
        {
            *(fD + i) = argv[i];
            ++fDNr;
        }

    short c;
    // assinging true to the flags given in the arguments
    while((c = getopt(argc, argv, "it:S")) != -1)
    {
        switch(c)
        {
            case 'i':
                // shows prompt
                iFlag = 1;
                break;
            case 't':
                // target directory, moves all args in the directory
                tFlag = 1;
                // strcpy(tDir, optarg);
                // // delete the cout after testing
                // std::cout << "tDir: " << tDir << '\n';
                break;
            case 'S':
                // make it with -b flag too
                SFlag = 1;
                break;
            default:
                std::cout << COLOR_ERROR << "Flag '" << c << "' not found.\n" << COLOR_RESET;
                break;
        }
    }
    // checking the flags after assignment
    if (iFlag)
        if (!prompt())
            return 0;
    if (tFlag)
    {
        // means fD[0] will be target
        if (fDNr > 2)
            for (short i = 1; i < fDNr; ++i) // i =  1 because the first one will be the path // fD[i], fD[0]
                if (rename(fD[i], fD[0]) != 0) // renames file, and moves it if need be
                    perror("Error renaming/moving file");
        else // renames file, and moves it if need be // fD[1], fD[0]
            if (rename(fD[1], fD[0]) != 0)
                perror("Error renaming/moving file");
    }
    else if(SFlag)
        mvS();
    else
    {
        // if (fDNr > 2)
        //     for (short i = 0; i < fDNr - 1; ++i) // fDNr - 1 because the last one will be the path // fD[i], fD[fDNr - 1]
        //         if (rename(fD[i], fD[fDNr - 1]) != 0) // renames file, and moves it if need be
        //             perror("Error renaming/moving file");
        // else // fD[0], fD[1]
        //     if (rename(fD[0], fD[1]) != 0) // renames file, and moves it if need be
        //         perror("Error renaming/moving file");
        mv(); // if you use this, comment out the other things in this else
    }
    
    return 0;
}