#include "myShell.h"
#include "myFunction.h"

int main()
{
    welcome();
    while (1)
    {
        getLocation();
        char *input = getInputFromUser();
        if (strcmp(input, "exit") == 0 || strncmp(input, "exit ", 5) == 0)
            logout(input);

        if (strchr(input, '|') != NULL)
        {
            // If input contains '|', we assume it's a pipe command
            char **argv1, **argv2;
            splitInputForPipe(input, &argv1, &argv2);

            // Execute the piped commands
            mypipe(argv1, argv2);

            // Wait for all child processes to finish
            int status;
            while (wait(&status) > 0)
                ;

            // Free the memory allocated for the arguments
            for (int i = 0; argv1[i] != NULL; i++)
            {
                free(argv1[i]);
            }
            for (int i = 0; argv2[i] != NULL; i++)
            {
                free(argv2[i]);
            }
            free(argv1);
            free(argv2);
        }
        else
        {
            // Process other commands if no pipe is found
            char **arguments = splitArgument(input);

            if (strcmp(input, "echo") == 0)
                echo(arguments);
            else if (strcmp(input, "cd") == 0)
                cd(arguments);
            else if (strcmp(input, "cp") == 0)
                cp(arguments);
            else if (strcmp(input, "delete") == 0)
                delete (arguments);
            else if (strcmp(input, "move") == 0)
                move(arguments);
            else if (strcmp(input, "cat") == 0)
                echoppend(arguments);
            else if (strcmp(input, "wrt") == 0)
                echowrite(arguments);
            else if (strcmp(input, "rd") == 0)
                rd(arguments);
            else if (strcmp(input, "wc") == 0)
                wordCount(arguments);

            free(arguments);
        }
        free(input);
    }
    return 0;
}

void printLineWithDelay(const char *line, int delay)
{
    printf("%s\n", line);
    fflush(stdout); // Ensure the line is printed immediately
    usleep(delay);  // Delay in microseconds
}

void printCastle(int offset)
{
    for (int i = 0; i < offset; i++)
    {
        printLineWithDelay("", 50000); // Simulate vertical movement with empty lines
    }

    puts("\033[1;36m"); // Cyan color for the drawing
    printLineWithDelay("        |>>>                    |>>>", 50000);
    printLineWithDelay("        |                        |", 50000);
    printLineWithDelay("    _  _|_  _   Welcome to       _  _|_  _", 50000);
    printLineWithDelay("   |;|_|;|_|;|  Eduard's Castle  |;|_|;|_|;|", 50000);
    printLineWithDelay("   \\\\..      /  of Code          \\\\.    ./", 50000);
    printLineWithDelay("    \\\\.  ,  /                     \\\\.  ,  /", 50000);
    printLineWithDelay("     ||:   |                       ||:   |", 50000);
    printLineWithDelay("     ||:.  |                       ||:.  |", 50000);
    printLineWithDelay("     ||:  .|                       ||:  .|", 50000);
    printLineWithDelay("     ||:   |       \\,\\             ||:   |         ,/", 50000);
    printLineWithDelay("     ||: , |            /`\\        ||: , |            /`\\", 50000);
    printLineWithDelay("     ||:   |            .'.        ||:   |", 50000);
    printLineWithDelay("     ||: . |          /`   `\\      ||: . |          /`   `\\", 50000);
    puts("\033[0m"); // Reset text color
}

void welcome()
{
    for (int offset = 0; offset < 1; offset++)
    {                        // Adjust for desired movement range
        system("clear");     // Clear the console. Use "cls" on Windows.
        printCastle(offset); // Print the castle with offset
        printf("\033[1;32mRaise the portcullis with 'help'.\033[0m\n");
        // usleep(200000); // Delay between each castle drawing to control overall speed
    }
}