#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>

#define PATH_MAX 1024 // used to be 4096, because that is the upper limit for paths, but that is too much memory used for not much benefit
#define MAX_WORDS 20
#define MAX_WORD_CHARS 51 // last char is for \0

#define COLOR_PROMPT "\033[0;92;1m" // prompt color - makes font bright green
#define COLOR_ERROR "\a\033[0;31;1m" // makes font red
#define COLOR_SUCCESS "\a\033[0;32;1m" // makes font green
#define COLOR_RESET "\033[0m" //resets font color

char historyFileName[] = "history.txt"; // this is global just so I can change the name easier
char words[MAX_WORDS][MAX_WORD_CHARS], currentPath[PATH_MAX];
short wordNr; // index when adding words, start from 0, after the word addition, it becomes the number, so it's +1 from the index
short wordCharsNr[MAX_WORDS]; // they are indexes - start from 0

void cp(char *args[])
{
    int id = fork();
    if (id < 0)
    {
        // std::cout << COLOR_ERROR << "Error creating a process.\n" << COLOR_RESET;
        perror("Process creation error");
        return;
    }
    if (id == 0)
    {
        // child process
        std::string dirpath = currentPath;
        dirpath += "/cp";
        if (execvp(dirpath.c_str(), args) == -1)
            perror("Exec error");
        exit(0);
    }
    else
    {
        // main process
        wait(NULL);
    }
}

void mv(char *args[])
{
    int id = fork();
    if (id < 0)
    {
        // std::cout << COLOR_ERROR << "Error creating a process.\n" << COLOR_RESET;
        perror("Process creation error");
        return;
    }
    if (id == 0)
    {
        // child process
        std::string dirpath = currentPath;
        dirpath += "/dirname";
        if (execvp(dirpath.c_str(), args) == -1)
            perror("Exec error");
        exit(0);
    }
    else
    {
        // main process
        wait(NULL);
    }
}

void dirname(char *args[])
{
    int id = fork();
    if (id < 0)
    {
        // std::cout << COLOR_ERROR << "Error creating a process.\n" << COLOR_RESET;
        perror("Process creation error");
        return;
    }
    if (id == 0)
    {
        // child process
        std::string dirpath = currentPath;
        dirpath += "/dirname";
        if (execvp(dirpath.c_str(), args) == -1)
            perror("Exec error");
        exit(0);
    }
    else
    {
        // main process
        wait(NULL);
    }
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
    // std::cout << "      history / readh / cath - outputs the whole history file\n";
    // std::cout << "      deleteh / delh / clearh - deletes the history file\n";
    std::cout << "  Multiple word commands(only re-implemented):\n";
    std::cout << "      cp <> <>\n";
    std::cout << "      mv <> <>\n";
    std::cout << "      dirname <path> {<path>, ...} - strips the last component of the file name(s)\n";
    std::cout << "  Legend:\n";
    std::cout << "      <-> - flag\n";
    std::cout << "      <ARG> - argument that MUST be given (contains the name of what you should give)\n";
    std::cout << "      {<ARG>} - optional argument (only one accepted)\n";
    std::cout << "      {<ARG>, ...} - optional argument(s)\n";
    std::cout << "\n";
}

void version()
{
    // version info and author and whatever other "relevant information" I can think of
    std::cout << "\n";
    std::cout << "  Version: 2.0 (it's 2.0 because I re-started the project from scratch at one point)\n";
    std::cout << "  Author: Bogdan Birseuan\n";
    std::cout << "\n";
}

// void addHistory(char command[])
// {
//     // if the command is just an enter then don't add to the history
//     if (command[0] == '\0')
//         return;
//     std::ofstream history(historyFileName, std::ios::app); // open the history file in append mode
//     history << command << '\n';
//     history.close();
// }

