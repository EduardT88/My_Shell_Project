#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <libgen.h>
#include <sys/wait.h>

#define SIZE_BUFF 1024

void getLocation();
/**
 * Retrieves and displays the current working directory and hostname.
 *
 * This function fetches the current working directory and the system's hostname
 * and displays them on the standard output. The hostname is displayed in red,
 * and the current working directory is displayed in blue, followed by a '$'
 * sign, mimicking a typical command prompt style. This can be used to enhance
 * shell scripts or command-line tools with a custom prompt that provides
 * contextual information to the user.
 *
 * @note This function does not take any parameters.
 *
 * @warning The function uses ANSI escape codes to color the output, which may not
 *          be supported by all terminal emulators. If the terminal does not support
 *          ANSI escape codes, the output may include raw escape characters.
 *
 * @error Handling includes printing error messages to standard output if fetching
 *        the current working directory or hostname fails. The function returns
 *        early in these cases, avoiding partial or misleading output.
 *
 * @usage To use this function in a shell-like application, simply call it
 *        whenever you need to display a prompt or contextual information
 *        about the execution environment. Ensure that the terminal supports
 *        ANSI escape codes for the best experience.
 */

char *getInputFromUser();
/**
 * Reads a line of text from the user input until a newline character is encountered.
 *
 * This function dynamically allocates memory to store the characters read from the standard input,
 * including spaces but excluding the newline character. It automatically resizes the memory allocation
 * as more characters are read. The input is terminated when the newline character ('\n') is encountered,
 * which is not included in the returned string. The resulting string is null-terminated.
 *
 * The function starts with an initial allocation for one character and increases the allocation size by one
 * for each additional character read, including space for the null terminator at the end. This approach ensures
 * that the string is always properly null-terminated and that there is exactly enough memory allocated for the
 * number of characters read plus the null terminator.
 *
 * @return A pointer to the dynamically allocated string containing the user input. The caller is responsible
 *         for freeing this memory using free() when it is no longer needed.
 *
 * @warning This function uses getchar() to read input, which may behave differently in different environments
 *          and with different input buffering configurations. It is designed for simple console input scenarios.
 *
 * @note If an error occurs during memory allocation, the function returns NULL. The caller should check the
 *       return value before using the result to ensure that the operation was successful.
 *
 * @example char *userInput = getInputFromUser();
 *          if (userInput != NULL) {
 *              printf("You entered: %s\n", userInput);
 *              free(userInput);
 *          } else {
 *              printf("Memory allocation failed.\n");
 *          }
 */

char **splitArgument(char *str);
/**
 * Splits a string into individual tokens based on spaces.
 *
 * This function accepts a string and divides it into tokens whenever a space character is
 * encountered. It dynamically allocates memory for an array of string pointers to hold
 * these tokens. The caller must ensure the release of this memory to avoid memory leaks.
 *
 * @param str A pointer to the string that is to be tokenized. This string is modified by the
 *           function as it replaces spaces with null terminators to isolate tokens.
 *
 * @return A dynamically allocated array of string pointers (char**), where each pointer
 *         directs to a token within the original string. The array is terminated by a NULL pointer to
 *         signify the end of the tokens. If the input is NULL or if a memory allocation fails, the
 *         function returns NULL.
 *
 * @note Since strtok is used for tokenization, the input string is altered by this function.
 *       Original string content is lost as spaces are replaced with null characters to end tokens. If
 *       the original string needs to be preserved, a copy should be passed instead.
 *
 *@warning The function allocates memory dynamically for the array of string pointers
 *         but does not allocate new memory for the tokens themselves; instead, it points to locations
 *         within the modified input string. The caller is responsible for freeing the memory
 *         allocated for the array of pointers. Additionally, care should be taken not to free individual
 *         tokens since they point within the modified input string.
 */

