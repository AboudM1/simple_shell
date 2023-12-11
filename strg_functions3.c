#include "shell.h"

/**
 * custom_strncpy - copies a string
 * @destination: the destination string to be copied to
 * @source: the source string
 * @max_length: the maximum number of characters to be copied
 * Return: the concatenated string
 */
char *custom_strncpy(char *destination, char *source, int max_length)
{
	int i, j;
	char *result = destination;

	i = 0;
	while (source[i] != '\0' && i < max_length - 1)
	{
		destination[i] = source[i];
		i++;
	}
	if (i < max_length)
	{
		j = i;
		while (j < max_length)
		{
			destination[j] = '\0';
			j++;
		}
	}
	return (result);
}

/**
 * custom_strncat - concatenates two strings
 * @destination: the first string
 * @source: the second string
 * @max_length: the maximum number of bytes to be used
 * Return: the concatenated string
 */
char *custom_strncat(char *destination, char *source, int max_length)
{
	int i, j;
	char *result = destination;

	i = 0;
	j = 0;
	while (destination[i] != '\0')
		i++;
	while (source[j] != '\0' && j < max_length)
	{
		destination[i] = source[j];
		i++;
		j++;
	}
	if (j < max_length)
		destination[i] = '\0';
	return (result);
}

/**
 * custom_strchr - locates a character in a string
 * @str: the string to be parsed
 * @character: the character to look for
 * Return: (str) a pointer to the memory area str
 */
char *custom_strchr(char *str, char character)
{
	do {
		if (*str == character)
			return (str);
	} while (*str++ != '\0');

	return (NULL);
}
