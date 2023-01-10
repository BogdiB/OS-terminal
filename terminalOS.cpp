// If someone tries to read/understand this code, God bless your soul, I'm sorry
#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
// #include <termios.h>
// #include <readline.h> // search this on google for arrow things
#define PATH_MAX 4096
#define COMM_SIZE 101 // last char is for \0
#define MAX_WORDS 20
#define MAX_WORD_CHARS 51 // last char is for \0

char historyFileName[] = "history.txt", currentPath[PATH_MAX], words[MAX_WORDS][MAX_WORD_CHARS];
short wordNr; // index when adding words, start from 0, after the word addition, it becomes the number, so it's +1 from the index
// int currentHistoryFileLine, endHistoryFileLine = 0; // 0 for these values means that it doesn't exist

void dirname() // uses words variable
{
    std::cout << "Dirname called.\n";
    // for (int i = 1; i < wordNr; ++i)
    // {
    //     // do what it's supposed to do
    //     ;
    // }
}

void help()
{
    // commands and things I've implemented
    std::cout << "\n";
    std::cout << "  Command line interpreter\n";
    std::cout << "\n";
    std::cout << "  One word commands (some can be called with multiple names, the names are separated by a '\\'):\n";
    std::cout << "      help - outputs this\n";
    std::cout << "      version - outputs the interpreter version number and the name of the author\n";
    std::cout << "      exit / close / stop - stops the interpreter\n";
    std::cout << "      history / readh / cath - outputs the whole history file\n";
    std::cout << "      deleteh / delh / clearh - deletes the history file\n";
    std::cout << "  Multiple word commands:\n";
    std::cout << "      dirname - strips the last component of the file name(s)\n";
    std::cout << "\n";
}

void version()
{
    // version info and author and whatever other "relevant information" I can think of
    std::cout << "\n";
    std::cout << "  Version: 1.0\n";
    std::cout << "  Author: Bogdan Birseuan\n";
    std::cout << "\n";
}

void deleteFile(char fileName[])
{
    // currentHistoryFileLine = 0;
    // endHistoryFileLine = 0;
    if (remove(fileName) == 0)
    {
        std::cout << "File '" << fileName << "' succesfully deleted.\n";
        return;
    }
    std::cout << "Error deleting file '" << fileName << "': No such file or directory.\n";
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
    // endHistoryFileLine += 1;
    std::ofstream history(historyFileName, std::ios::app); // open the history file in append mode
    history << command << '\n';
    history.close();
}

