#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <termios.h>
#define PATH_MAX 4096
#define COMM_SIZE 100

char historyFileName[] = "history.txt";
int historyFileLine;

bool deleteFile(char fileName[])
{
    if (remove(fileName) == 0)
    {
        std::cout << "File '" << fileName << "' succesfully deleted.\n";
        return true;
    }
    std::cout << "Error deleting file '" << fileName << "': No such file or directory.\n";
    return false;
}

void readFile(char fileName[])
{
    char text[1024];
    std::ifstream file(fileName);
    while(file.getline(text, 1024))
        std::cout << text << '\n';
    file.close();
}

void addHistory(char command[])
{
    // if the command is just an enter then don't add to the history
    if (command[0] == '\0')
        return;
    std::ofstream history(historyFileName, std::ios::app); // open the history file in append mode
    history << command << '\n';
    history.close();
}

bool commandDecrypt()
{
    char initialCommand[COMM_SIZE], command[COMM_SIZE], c;
    short i = 0, cmdlen;
    // checking for arrow keys
    c = std::cin.peek();
    if (c != EOF && c == 27)
    {
        c = getc(stdin); // special char (0 or 224 ascii)
        c = getc(stdin); // '[' char
        c = getc(stdin); // A (65 ascii) for up, B (66 ascii) for down
        // probably shouldn't delete the cin clear and ignore for reasons I forgot to write down when making this
        std::cin.clear();
        std::cin.ignore();
        if (c == 65)
        {
            // up arrow key
            std::cout << "up key\n" << '\e'; // erase this after things are done
            /* int x;
            std::cin >> x;
            std::cout << x << '\n'; */
            // get command[] and put in history
        }
        else if (c == 66)
        {
            // down arrow key
            std::cout << "down key\n"; // erase this after things are done
            // get command[] and put in history
        }
        return true;
    }
    else
    {
        // constructing the command strings
        while((c = getc(stdin)) != '\n' && c > 0)
        {
            // keep the initial command for keeping history
            initialCommand[i] = c;
            // make the command lowercase
            command[i] = tolower(c);
            ++i;
            // this if is for validating the size of the input
            // (so that the number of characters taken doesn't go over the size limit (COMM_SIZE), and thus, cause an error)
            if (i >= COMM_SIZE) // leaves room for the \0 character as the last character
            {
                // clearing the stdin
                std::cin.clear();
                std::cin.ignore();
                std::cout << "The maximum number of characters you can input is " << COMM_SIZE - 1 << ".\n";
                initialCommand[i] = '\0';
                command[i] = '\0';
                return 1;
            }
        }
        cmdlen = i;
        initialCommand[i] = '\0';
        addHistory(initialCommand);
        command[i] = '\0';
    }
    // TODO: ADD initialCommand to history
    // if command is empty, just continue
    if (cmdlen == 0)
        return true;
    // check if the command for exiting/closing/stopping the terminal
    if (strcmp(command, "exit") == 0 || strcmp(command, "close") == 0 || strcmp(command, "stop") == 0)
        return false;
    else if (strcmp(command, "history") == 0 || strcmp(command, "readh") == 0 || strcmp(command, "cath") == 0)
        readFile(historyFileName);
    else if (strcmp(command, "deleteh") == 0 || strcmp(command, "delh") == 0 || strcmp(command, "clearh") == 0)
        deleteFile(historyFileName);
    else std::cout << "Command '" << command << "' not found.\n";

    return true;
}

int main(int argc, char **argv)
{

    // this whole part of the code is just for the main messages, pretty much everything important happens in commandDecrypt()
    char currentPath[PATH_MAX];

    std::cout << "\n--Welcome to my terminal!--\n\n";
    do
    {
        // getcwd returns the current path
        if (getcwd(currentPath, sizeof(currentPath)) == NULL)
        {
            perror("Error getting the current path.\n");
            std::cout << "\n\n--Thank you for using this terminal!--\n\n";
            return 1;
        }
        std::cout << "<" << currentPath << "> ";
    }while(commandDecrypt());
    std::cout << "\n--Thank you for using this terminal!--\n\n";

    return 0;
}