char *my_strtok(char *str, const char *delim);
/**
 * Custom string tokenizer function that emulates the behavior of the standard strtok function.
 *
 * This function tokenizes a given string ('str') based on a set of delimiter
 * characters ('delim'), returning one token per call. On the first call,
 * 'str' should point to the string to be tokenized; on subsequent calls,
 * 'str' should be NULL, indicating continuation of tokenization on the same
 * string. The function maintains the context of its operation through
 * a static internal pointer, allowing it to continue from where
 *  it left off in the previous call.
 *
 * The function operates by first skipping any leading delimiters to find
 * the start of the next token. It then scans ahead in the string to find
 * the end of this token, which is either the next occurrence of any
 * character in 'delim' or the end of the string. This token is
 * null-terminated, and the internal pointer is updated to point just
 * past the end of the token, preparing the function for the next call.
 *
 * @param str The string to tokenize on the first call; NULL on subsequent
 *            calls.
 * @param delim A null-terminated string containing all possible delimiter
 *            characters.
 *
 * @return Returns a pointer to the next token found in the string,
 *         or NULL if there are no more tokens.
 *
 * @note This function modifies the input string by inserting NULL
 *       characters ('\0') to terminate each token.
 * @warning This function maintains state between calls using a static
 *          internal pointer, making it not reentrant and not safe for
 *          concurrent use in multi-threaded environments.
 * @error Handling includes returning NULL immediately if 'str' is NULL and
 *        no previous string is being tokenized, indicating there are no
 *        more tokens to retrieve.
 */

void logout(char *input);

void echo(char **arguments);

void cp(char **arguments);
/**
 * A simple file copy function that duplicates the contents of one file to another.
 *
 * This function takes an array of strings 'arguments' as input, where
 * 'arguments[1]' is expected to be the path of the source file and 'arguments[2]'
 * the path of the destination file. It attempts to open the source file in read mode
 * and the destination file in write mode. If either file cannot be opened, an error
 * message is printed to standard output and the function returns early.
 *
 * The function reads the source file character by character using fgetc and writes
 * each character to the destination file using fputc. This process continues until
 * the end of the source file is reached (indicated by fgetc returning EOF, the
 * end-of-file marker). Finally, the function closes both the source and destination
 * files to free up system resources.
 *
 * @param arguments An array of string pointers, with 'arguments[1]' being the
 *                  source file path and 'arguments[2]' the destination file path.
 *
 * @return This function does not return a value. It performs the copy operation
 *         and prints an error message to standard output if it cannot open
 *         the source or destination file.
 *
 * @note This function uses standard I/O library functions fgetc and fputc to
 *       perform the file copy operation. It handles file opening and closing
 *       internally, ensuring that all resources are properly managed.
 * @warning If the destination file already exists, its contents will be
 *          overwritten without warning. Users should ensure that overwriting
 *          is the intended action or check file existence before calling.
 * @error Handling includes printing "error" to standard output if a file cannot
 *        be opened, and early return from the function to avoid further operations.
 */

void cd(char **path);
/**
 * Custom 'cd' command implementation that changes the current working directory.
 *
 * This function takes an array of strings 'path' as input, which represents the
 * arguments passed to the 'cd' command. It constructs a single directory path
 * from these arguments, accommodating spaces within the path by concatenating
 * the arguments with a space delimiter. This is particularly useful for handling
 * paths with spaces that are typically enclosed in quotes on the command line.
 *
 * The function first initializes a buffer 'combenedPath' to construct the full path.
 * It iterates through the 'path' array starting from the first argument after 'cd',
 * combining the arguments into 'combenedPath'. If the path is enclosed in quotes,
 * these are removed from the beginning and end of the constructed path.
 *
 * Finally, the function attempts to change the current working directory to
 * 'combenedPath' using the chdir system call. If chdir returns a non-zero value,
 * indicating failure, an error message is printed specifying that the file or
 * directory could not be found.
 *
 * @param path An array of string pointers containing the command ('cd') and its
 *             arguments, representing parts of the path or the full path to change
 *             the current directory to.
 *
 * @return This function does not return a value. It either changes the current
 *         working directory or prints an error message if the specified path does
 *         not exist.
 *
 * @note This function supports paths with spaces by expecting such paths to be
 *       passed in as separate arguments, which it then concatenates with a space
 *       delimiter. This approach assumes that the command line arguments are
 *       provided in a format that separates space-containing paths into multiple
 *       arguments.
 * @warning The function does not provide extensive error handling for invalid
 *          path formats or inaccessible directories due to permissions or other
 *          reasons. It simply prints a generic error message for any failure.
 * @error Handling involves checking the return value of chdir and printing an
 *        error message if the path is not found or cannot be accessed.
 */

