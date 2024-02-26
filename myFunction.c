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

    char *subStr;
    // char *delim = " ";

    subStr = my_strtok(str, " ");
    int size = 2;
    int index = 0;
    char **argumnts = (char **)malloc(size * sizeof(char *));
    *(argumnts + index) = subStr; // [subStr,subStr,subStr,subStr,NULL]
    while ((subStr = my_strtok(NULL, " ")) != NULL)
    {
        index++;
        size++;
        argumnts = (char **)realloc(argumnts, size * sizeof(char *));
        *(argumnts + index) = subStr;
    }
    *(argumnts + (index + 1)) = NULL;

    return argumnts;
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