bool commandDecrypt(char initialCommand[])
{
    // clearing wordNr and wordCharsNr so that they don't remain the same from the last command and mess things up
    wordNr = 0;
    for (short i = 0; i < MAX_WORDS; ++i)
        wordCharsNr[i] = 0;
    // splitting the initial command on spaces in the variable words
    for (short i = 0; i < strlen(initialCommand); ++i)
    {
        if (initialCommand[i] != ' ')
            words[wordNr][wordCharsNr[wordNr]++] = initialCommand[i];
        else
            words[wordNr][wordCharsNr[wordNr++]] = '\0';
        if (wordNr >= MAX_WORDS) // max is 19 when comparing, not 20, because it starts from 0
        {
            std::cout << COLOR_ERROR << "Word limit per command reached, please only input " << MAX_WORDS << " words per command.\n" << COLOR_RESET;
            words[wordNr][wordCharsNr[wordNr]] = '\0';
            return true;
        }
        if (wordCharsNr[wordNr] >= MAX_WORD_CHARS)
        {
            std::cout << COLOR_ERROR << "Character limit per word reached, please only input " << MAX_WORD_CHARS << " characters per word.\n" << COLOR_RESET;
            words[wordNr][wordCharsNr[wordNr]] = '\0';
            return true;
        }
    }
    words[wordNr][wordCharsNr[wordNr++]] = '\0';

    // add "errors" when using the multi-word commands but only one word is present
    // add errors to every function, because each one must return a value
    bool ok = false;
    if (strcmp(words[0], "exit") == 0 || strcmp(words[0], "close") == 0 || strcmp(words[0], "stop") == 0)
        return false;
    // else if (strcmp(words[0], "history") == 0 || strcmp(words[0], "readh") == 0 || strcmp(words[0], "cath") == 0)
    // {
    //     readFile(historyFileName);
    //     return true;
    // }
    // else if (strcmp(words[0], "deleteh") == 0 || strcmp(words[0], "delh") == 0 || strcmp(words[0], "clearh") == 0)
    // {
    //     deleteFile(historyFileName);
    //     return true;
    // }
    else if (strcmp(words[0], "help") == 0)
    {
        help();
        return true;
    }
    else if (strcmp(words[0], "version") == 0)
    {
        version();
        return true;
    }
    else
    {
        // multiple word commands
        // making args (in this case the arguments have to start from index 1 in args, that's why we give words[0] to args only here, even though it is redundant)
        char *args[20];
        for (short i = 0; i < wordNr; ++i)
            *(args + i) = words[i];
        *(args + wordNr) = nullptr;
        // searching which command it is
        if (strcmp(words[0], "cp") == 0)
        {
            cp(args);
            return true;
        }
        else if (strcmp(words[0], "mv") == 0)
        {
            mv(args);
            return true;
        }
        else if (strcmp(words[0], "dirname") == 0)
        {
            dirname(args);
            return true;
        }
        else
        {
            int id = fork();
            if (id < 0)
            {
                // std::cout << COLOR_ERROR << "Error creating a process.\n" << COLOR_RESET;
                perror("Process creation error");
                return true;
            }
            if (id == 0)
            {
                // child process
                std::string path = "/bin/";
                path += words[0];
                if (execvp(path.c_str(), args) == -1)
                {
                    perror("Exec error");
                }
                return 1;
            }
            else
            {
                // main process
                wait(NULL);
            }
        }
    }
    if (ok == false)
        std::cout << "Command '" << words[0] << "' not found. Type 'help' to view commands.\n";

    return 1;
}

int main(int argc, char *argv[])
{
    // this whole part of the code is just for the main messages mostly, pretty much everything important happens in commandDecrypt()
    // readline does filename completion by default so we tell readline to just insert the '\t'
    rl_bind_key('\t', rl_insert);
    system("clear"); // I'm assuming I can use system for something this trivial, and this is the only time it is used in the program
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
                if (commandDecrypt(command) == false)
                {
                    free(command);
                    std::cout << "\n--Thank you for using this interpreter!--\n\n";
                    return 0;
                }
            }
            // readline will malloc command each time it is called
            free(command);
        }
    }
    else if (argc == 1 || (argc == 2 && (argv[1] != "0" || argv[1] != "false")))
    {
        char *command;
        std::string prompt;
        // getcwd returns the current path
        if (getcwd(currentPath, sizeof(currentPath)) == NULL)
        {
            // std::cout << COLOR_ERROR << "Error getting the current path.\n" << COLOR_RESET;
            perror("Error with the current path");
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
                if (commandDecrypt(command) == false)
                {
                    free(command);
                    std::cout << "\n--Thank you for using this interpreter!--\n\n";
                    return 0;
                }
            }
            // readline will malloc command each time it is called
            free(command);
            // resetting the current path and updating the prompt in case they were changed
            if (getcwd(currentPath, sizeof(currentPath)) == NULL)
            {
                // std::cout << COLOR_ERROR << "Error getting the current path.\n" << COLOR_RESET;
                perror("Error with the current path");
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
        std::cout << COLOR_ERROR << "Execution: exec <ARG>\n   exec - executable name\n    <ARG> - optional bool (0/1 or true/false) argument, which if true (1), enables user anonymity by switching the output to <user> when waiting user input, instead of <{currentPath}>\n" << COLOR_RESET;

    return 0;
}