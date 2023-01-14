#include <iostream>
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

bool iFlag = false, rFlag = false, tFlag = false, vFlag = false, hFlag = false;
char *fD[50], tDir[30];
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

void cpr(char *source, char *dest)
{
    // recursive cp
    // remember to close dir, delete this comment after
    DIR *dir;
    struct dirent *ent;
    struct stat st;
    char sourcePath[FILENAME_MAX];
    char destPath[FILENAME_MAX];

    if ((dir = opendir(source)) != NULL)
    {
        // create the destination directory
        // if the error isn't that it already exists, output the error
        if (mkdir(dest, 0777) != 0)
            if (errno != EEXIST)
            {
                perror("Error at making the directory");
                exit(0);
            }

        // copying files and subdirectories
        while ((ent = readdir(dir)) != NULL)
        {
            // if it encounters "." or "..", just continue
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
                continue;

            snprintf(sourcePath, FILENAME_MAX, "%s/%s", source, ent->d_name);
            snprintf(destPath, FILENAME_MAX, "%s/%s", dest, ent->d_name);

            if (stat(sourcePath, &st) == 0)
            {
                // if it's a directory, do recursive cp
                if (S_ISDIR(st.st_mode))
                    cpr(sourcePath, destPath);
                else
                {
                    // copying file
                    FILE *sourceF = fopen(sourcePath, "rb");
                    FILE *destF = fopen(destPath, "wb");
                    size_t bytesRead;
                    char buffer[4096];

                    while ((bytesRead = fread(buffer, 1, sizeof buffer, sourceF)) > 0)
                        fwrite(buffer, 1, bytesRead, destF);

                    fclose(sourceF);
                    fclose(destF);
                }
            }
        }
        closedir(dir);
    }
}

int cpt(char *loc)
{
    // target cp - almost the same as normal cp
    for(short i = 0; i < fDNr - 1; ++i)
    {
        char fLoc[512];

        strcpy(fLoc, loc);
        strcat(fLoc, "/");
        strcat(fLoc, fD[i]);

        // opening input/output file error management
        int fD1, fD2;
        if((fD1 = open(fD[i], O_RDONLY)) < 0)
        {
            perror("Error opening the input file\n");
            return 1;
        }
        if((fD2 = open(fLoc, O_WRONLY | O_CREAT, 0664)) < 0)
        {
            perror("Error opening output file\n");
            return 2;
        }

        int n, length = lseek(fD1, 0, SEEK_END) - lseek(fD1, 0, SEEK_SET);
        char *buffer = (char *) malloc(length * sizeof(char));

        if(buffer == NULL)
        {
            perror("Error, null buffer");
            return 3;
        }
        // basic file reading and writing
        while((n = read(fD1, buffer, length)) > 0)
        {
            buffer[n] = '\0';
            write(fD2, buffer, length);
        }
        if(n < 0)
        {
            perror("Error reading the file\n");
            return 4;
        }

        free(buffer);
        close(fD1);
        close(fD2);
    }
    return 0;
}

int cp()
{
    // the basic cp functionality
    // opening input/output file error management
    int fD1, fD2;
    if((fD1 = open(fD[0], O_RDONLY)) < 0)
    {
        perror("Error opening the input file");
        return 2;
    }
    if((fD2 = open(fD[1],  O_WRONLY | O_CREAT ,0664)) < 0)
    {
        perror("Error opening the output file");
        return 2;
    }

    int n, length = lseek (fD1, 0, SEEK_END) - lseek(fD1, 0, SEEK_SET); // gets the file length so 
    char* buffer = (char *) malloc(length * sizeof(char));

    if(buffer == NULL)
    {
        perror("Error, null buffer");
        return 3;
    }
    // basic file reading and writing
    while( (n = read(fD1, buffer, length)) > 0)
    {
        buffer[n] = '\0';
        write(fD2,buffer,length);
    }
    if( n < 0)
    {
        perror("Error reading the file");
        return 4;
    }

    free(buffer);
    close(fD1);
    close(fD2);

    return 0;
}

int main(int argc, char *argv[])
{
    // COMPILE EVERY CHANGE IN THE SAME DIRECTORY WITH 'terminalOS2.cpp' with: g++ -o cp cp.cpp

    // keeping everything except argv[0] and flags in fD for later use
    for(short i = 1; i < argc; ++i)
    {
        // std::cout << argv[i] << '\n';
        if (argv[i][0] != '-') // if the first character in this argument is '-', that means it's a flag, so we skip over it
        {
            *(fD + i - 1) = argv[i];
            // std::cout << fD[i] << '\n'; // testing
            ++fDNr;
        }
    }

    short c;
    // assinging true to the flags given in the arguments
    while((c = getopt(argc, argv, "irRt:v")) != -1)
    {
        switch(c)
        {
            case 'i':
                // shows prompt
                iFlag = 1;
                break;
            case 'r':
                // recursive
                rFlag = 1;
                break;
            case 'R':
                // same as 'r'
                rFlag = 1;
                break;
            case 't':
                // target directory, copies all args in the directory
                tFlag = 1;
                strcpy(tDir, optarg);
                // delete the cout after testing
                std::cout << "tDir: " << tDir << '\n';
                break;
            case 'v':
                // verbose - "explains" what it's doing
                vFlag = 1;
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
    if (vFlag)
    {
        // "explaining" what it's doing
        std::cout << "'" << fD[0] << "' -> '" << fD[1] << "'\n";
    }
    if (tFlag)
        cpt(tDir);
    else if (rFlag)
    {
        DIR *dir;
        struct dirent *ent;
        struct stat st;
        char destPath[FILENAME_MAX];

        if ((dir = opendir(fD[0])) != NULL)
        {
            // creating the destination directory
            char *dirName = basename(fD[0]);
            snprintf(destPath, FILENAME_MAX, "%s/%s", fD[1], dirName); // works like printf, but doesn't output, instead it stores it in the buffer
            strcpy(fD[1], destPath);
            if (mkdir(destPath, 0777) != 0)
            {
                if (errno != EEXIST)
                {
                    perror("Error creating directory");
                    exit(1);
                }
            }
        }
        cpr(fD[0], fD[1]);
    }
    else
        cp();
    
    return 0;
}