bool commandDecrypt()
{
    // don't delete or if you use arrow keys it goes into an endless loop
    // std::cin.clear();
    // std::cin.ignore();

    char initialCommand[COMM_SIZE], command[COMM_SIZE], c/*, AC[3]*/;
    short wordCharsNr[MAX_WORDS]; // they are indexes - start from 0
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
        wordNr = 0;
        for (short k = 0; k < MAX_WORDS; ++k)
            wordCharsNr[k] = 0;
        // removes starting whitespace
        char p;
        while((p = std::cin.peek()) == ' ')
            c = getc(stdin);
        //TODO: remove mid command and ending whitespace
        while((c = getc(stdin)) != '\n' && c > 0)
        {
            if (c == ' ')
            {
                // if ((p = std::cin.peek()) != ' ' && p != '\n')
                // {
                    words[wordNr][wordCharsNr[wordNr]] = '\0';
                    ++wordNr;
                // }
                // else
                // {
                //     while((p = std::cin.peek()) == ' ') // removes random whitespace
                //         c = getc(stdin);
                //     if ((p = std::cin.peek()) != '\n')
                //     {
                //         words[wordNr][wordCharsNr[wordNr]] = c;
                //         ++wordNr;
                //     }
                // }
            }
            else words[wordNr][wordCharsNr[wordNr]++] = c;
            // keep the initial command for keeping history
            initialCommand[i] = c;
            // make the command lowercase for the one word case
            command[i] = tolower(c);
            ++i;
            // these ifs are for validating the size of the input
            if (wordNr >= MAX_WORDS) // max is 19 when comparing, not 20, because it starts from 0
            {
                // clearing the stdin
                std::cin.clear();
                std::cin.ignore();
                std::cout << "Word limit per command reached, please only input" << MAX_WORDS << " words per command.\n";
                initialCommand[i] = '\0';
                command[i] = '\0';
                words[wordNr][wordCharsNr[wordNr]] = '\0';
                return true;
            }
            if (wordCharsNr[wordNr] >= MAX_WORD_CHARS)
            {
                // clearing the stdin
                std::cin.clear();
                std::cin.ignore();
                std::cout << "Character limit per word reached, please only input" << MAX_WORD_CHARS << " characters per word.\n";
                initialCommand[i] = '\0';
                command[i] = '\0';
                words[wordNr][wordCharsNr[wordNr]] = '\0';
                return true;
            }
            // (so that the number of characters taken doesn't go over the size limit (COMM_SIZE), and thus, cause an error)
            if (i >= COMM_SIZE) // leaves room for the \0 character as the last character
            {
                // clearing the stdin
                std::cin.clear();
                std::cin.ignore();
                std::cout << "The maximum number of characters you can input is " << COMM_SIZE - 1 << ".\n";
                initialCommand[i] = '\0';
                command[i] = '\0';
                words[wordNr][wordCharsNr[wordNr]] = '\0';
                return true;
            }
        }
        cmdlen = i;
        words[wordNr][wordCharsNr[wordNr]] = '\0';
        ++wordNr;
        initialCommand[i] = '\0';
        addHistory(initialCommand);
        command[i] = '\0';
    // }
    // if command is empty, just continue
    if (cmdlen == 0)
        return true;
    if (wordNr == 1)
    {
        // add "errors" when using the multi-word commands but only one word is present
        // add errors to every function, because each one must return a value
        if (strcmp(command, "exit") == 0 || strcmp(command, "close") == 0 || strcmp(command, "stop") == 0)
            return false;
        else if (strcmp(command, "history") == 0 || strcmp(command, "readh") == 0 || strcmp(command, "cath") == 0)
        {
            readFile(historyFileName);
            return true;
        }
        else if (strcmp(command, "deleteh") == 0 || strcmp(command, "delh") == 0 || strcmp(command, "clearh") == 0)
        {
            deleteFile(historyFileName);
            return true;
        }
        else if (strcmp(command, "help") == 0)
        {
            help();
            return true;
        }
        else if (strcmp(command, "version") == 0)
        {
            version();
            return true;
        }
        else if (strcmp(command, "dirname") == 0)
        {
            std::cout << "Wrong command execution: dirname <ARG> {<ARG>, <ARG>, ...}.\n    ARG - the path\n";
            return true;
        }
        else
        {
            std::cout << "Command '" << command << "' not found. Type 'help' to view commands.\n";
            return true;
        }
    }
    // while (words != NULL)
    // {
    //     words[wordNr++] = strtok(NULL, " ");
    // }
    // --wordNr;

    // delete from here
    std::cout << "Word nr: " << wordNr << '\n';
    std::cout << "Words: " << '\n';
    for (int k = 0; k < wordNr; ++k)
        std::cout << "Word " << k << ": " << words[k] << '\n';
    // to here

    if (strcmp(words[0], "dirname") == 0)
        dirname();
    else
        std::cout << "Command '" << words[0] << "' not found. Type 'help' to view commands.\n";

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
    std::cout << "\n--Welcome to my command line interpreter!--\n\n";
    if (argc == 2 && (argv[1] != "1" || argv[1] != "true"))
    {
        do
        {
            std::cout << "<user> ";
        }while(commandDecrypt());
    }
    else if (argc == 2 && (argv[1] != "0" || argv[1] != "false"))
    {
        std::cout << "Execution: exec <ARG>\n   exec - executable name\n    <ARG> - optional bool (0/1 or true/false) argument, which if true (1), enables user anonymity by switching the output to <user> when waiting user input, instead of <{currentPath}>";
    }
    else
    {
        do
        {
            // getcwd returns the current path
            if (getcwd(currentPath, sizeof(currentPath)) == NULL)
            {
                std::cout << "Error getting the current path.\n";
                std::cout << "\n\n--Thank you for using this interpreter!--\n\n";
                return 1;
            }
            std::cout << "<" << currentPath << "> ";
        }while(commandDecrypt());
    }
    std::cout << "\n--Thank you for using this interpreter!--\n\n";

    return 0;
}