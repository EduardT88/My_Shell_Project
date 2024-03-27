#include "myFunction.h"

char *my_strtok(char *str, const char *delim)
{
    static char *lastToken = NULL; // Maintain the context of string between successive calls

    if (str == NULL)
    {
        str = lastToken;
    }

    // Skip leading delimiters
    while (*str && strchr(delim, *str))
    {
        ++str;
    }

    // If we've reached the end of the string, return NULL
    if (!*str)
    {
        lastToken = NULL;
        return NULL;
    }

    // Mark the beginning of the token
    char *start = str;

    // Move to the end of the token
    while (*str && !strchr(delim, *str))
    {
        ++str;
    }

    if (*str)
    {
        *str++ = '\0'; // Null-terminate the token and move to the next character
    }

    lastToken = str;

    return start; // Return the token
}

void getLocation()
{
    char cwd[SIZE_BUFF]; // Buffer for current working directory
    char hostname[256];  // Buffer for hostname

    // Get current working directory
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        puts("Error getting current working directory\n");
        return;
    }

    // Get hostname
    if (gethostname(hostname, sizeof(hostname)) == -1)
    {
        puts("Error getting hostname\n");
        return;
    }

    // Print the hostname in another color, for example red (\033[0;31m)
    printf("\033[0;31m%s\033[0m", hostname);

    // Print the separator, cwd, and '$' sign in a different color, for example blue (\033[0;34m)
    printf("\033[0;34m:%s$\033[0m ", cwd);
    fflush(stdout); // Ensure that the output is displayed before reading input
}

char *getInputFromUser()
{
    char ch;
    int size = 1;
    int index = 0;
    char *str = (char *)malloc(size * sizeof(char));
    while ((ch = getchar()) != '\n')
    {
        *(str + index) = ch;
        size++;
        index++;
        str = (char *)realloc(str, size);
    }
    *(str + index) = '\0';
    return str;
}

char **splitArgument(char *str)
{
    int size = 1, index = 0;
    char **arguments = (char **)malloc(size * sizeof(char *));
    char *token = str;
    char *end = str;
    int inQuotes = 0;

    while (*end)
    {
        if (*end == '\"')
        {
            inQuotes = !inQuotes;
        }
        else if (*end == ' ' && !inQuotes)
        {
            if (token != end)
            {
                *end = '\0';                // Null-terminate the token
                arguments[index++] = token; // Add the token to the arguments array
                size++;
                arguments = (char **)realloc(arguments, size * sizeof(char *));
            }
            token = end + 1; // Start of a new token
        }
        end++;
    }

    if (token != end)
    {
        arguments[index++] = token; // Add the last token to the arguments array
        size++;
        arguments = (char **)realloc(arguments, size * sizeof(char *));
    }

    arguments[index] = NULL; // Null-terminate the arguments array

    // Remove quotes from arguments if present
    for (int i = 0; i < index; i++)
    {
        if (arguments[i][0] == '\"')
        {
            // Remove the leading quote
            arguments[i]++;
            // Remove the trailing quote if present
            int len = strlen(arguments[i]);
            if (arguments[i][len - 1] == '\"')
            {
                arguments[i][len - 1] = '\0';
            }
        }
    }

    return arguments;
}

void logout(char *input)
{
    free(input);
    puts("logout");
    exit(EXIT_SUCCESS); // EXIT_SUCCESS = 0
}

void echo(char **arguments)
{

    while (*(++arguments))
        printf("%s ", *arguments);

    puts("");
}

void cp(char **arguments)
{
    char ch;
    FILE *src, *des;
    if ((src = fopen(arguments[1], "r")) == NULL)
    {
        puts("error");
        return;
    }

    if ((des = fopen(arguments[2], "w")) == NULL)
    {
        puts("error");
        fclose(src);
        return;
    }
    while ((ch = fgetc(src)) != EOF)
        fputc(ch, des);

    fclose(src);
    fclose(des);
}

void cd(char **path)
{
    char combenedPath[1024] = {0}; // Buffer to construct the path

    // Start from first argument after cd
    for (int i = 1; path[i] != NULL; i++)
    {
        // If this first path component copy it
        if (i == 1)
            strncpy(combenedPath, path[i], sizeof(combenedPath) - 1);
        else
        {
            // For subsequent components, append a space and then the component
            // This handles spaces within quoted paths
            strncat(combenedPath, " ", sizeof(combenedPath) - strlen(combenedPath) - 1);
            strncat(combenedPath, path[i], sizeof(combenedPath) - strlen(combenedPath) - 1);
        }
    }

    if (combenedPath[0] == "\"")
    {
        memmove(combenedPath, combenedPath + 1, strlen(combenedPath)); // Remove first quote
        combenedPath[strlen(combenedPath) - 1] = '\0';                 // Remove last quote
    }

    if (chdir(combenedPath) != 0)
        printf("-myShell: cd: %s: No suche file or direction\n", path[1]);
}