void delete(char **path);
/**
 * A simple file deletion function designed for a custom shell.
 *
 * This function takes an array of strings 'path', where 'path[1]' is expected to
 * be the name (or path) of the file to be deleted. It first checks if the file
 * name is provided; if not, it prints an error message indicating that the file
 * name is missing and returns immediately.
 *
 * The function attempts to resolve the absolute path of the specified file using
 * the 'realpath' function. If 'realpath' fails to resolve the path (indicating that
 * the file does not exist or cannot be accessed), it prints an error message along
 * with the name of the file that could not be found.
 *
 * If the path is successfully resolved, the function then attempts to delete the
 * file using the 'unlink' system call. If 'unlink' fails (for example, if the file
 * does not exist or the user lacks the necessary permissions), an error message is
 * printed specifying the issue.
 *
 * The dynamically allocated memory for the resolved path is freed after the
 * deletion attempt, regardless of whether 'unlink' succeeded or failed.
 *
 * @param path An array of string pointers, with 'path[1]' expected to contain the
 *             file name or path of the file to be deleted.
 *
 * @return This function does not return a value. It either deletes the specified
 *         file or prints an error message if the file cannot be found or deleted.
 *
 * @note This function uses the 'realpath' function to obtain the absolute path of
 *       the file, which helps in accurately identifying the file to be deleted.
 * @warning The function does not perform extensive validation on the provided file
 *          path and assumes that 'path[1]' contains the correct file name or path.
 *          Incorrect or malicious file paths can lead to unintended consequences.
 * @error Handling includes checking for a NULL 'path[1]', failure of 'realpath' to
 *        resolve the file path, and failure of 'unlink' to delete the file. In each
 *        case, appropriate error messages are printed to inform the user.
 */

void mypipe(char **argv1, char **argv2);
/**
 * Custom implementation of a pipe mechanism between two commands in a shell-like environment.
 *
 * This function takes two arrays of strings, 'argv1' and 'argv2', each representing the
 * arguments for two separate commands that need to be piped together. The function sets
 * up a unidirectional data channel (pipe) such that the standard output of the command
 * represented by 'argv1' becomes the standard input of the command represented by 'argv2'.
 *
 * The function first creates a pipe using the 'pipe()' system call, which fills the 'fildes'
 * array with two file descriptors: 'fildes[0]' for reading from the pipe, and 'fildes[1]'
 * for writing to the pipe. It then forks the current process. In the child process (where
 * 'fork()' returns 0), another fork is performed to create two distinct processes for
 * handling each command.
 *
 * In the inner child process, which executes the first command ('argv1'), the standard
 * output (STDOUT_FILENO) is redirected to the write end of the pipe ('fildes[1]') using
 * 'dup()', and then the command is executed with 'execvp()'. The read end of the pipe
 * ('fildes[0]') is closed in this process, as it does not need to read from the pipe.
 *
 * In the outer child process, which is responsible for executing the second command
 * ('argv2'), the standard input (STDIN_FILENO) is redirected to the read end of the pipe
 * ('fildes[0]') using 'dup()', and then the command is executed with 'execvp()'. The
 * write end of the pipe ('fildes[1]') is closed in this process, as it does not need to
 * write to the pipe.
 *
 * @param argv1 An array of string pointers, representing the arguments for the first command.
 * @param argv2 An array of string pointers, representing the arguments for the second command.
 *
 * @note This function assumes 'argv1' and 'argv2' are properly null-terminated arrays of strings.
 * @warning This function does not include error handling for the 'pipe()', 'fork()', 'dup()',
 *          or 'execvp()' calls, and assumes these system calls succeed. Failure of these calls
 *          can lead to undefined behavior.
 * @error There is no explicit error handling or cleanup mechanism for system call failures in
 *        this function, which is a significant oversight in robust program design.
 */

