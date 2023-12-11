#include "shell.h"

/**
 * _eputs - prints a string to stderr
 * @str: the string to be printed
 *
 * Return: Nothing
 */
void _eputs(char *str)
{
	int i = 0;

	if (!str)
		return;
	while (str[i] != '\0')
	{
		_eputchar(str[i]);
		i++;
	}
}

/**
 * _eputchar - writes the character c to stderr
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _eputchar(char c)
{
	static int index;
	static char buffer[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || index >= WRITE_BUF_SIZE)
	{
		write(2, buffer, index);
		index = 0;
	}
	if (c != BUF_FLUSH)
		buffer[index++] = c;
	return (1);
}

/**
 * _put_to_fd - writes the character c to the given file descriptor
 * @c: The character to print
 * @fd: The file descriptor to write to
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _put_to_fd(char c, int fd)
{
	static int index;
	static char buffer[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || index >= WRITE_BUF_SIZE)
	{
		write(fd, buffer, index);
		index = 0;
	}
	if (c != BUF_FLUSH)
		buffer[index++] = c;
	return (1);
}

/**
 * _puts_to_fd - prints a string to the given file descriptor
 * @str: the string to be printed
 * @fd: the file descriptor to write to
 *
 * Return: the number of characters written
 */
int _puts_to_fd(char *str, int fd)
{
	int count = 0;

	if (!str)
		return (0);
	while (*str)
	{
		count += _put_to_fd(*str++, fd);
	}
	return (count);
}