void delete(char **path)
{
    // Assuming path[1] contains the file name
    if (path[1] == NULL)
    {
        printf("-myShell: delete: Missing file name\n");
        return;
    }

    char *resolved_path = realpath(path[1], NULL);
    if (resolved_path == NULL)
    {
        perror("-myShell: delete");
        printf("-myShell: delete: %s: No such file or directory\n", path[1]);
    }
    else
    {
        if (unlink(resolved_path) != 0)
        {
            perror("-myShell: delete");
            printf("-myShell: delete: %s: No such file or directory\n", resolved_path);
        }

        free(resolved_path);
    }
}

void mypipe(char **argv1, char **argv2)
{

    int fildes[2];
    if (fork() == 0)
    {
        pipe(fildes);
        if (fork() == 0)
        {
            /* first component of command line */
            close(STDOUT_FILENO);
            dup(fildes[1]);
            close(fildes[1]);
            close(fildes[0]);
            execvp(argv1[0], argv1);
        }
        /* 2nd command component of command line */
        close(STDIN_FILENO);
        dup(fildes[0]);
        close(fildes[0]);
        close(fildes[1]);
        /* standard input now comes from pipe */
        execvp(argv2[0], argv2);
    }
}

void splitInputForPipe(char *input, char ***argv1, char ***argv2)
{
    char *pipePos = strchr(input, '|');
    if (!pipePos)
        return; // No pipe found, should not happen since we check before calling

    int firstCommandLength = pipePos - input;
    char firstCommand[firstCommandLength + 1];
    strncpy(firstCommand, input, firstCommandLength);
    firstCommand[firstCommandLength] = '\0';

    char *secondCommand = pipePos + 1; // Skip the pipe character

    // Tokenize the first command into argv1
    int argc1 = 0;
    char **args1 = malloc(sizeof(char *));
    char *token = my_strtok(firstCommand, " ");
    while (token)
    {
        args1 = realloc(args1, sizeof(char *) * (argc1 + 1));
        args1[argc1++] = strdup(token);
        token = my_strtok(NULL, " ");
    }
    args1 = realloc(args1, sizeof(char *) * (argc1 + 1)); // For NULL termination
    args1[argc1] = NULL;                                  // NULL terminate the array

    // Tokenize the second command into argv2
    int argc2 = 0;
    char **args2 = malloc(sizeof(char *));
    token = my_strtok(secondCommand, " ");
    while (token)
    {
        args2 = realloc(args2, sizeof(char *) * (argc2 + 1));
        args2[argc2++] = strdup(token);
        token = my_strtok(NULL, " ");
    }
    args2 = realloc(args2, sizeof(char *) * (argc2 + 1)); // For NULL termination
    args2[argc2] = NULL;                                  // NULL terminate the array

    *argv1 = args1;
    *argv2 = args2;
}

void move(char **args)
{
    if (args == NULL || args[1] == NULL || args[2] == NULL)
    {
        printf("Usage: move <source> <destination>\n");
        return;
    }

    char *sourcePath = realpath(args[1], NULL); // Resolve the source file path
    if (sourcePath == NULL)
    {
        perror("Error: Failed to resolve source path");
        return;
    }

    char destPath[PATH_MAX];
    if (realpath(args[2], destPath) == NULL) // Try to resolve the destination path
    {
        perror("Error: Failed to resolve destination path");
        free(sourcePath);
        return;
    }

    // If destination is a directory, construct the full path for the file within it
    struct stat destStat;
    if (stat(destPath, &destStat) == 0 && S_ISDIR(destStat.st_mode))
    {
        // Append the source file name to the destination directory path
        char *sourceFileName = basename(sourcePath);
        if (strlen(destPath) + strlen(sourceFileName) + 2 > PATH_MAX)
        {
            printf("Error: Destination path is too long\n");
            free(sourcePath);
            return;
        }
        strcat(destPath, "/");
        strcat(destPath, sourceFileName);
    }

    if (rename(sourcePath, destPath) != 0)
    {
        perror("Error: Failed to move the file");
    }
    else
    {
        printf("File moved successfully from '%s' to '%s'\n", sourcePath, destPath);
    }

    free(sourcePath);
}

