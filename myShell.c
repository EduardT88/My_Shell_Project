#include "myShell.h"
#include "myFunction.h"
int main()
{
    welcome();
    while (1)
    {
        getLocation();
        char *input = getInputFromUser();
        puts(input);
        char **arguments = splitArgument(input);
        for (int i = 0; *(arguments + i) != NULL; i++)
        {
            puts(*(arguments + i));
            // puts(arguments[i]);
        }

        free(arguments);
        free(input);

        break;
    }
    return 0;
}

void printLineWithDelay(const char* line, int delay) {
    printf("%s\n", line);
    fflush(stdout);  // Ensure the line is printed immediately
    usleep(delay);  // Delay in microseconds
}

void printCastle(int offset) {
    for (int i = 0; i < offset; i++) {
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

void welcome() {
    for (int offset = 0; offset < 1; offset++) { // Adjust for desired movement range
        system("clear"); // Clear the console. Use "cls" on Windows.
        printCastle(offset); // Print the castle with offset
        printf("\033[1;32mRaise the portcullis with 'help'.\033[0m\n");
        // usleep(200000); // Delay between each castle drawing to control overall speed
    }
}