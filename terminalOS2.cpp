#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#define PATH_MAX 1024 // used to be 4096, because that is the upper limit for paths, but that is too much memory used for not much benefit
#define COMM_SIZE 101 // last char is for \0
#define MAX_WORDS 20
#define MAX_WORD_CHARS 51 // last char is for \0
#define COLOR_PROMPT "\033[0;92;1m" // prompt color - makes font bright green
#define COLOR_ERROR "\a\033[0;31;1m" // makes font red
#define COLOR_SUCCESS "\a\033[0;32;1m" // makes font green
#define COLOR_RESET "\033[0m" //resets font color

char historyFileName[] = "history.txt";

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

// void addHistory(char command[])
// {
//     // if the command is just an enter then don't add to the history
//     if (command[0] == '\0')
//         return;
//     // endHistoryFileLine += 1;
//     std::ofstream history(historyFileName, std::ios::app); // open the history file in append mode
//     history << command << '\n';
//     history.close();
// }

bool commandDecrypt(char command[])
{
    std::cout << "Command decrypt reached.\n";
    return 1;
}

int main(int argc, char *argv[])
{
    // this whole part of the code is just for the main messages mostly, pretty much everything important happens in commandDecrypt()
    // readline does filename completion by default so we tell readline to just insert the '\t'
    rl_bind_key('\t', rl_insert);
    std::cout << "\n--Welcome to my command line interpreter!--\n\n";
    if (argc == 2 && (argv[1] != "1" || argv[1] != "true"))
    {
        char* command;
        std::string prompt;
        prompt += COLOR_PROMPT;
        prompt += "<user> ";
        prompt += COLOR_RESET;
        while ((command = readline(prompt.c_str())) != nullptr)
        {
            if (strlen(command) > 0)
            {
                add_history(command);
            }
            std::cout << "Command is: '" << command << "'.\n";
            commandDecrypt(command);
            // readline will malloc command each time it is called
            free(command);
        }
    }
    else if (argc == 1 || (argc == 2 && (argv[1] != "0" || argv[1] != "false")))
    {
        char *command, currentPath[PATH_MAX];
        std::string prompt;
        // getcwd returns the current path
        if (getcwd(currentPath, sizeof(currentPath)) == NULL)
        {
            std::cout << "Error getting the current path.\n";
            std::cout << "\n\n--Thank you for using this interpreter!--\n\n";
            return 1;
        }
        prompt += COLOR_PROMPT;
        prompt += "<";
        prompt += currentPath;
        prompt += "> ";
        prompt += COLOR_RESET;
        while ((command = readline(prompt.c_str())) != nullptr)
        {
            if (strlen(command) > 0)
            {
                add_history(command);
            }
            std::cout << "Command is: '" << command << "'.\n";
            commandDecrypt(command);
            // readline will malloc command each time it is called
            free(command);
            // resetting the current path and updating the prompt in case they were changed
            if (getcwd(currentPath, sizeof(currentPath)) == NULL)
            {
                std::cout << "Error getting the current path.\n";
                std::cout << "\n\n--Thank you for using this interpreter!--\n\n";
                return 1;
            }
            prompt.clear();
            prompt += COLOR_PROMPT;
            prompt += "<";
            prompt += currentPath;
            prompt += "> ";
            prompt += COLOR_RESET;
        }
    }
    else 
    {
        std::cout << "Execution: exec <ARG>\n   exec - executable name\n    <ARG> - optional bool (0/1 or true/false) argument, which if true (1), enables user anonymity by switching the output to <user> when waiting user input, instead of <{currentPath}>";
    }
    std::cout << "\n--Thank you for using this interpreter!--\n\n";

    return 0;
}