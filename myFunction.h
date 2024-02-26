#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
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

char **splitArgument(char *);
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

void cd(char **path);