void echoppend(char **args)
{
    int size = 0;
    while (args[size] != NULL)
    { // Calculate the size of the array
        size++;
    }

    FILE *file;

    // Check if '>>' is specified before the file path
    int appendMode = 0; // Flag for append mode
    char *filePath;
    if (size >= 3 && strcmp(args[size - 2], ">>") == 0)
    {
        appendMode = 1;            // Set append mode if '>>' is found
        filePath = args[size - 1]; // File path is the last argument
        size -= 2;                 // Adjust size to skip '>>' and the file path in the loop
    }
    else
    {
        filePath = args[size - 1]; // File path is the last argument
        size -= 1;                 // Adjust size to skip the file path in the loop
    }

    // Open the file in the determined mode
    file = fopen(filePath, appendMode ? "a" : "w");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    // Append or write the arguments to the file
    for (int i = 1; i < size; i++)
    {
        fprintf(file, "%s", args[i]);
        if (i < size - 1)
        {
            fprintf(file, " "); // Add space between strings
        }
    }
    fprintf(file, "\n"); // Add a newline at the end
    fclose(file);        // Close the file after writing

    // Open the file again in read mode to print its contents
    file = fopen(filePath, "r");
    if (file == NULL)
    {
        perror("Error opening file for reading");
        return;
    }

    // Read and print the file's contents to the terminal
    char buffer[1024]; // Buffer to store lines read from the file
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        printf("%s", buffer); // Print the line
    }

    fclose(file); // Close the file after reading
}

void echowrite(char **args)
{
    int size = 0;
    int contentWritten = 0;
    while (args[size] != NULL)
    { // Calculate the size of the array
        size++;
    }

    // Check that there is at least one string to write and one for the file path
    if (size < 2)
    {
        puts("Error: Not enough arguments provided.");
        return;
    }

    // The last argument is the path to the file
    char *filePath = args[size - 1];

    // Determine if the '>' character was provided before the file path
    int overwriteFile = 0;
    if (strcmp(args[size - 2], ">") == 0)
    {
        overwriteFile = 1;
        size--; // Adjust size to ignore the '>' in args
    }

    // Open the file in the appropriate mode
    FILE *file = fopen(filePath, overwriteFile ? "w" : "a");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    // If '>' was found, args have been adjusted, and we skip it during writing
    for (int i = 1; i < size - 1; i++)
    {
        fprintf(file, "%s", args[i], "\n");
        if (i < size - 2)
        {
            fprintf(file, " "); // Add space between strings
        }
        contentWritten = 1;
    }

    if (contentWritten)
    {
        fprintf(file, "\n");
    }

    fclose(file); // Close the file after writing

    // Now open the file again in read mode to print its new contents
    file = fopen(filePath, "r");
    if (file == NULL)
    {
        perror("Error opening file for reading");
        return;
    }

    // Print the new contents of the file to the terminal
    char buffer[1024]; // Buffer to store lines read from the file
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        printf("%s", buffer); // Print the line
    }

    fclose(file); // Close the file after reading
}

void rd(char **args)
{
    FILE *file = fopen(args[1], "r"); // Attempt to open the file specified by the first argument
    if (file == NULL)
    {
        // If the file can't be opened, exit the function
        printf("File cannot be opened or does not exist.\n");
        return;
    }

    char buffer[1024]; // Buffer to store file content
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        // Read and print the file content
        printf("%s", buffer);
    }

    fclose(file); // Close the file after reading
}

void wordCount(char **args)
{
    // Attempt to open the file specified by the second argument
    FILE *file = fopen(args[2], "r");
    if (file == NULL)
    {
        // If the file can't be opened or doesn't exist, exit the function
        return;
    }

    int count = 0; // Initialize count
    char ch, prevCh = '\0';

    if (strcmp(args[1], "-l") == 0)
    {
        // Count lines
        while ((ch = fgetc(file)) != EOF)
        {
            if (ch == '\n')
            {
                count++;
            }
            prevCh = ch;
        }
        // Consider the last line if it doesn't end with '\n'
        if (prevCh != '\n')
        {
            count++;
        }
    }
    else if (strcmp(args[1], "-w") == 0)
    {
        // Count words
        int inWord = 0; // Flag to track whether we're inside a word
        while ((ch = fgetc(file)) != EOF)
        {
            if (isspace(ch) || ch == '\n' || ch == '\t')
            {
                inWord = 0;
            }
            else if (inWord == 0)
            {
                inWord = 1;
                count++;
            }
        }
    }

    fclose(file); // Close the file

    // Print the count
    printf("%d\n", count);
}