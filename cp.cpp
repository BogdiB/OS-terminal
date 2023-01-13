#include <iostream>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>

#include <stdlib.h>
#include <errno.h>
#include <libgen.h>

#define COLOR_ERROR "\a\033[0;31;1m" // makes font red
#define COLOR_RESET "\033[0m" //resets font color

char* fD[50];

void cp_r(char *source, char *dest)
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
                    cp_r(sourcePath, destPath);
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

int cp_t(char *location, int fdCount)
{
    // target cp
    for(short i = 0; i < fdCount - 1; ++i)
    {
        char fileLocation[512], *buffer;
        int fd1, fd2, length, n;

        strcpy(fileLocation,location);
        strcat(fileLocation,"/");
        strcat(fileLocation, fD[i]);

        if((fd1 = open(fD[i], O_RDONLY)) < 0)
        {
            perror("Error opening the input file\n");
            return 1;
        }
        if((fd2 = open(fileLocation, O_WRONLY | O_CREAT, 0664)) < 0)
        {
            perror("Error opening output file\n");
            return 2;
        }
        length = lseek(fd1, 0, SEEK_END) - lseek(fd1, 0, SEEK_SET);
        buffer = (char *) malloc(length * sizeof(char));

        if(buffer == NULL)
        {
            perror("Error null buffer");
            return 3;
        }
        // reading the file
        while((n = read(fd1, buffer, length)) > 0)
        {
            buffer[n] = '\0';
            write(fd2, buffer, length);
        }
        if(n < 0)
        {
            perror("Error reading the file\n");
            return 4;
        }
        free(buffer);
        close(fd1);
        close(fd2);
    }
}

int cp()
{
    // the basic cp functionality
    int fd1,fd2, n, length;
    char* buffer;

    if((fd1 = open(fD[0], O_RDONLY)) < 0)
    {
        perror("Error opening the input file\n");
        return 2;
    }
    if((fd2 = open(fD[1],  O_WRONLY | O_CREAT ,0664)) < 0)
    {
        perror("Error opening the output file\n");
        return 2;
    }

    length = lseek (fd1, 0, SEEK_END) - lseek(fd1, 0, SEEK_SET);
    buffer = (char *) malloc(length * sizeof(char));

    if(buffer == NULL)
    {
        perror("Malloc failed.");
        return 3;
    }
    // basic file reading and writing
    while( (n = read(fd1, buffer, length)) > 0)
    {
        buffer[n] = '\0';
        write(fd2,buffer,length);
    }
    if( n < 0)
    {
        perror("Error reading the file\n");
        return 4;
    }
    free(buffer);
    close(fd1);
    close(fd2);
}

int main(int argc, char *argv[])
{
    // COMPILE EVERY CHANGE IN THE SAME DIRECTORY WITH 'terminalOS2.cpp' with: g++ -o cp cp.cpp
    // argv only consists of things after the dirname command in terminalOS2

    // counting the number of file descriptors
    short fDNr = 0;
    for(short i = 1; i < argc; ++i)
        for(short j = 0; j < strlen(argv[i]); ++j)
        {
            if(argv[i][j] == '-')
                break;
            else if(j == strlen(argv[i]) - 1)
                fD[fDNr++] = argv[i];
        }

    // assinging true to the flags given in the arguments
    short c;
    bool iFlag = false, rFlag = false, tFlag = false, vFlag = false, hFlag = false, noFlags = true;
    char tDir[30];
    while((c = getopt(argc, argv, "irRt:v")) != -1)
    {
        switch(c)
        {
            case 'i':
                // shows prompt
                noFlags = false;
                iFlag = 1;
                break;
            case 'r':
                // recursive
                noFlags = false;
                rFlag = 1;
                break;
            case 'R':
                // same as 'r'
                noFlags = false;
                rFlag = 1;
                break;
            case 't':
                // target directory, copies all args in the directory
                noFlags = false;
                tFlag = 1;
                strcpy(tDir, optarg);
                // delete the cout after testing
                std::cout << "tDir: " << tDir << '\n';
                break;
            case 'v':
                // verbose - explains what it's doing
                noFlags = false;
                vFlag = 1;
                break;
            default:
                std::cout << COLOR_ERROR << "Flag '" << c << "' not found." << COLOR_RESET;
                break;
        }
    }
    // checking the flags after assignment
    if (iFlag == true)
    {
        // prompt for overwriting, if no, then don't copy
        std::cout << "Overwrite '" << fD[1] << "'?(y/n): ";
        char yn = getc(stdin);
        // any other input besides 'y' and 'Y' gets treated as no
        if (yn == 'y' || yn == 'Y');
            cp();
        std::cin.clear();
        std::cin.ignore();
    }
    else if (vFlag == true)
    {
        // "explaining" what it's doing
        std::cout << "'" << fD[0] << "' -> '" << fD[1] << "'\n";
        cp();
    }
    else if (tFlag == true)
        cp_t(tDir, fDNr);
    else if (rFlag == true)
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
        cp_r(fD[0], fD[1]);
    }
    else if (noFlags = true)
        cp();
    
    return 0;
}