void splitInputForPipe(char *input, char ***argv1, char ***argv2);

void move(char **args);
/**
 * Implements a file moving function similar to the 'mv' command in Unix-like systems.
 *
 * This function expects an array 'args' containing at least three elements: the command name,
 * the source file path, and the destination path. It first verifies that these arguments are
 * provided. If not, it prints a usage message and returns early.
 *
 * The function then resolves the absolute path of the source file using 'realpath'. If this
 * fails (indicating the source file does not exist or cannot be accessed), an error message
 * is printed, and the function returns.
 *
 * Next, it attempts to resolve the destination path. If 'realpath' fails for the destination,
 * it's assumed the destination does not exist yet, or it might be a new file name in an existing
 * directory. The function then checks if the resolved destination is a directory using 'stat'
 * and 'S_ISDIR'. If so, it appends the basename of the source file to the destination path,
 * effectively moving the file into that directory.
 *
 * Finally, the function attempts to move the file by renaming the source path to the destination
 * path using 'rename'. If successful, it prints a confirmation message; otherwise, it prints an
 * error message.
 *
 * @param args An array of string pointers, with 'args[1]' containing the source file path and
 *             'args[2]' containing the destination path or directory.
 *
 * @note The function assumes 'args' is null-terminated and contains at least three elements.
 * @warning The function does not handle cases where the destination is an existing file. The
 *          'rename' function will overwrite the destination file without warning, which might not
 *          be the desired behavior.
 * @error Handling includes checking for null or insufficient arguments, failure to resolve the
 *        source or destination paths, and failure of the 'rename' operation. Appropriate error
 *        messages are printed for each case.
 */

void echoppend(char **args);
/**
 * Implements an 'echo' command with the ability to append or write to a file.
 *
 * This function, 'echoppend', takes an array of strings 'args', where each element
 * represents a part of the command. The command echoes the given strings to a specified
 * file and can operate in either write ('w') or append ('a') mode. If '>>' is found
 * before the last argument, the function appends the strings to the file; otherwise,
 * it writes to the file, overwriting its previous contents.
 *
 * The function first calculates the size of the 'args' array to determine the number
 * of strings to echo. It then checks for the presence of '>>' to set the file open
 * mode and adjusts the size to exclude the file path (and '>>' if present) from the
 * echo content.
 *
 * After opening the file in the determined mode, the function iterates over the 'args'
 * array, writing each string to the file, separated by spaces. A newline is added at
 * the end of the echoed content.
 *
 * Lastly, the function reopens the file in read mode to print its updated contents to
 * the terminal. This provides immediate feedback of the file's contents after the echo
 * operation.
 *
 * @param args An array of string pointers, starting with the command name, followed by
 *             the strings to echo, and ending with an optional '>>' and the file path.
 *
 * @note The function assumes 'args' is null-terminated and contains at least two elements.
 * @warning The function does not handle potential errors in file opening, writing, or reading
 *          beyond printing a perror message. There are no checks for maximum file path length
 *          or buffer overflow when reading the file's contents.
 * @error Handling includes checking for a NULL file pointer after attempting to open the file
 *        for writing/reading and printing an appropriate perror message upon failure.
 */

