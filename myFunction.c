#include "myFunction.h"


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

// עליכם לממש את הפונקציה strtok כלומר שהפונקציה הנ"ל תבצע בדיוק אותו הדבר רק בלי השימוש בפונקציה strtok
char **splitArgument(char *str)
{
    // cp file file
    //[cp,file,file,NULL]
    char *subStr;
    // cp\0file file
    // hello1\0hello2\0hello3\0hello4\0
    // subStr = address of 'h'
    // str = address of 'h'
    // int i=0;
    // int startIndex=0;
    // while(*str+i!=' '){
    // }
    // *(str+i)='\0';
    // // [str+startIndex,]
    // startIndex=++i;
    // while(*str+i!=' '){
    // }
    // *(str+i)='\0';
    // // [str+startIndex,str+startIndex]
    // startIndex=++i;

    subStr = strtok(str, " ");
    int size = 2;
    int index = 0;
    char **argumnts = (char **)malloc(size * sizeof(char *));
    *(argumnts + index) = subStr; // [subStr,subStr,subStr,subStr,NULL]
    while ((subStr = strtok(NULL, " ")) != NULL)
    {
        index++;
        size++;
        argumnts = (char **)realloc(argumnts, size * sizeof(char *));
        *(argumnts + index) = subStr;
    }
    *(argumnts + (index + 1)) = NULL;

    return argumnts;
}

// בכל שינוי יש לבצע קומיט מתאים העבודה מחייבת עבודה עם גיט.
// ניתן להוסיף פונקציות עזר לתוכנית רק לשים לב שלא מוסיפים את חתימת הפונקציה לקובץ הכותרות
