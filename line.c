#include "shell.h"

/**
 * inputBuffer - buffers chained commands
 * @shellInfo: parameter struct
 * @buffer: address of buffer
 * @length: address of length variable
 *
 * Return: bytes read
 */
ssize_t inputBuffer(info_t *shellInfo, char **buffer, size_t *length)
{
	ssize_t bytesRead = 0;
	size_t lenPointer = 0;

	if (!*length) /* if nothing left in the buffer, fill it */
	{
		free(*buffer);
		*buffer = NULL;
		signal(SIGINT, sigintHandler);
#if USE_GETLINE
		bytesRead = getline(buffer, &lenPointer, stdin);
#else
		bytesRead = _getline(shellInfo, buffer, &lenPointer);
#endif
		if (bytesRead > 0)
		{
			if ((*buffer)[bytesRead - 1] == '\n')
			{
				(*buffer)[bytesRead - 1] = '\0'; /* remove trailing newline */
				bytesRead--;
			}
			shellInfo->linecountFlag = 1;
			removeComments(*buffer);
			buildHistoryList(shellInfo, *buffer, shellInfo->histcount++);
			{
				*length = bytesRead;
				shellInfo->commandBuffer = buffer;
			}
		}
	}
	return bytesRead;
}

/**
 * getInput - gets a line minus the newline
 * @shellInfo: parameter struct
 *
 * Return: bytes read
 */
ssize_t getInput(info_t *shellInfo)
{
	static char *buffer; /* the ';' command chain buffer */
	static size_t i, j, length;
	ssize_t bytesRead = 0;
	char **bufferPointer = &(shellInfo->arg), *p;

	_putchar(BUF_FLUSH);
	bytesRead = inputBuffer(shellInfo, &buffer, &length);
	if (bytesRead == -1) /* EOF */
		return -1;
	if (length) /* we have commands left in the chain buffer */
	{
		j = i; /* init new iterator to the current buffer position */
		p = buffer + i; /* get pointer for return */

		checkChain(shellInfo, buffer, &j, i, length);
		while (j < length) /* iterate to semicolon or end */
		{
			if (isChainDelimiter(shellInfo, buffer, &j))
				break;
			j++;
		}

		i = j + 1; /* increment past nulled ';'' */
		if (i >= length) /* reached the end of the buffer? */
		{
			i = length = 0; /* reset position and length */
			shellInfo->commandBufferType = CMD_NORM;
		}

		*bufferPointer = p; /* pass back pointer to the current command position */
		return _strlen(p); /* return length of the current command */
	}

	*bufferPointer = buffer; /* else not a chain, pass back buffer from _getline() */
	return bytesRead; /* return length of the buffer from _getline() */
}

/**
 * readBuffer - reads a buffer
 * @shellInfo: parameter struct
 * @buffer: buffer
 * @i: size
 *
 * Return: bytesRead
 */
ssize_t readBuffer(info_t *shellInfo, char *buffer, size_t *i)
{
	ssize_t bytesRead = 0;

	if (*i)
		return 0;
	bytesRead = read(shellInfo->readfd, buffer, READ_BUF_SIZE);
	if (bytesRead >= 0)
		*i = bytesRead;
	return bytesRead;
}

/**
 * _getline - gets the next line of input from STDIN
 * @shellInfo: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: s
 */
int _getline(info_t *shellInfo, char **ptr, size_t *length)
{
	static char buf[READ_BUF_SIZE];
	static size_t i, len;
	size_t k;
	ssize_t bytesRead = 0, size = 0;
	char *p = NULL, *newP = NULL, *c;

	p = *ptr;
	if (p && length)
		size = *length;
	if (i == len)
		i = len = 0;

	bytesRead = readBuffer(shellInfo, buf, &len);
	if (bytesRead == -1 || (bytesRead == 0 && len == 0))
		return -1;

	c = _strchr(buf + i, '\n');
	k = c ? 1 + (unsigned int)(c - buf) : len;
	newP = _realloc(p, size, size ? size + k : k + 1);
	if (!newP) /* MALLOC FAILURE! */
		return p ? free(p), -1 : -1;

	if (size)
		_strncat(newP, buf + i, k - i);
	else
		_strncpy(newP, buf + i, k - i + 1);

	size += k - i;
	i = k;
	p = newP;

	if (length)
		*length = size;
	*ptr = p;
	return size;
}

/**
 * sigintHandler - blocks ctrl-C
 * @sigNum: the signal number
 *
 * Return: void
 */
void sigintHandler(__attribute__((unused))int sigNum)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);
}
