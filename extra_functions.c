#include "shell.h"

/**
 * is_shell_interactive - returns true if the shell is in interactive mode
 * @info: struct address
 *
 * Return: 1 if interactive mode, 0 otherwise
 */
int is_shell_interactive(info_t *info)
{
	return (isatty(STDIN_FILENO) && info->readfd <= 2);
}

/**
 * is_delimiter - checks if character is a delimiter
 * @character: the char to check
 * @delimiter_string: the delimiter string
 * Return: 1 if true, 0 if false
 */
int is_delimiter(char character, char *delimiter_string)
{
	while (*delimiter_string)
		if (*delimiter_string++ == character)
			return (1);
	return (0);
}

/**
 * custom_isalpha - checks for alphabetic character
 * @character: The character to check
 * Return: 1 if character is alphabetic, 0 otherwise
 */
int custom_isalpha(int character)
{
	if ((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z'))
		return (1);
	else
		return (0);
}

/**
 * custom_atoi - converts a string to an integer
 * @str: the string to be converted
 * Return: 0 if no numbers in string, converted number otherwise
 */
int custom_atoi(char *str)
{
	int i, sign = 1, flag = 0, output;
	unsigned int result = 0;

	for (i = 0;  str[i] != '\0' && flag != 2; i++)
	{
		if (str[i] == '-')
			sign *= -1;

		if (str[i] >= '0' && str[i] <= '9')
		{
			flag = 1;
			result *= 10;
			result += (str[i] - '0');
		}
		else if (flag == 1)
			flag = 2;
	}

	if (sign == -1)
		output = -result;
	else
		output = result;

	return (output);
}