void echowrite(char **args);
/**
 * Implements a custom version of the 'echo' command with file write capability.
 *
 * This function, 'echowrite', writes provided text arguments to a file, supporting
 * both overwriting and appending modes. It parses the command-line arguments given
 * in 'args', expecting the last argument to be the file path. If the penultimate
 * argument is ">", the file is overwritten; otherwise, the text is appended.
 *
 * The function begins by calculating the size of 'args' to determine how many
 * strings have been passed. It requires at least one string to write and the file
 * path, thus it checks for a minimum of two arguments. If insufficient arguments
 * are provided, it displays an error message and exits.
 *
 * Based on the presence of ">", 'echowrite' decides the file opening mode ("w" for
 * overwrite and "a" for append). It opens the file in the determined mode and writes
 * the text arguments to it, each separated by a space. After writing, it appends a
 * newline character and closes the file.
 *
 * To provide immediate feedback on the operation, the function reopens the file in
 * read mode and prints its contents to the terminal, allowing the user to see the
 * result of their 'echowrite' command.
 *
 * @param args An array of string pointers, where each element up to the second to
 *             last (optionally excluding the penultimate if it's ">") is considered
 *             text to write, and the last element is the file path.
 *
 * @note The function assumes 'args' is null-terminated. It checks for the presence
 *       of ">", adjusting its behavior to either append to or overwrite the target
 *       file based on this indicator.
 * @warning The function overwrites or appends to the file without any confirmation
 *          or safety check, which might result in unintended data loss.
 * @error Handling includes checking for sufficient arguments, failure to open the
 *        file for writing or reading, with appropriate error messages displayed.
 */

void rd(char **args);
/**
 * A simple file reading function that displays the contents of a specified file.
 *
 * This function, 'rd', takes an array of strings 'args', where 'args[1]' is expected
 * to contain the path of the file to be read. It attempts to open the specified file
 * in read mode. If the file cannot be opened, possibly because it does not exist or
 * the program lacks the necessary permissions, the function prints an error message
 * and exits.
 *
 * Upon successfully opening the file, 'rd' reads its content line by line into a buffer
 * and prints each line to the standard output. This continues until the end of the file
 * is reached, signified by 'fgets' returning NULL. Finally, the function closes the file
 * to release the system resources.
 *
 * @param args An array of string pointers, with 'args[1]' expected to be the path to the
 *             file that needs to be read.
 *
 * @note The function is designed to read text files. Binary files may not be displayed
 *       correctly due to the presence of null characters and other non-printable bytes.
 * @warning There's no explicit error handling for scenarios where 'args' does not contain
 *          at least two elements, which could lead to undefined behavior if 'args[1]' is
 *          accessed without checking.
 * @error Handling includes checking the file's existence and accessibility, with an error
 *        message printed if the file cannot be opened.
 */

void wordCount(char **args);
/**
 * A function to count words or lines in a specified file, similar to the 'wc' Unix command.
 *
 * The 'wordCount' function takes an array of strings 'args' as input, where 'args[1]'
 * specifies the counting mode ('-l' for lines, '-w' for words) and 'args[2]' contains
 * the path to the file. It attempts to open the specified file in read mode. If the file
 * cannot be opened, the function silently exits without producing output.
 *
 * In line counting mode ('-l'), the function increments 'count' for each newline character
 * encountered. It also accounts for the last line of the file, which might not end with a
 * newline character, by incrementing 'count' if the last character read is not a newline.
 *
 * In word counting mode ('-w'), the function uses a flag 'inWord' to track whether it is
 * currently inside a word. A word is defined as any sequence of characters that does not
 * contain spaces, newlines, or tabs. The 'count' is incremented upon transitioning from a
 * non-word to a word state, which is detected by reading a non-space character following
 * any space character.
 *
 * After processing the file, the function closes it and prints the 'count' to the standard
 * output. The count reflects the number of lines or words in the file, depending on the mode
 * selected.
 *
 * @param args An array of string pointers, with 'args[1]' indicating the mode ('-l' for lines,
 *             '-w' for words) and 'args[2]' specifying the file path.
 *
 * @note This function assumes the 'args' array is properly formatted and contains at least
 *       three elements. It does not provide explicit error messages for file opening failures
 *       or invalid mode specifications.
 * @warning The function exits silently if it cannot open the file, potentially leading to
 *          confusion if the user does not realize the operation has failed.
 * @error Handling is minimal, with the function designed to exit without output if the file
 *        cannot be opened. Additional error handling and user feedback might be desirable.
 */
