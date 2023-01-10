#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
// #include <termios.h>
// #include <readline.h> // search this on google for arrow things
#define PATH_MAX 4096
#define COMM_SIZE 100

char historyFileName[] = "history.txt", currentPath[PATH_MAX];
int currentHistoryFileLine, endHistoryFileLine = 0; // 0 for these values means that it doesn't exist

bool deleteFile(char fileName[])
{
    currentHistoryFileLine = 0;
    endHistoryFileLine = 0;
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
    endHistoryFileLine += 1;
    std::ofstream history(historyFileName, std::ios::app); // open the history file in append mode
    history << command << '\n';
    history.close();
}

bool commandDecrypt()
{
    // don't delete or if you use arrow keys it goes into an endless loop
    // std::cin.clear();
    // std::cin.ignore();

    char initialCommand[COMM_SIZE], command[COMM_SIZE], c/*AC[3]*/;
    short i = 0, cmdlen;

    // TODO: if I touch this again before I finish the commands beat me
    // AC[0] = std::cin.peek();
    // // if (AC[0] == 27 && AC[1] == 91)
    // if (AC[0] == 27)
    // {
    //     AC[0] = getc(stdin);
    //     AC[1] = std::cin.peek();
    //     if (AC[1] == 91)
    //     {
    //         AC[1] = getc(stdin);
    //         AC[2] = getc(stdin);
    //         if (AC[2] == 65) // A in ascii, up arrow
    //         {
    //             if (getcwd(currentPath, sizeof(currentPath)) == NULL)
    //             {
    //                 std::cout << "Error getting the current path.\n";
    //                 std::cout << "\n\n--Thank you for using this terminal!--\n\n";
    //                 return 0;
    //             }
    //             std::cout << "\x1bM<" << currentPath << ">  \x1b[K";
    //         }
    //         else if (AC[2] == 66) // B in ascii, down arrow
    //         {
    //             std::cout << "\x9bK\r";
    //         }
    //         else
    //         {
    //             std::cout << "Unsuported behaviour.\n";
    //             std::cin.clear();
    //             std::cin.ignore();
    //             return true;
    //         }
    //     }
    //     else if ((AC[1] = getc(stdin)) == 94 && (AC[2] = getc(stdin)) == 63)
    //     {
    //         // backspace character
    //         std::cout << "\x1b[#D";
    //     }
    //     else
    //         {
    //             std::cout << "Unsuported behaviour.\n";
    //             std::cin.clear();
    //             std::cin.ignore();
    //             return true;
    //         }
    //     // std::cout << "\x1b[2K\r";
    //     // std::cout << "\b \b";
    //     std::cin.clear();
    //     std::cin.ignore();
    // }
    // if (strcmp())
    //     ;
    // checking for arrow keys
    // char t[3];
    // t[0] = std::cin.peek();
    // if (t[0] == 27)
    // {
    //     // erasing the next 3 chars (arrow key) through \e
    //     std::cout << "\x1b[2K\r"; // \x1b is ESC in ANSI - https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797 - https://en.wikipedia.org/wiki/ANSI_escape_code#Colors
    //     // c = getc(stdin); // ESC character (27 ascii, checked in the first if)
    //     // std::cout << "\r\e";
    //     // c = getc(stdin); // '[' char
    //     // std::cout << "\r\e";
    //     // c = getc(stdin); // A (65 ascii) for up, B (66 ascii) for down - checked in the next 2 ifs
        
    //     t[1] = std::cin.peek();
    //     t[2] = std::cin.peek();
    //     if (t[2] == 65)
    //     {
    //         // up arrow key
    //         std::cout << "up key\n"; // erase cout up key after things are done
    //         /* int x;
    //         std::cin >> x;
    //         std::cout << x << '\n'; */
    //         // get command[] and put in history
    //         // return after everything done
    //     }
    //     else if (t[2] == 66)
    //     {
    //         // down arrow key
    //         std::cout << "down key\n"; // erase this after things are done
    //         // get command[] and put in history
    //         // return after everything done
    //     }
    // }
    // else
    // {
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
    // }
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
    // // sets stdin to non-canonical input mode, so I can read things without end of line or EOF
    // struct termios old_tio, new_tio;
    // tcgetattr(STDIN_FILENO, &old_tio);
    // new_tio = old_tio;
    // new_tio.c_lflag &= (~ICANON);
    // tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
    
    // this whole part of the code is just for the main messages, pretty much everything important happens in commandDecrypt()
    std::cout << "\n--Welcome to my terminal!--\n\n";
    do
    {
        // getcwd returns the current path
        if (getcwd(currentPath, sizeof(currentPath)) == NULL)
        {
            std::cout << "Error getting the current path.\n";
            std::cout << "\n\n--Thank you for using this terminal!--\n\n";
            return 1;
        }
        std::cout << "<" << currentPath << "> ";
    }while(commandDecrypt());
    std::cout << "\n--Thank you for using this terminal!--\n\n";

    return 0;
}