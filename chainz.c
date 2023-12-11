#include "shell.h"

/**
 * isChainDelimiter - test if the current char in buffer is a chain delimiter
 * @shellInfo: the parameter struct
 * @buffer: the char buffer
 * @position: address of the current position in buffer
 *
 * Return: 1 if chain delimiter, 0 otherwise
 */
int isChainDelimiter(info_t *shellInfo, char *buffer, size_t *position)
{
	size_t j = *position;

	if (buffer[j] == '|' && buffer[j + 1] == '|')
	{
		buffer[j] = 0;
		j++;
		shellInfo->commandBufferType = CMD_OR;
	}
	else if (buffer[j] == '&' && buffer[j + 1] == '&')
	{
		buffer[j] = 0;
		j++;
		shellInfo->commandBufferType = CMD_AND;
	}
	else if (buffer[j] == ';') /* found end of this command */
	{
		buffer[j] = 0; /* replace semicolon with null */
		shellInfo->commandBufferType = CMD_CHAIN;
	}
	else
		return (0);
	*position = j;
	return (1);
}

/**
 * checkChain - checks if we should continue chaining based on the last status
 * @shellInfo: the parameter struct
 * @buffer: the char buffer
 * @position: address of the current position in buffer
 * @start: starting position in buffer
 * @length: length of buffer
 *
 * Return: Void
 */
void checkChain(info_t *shellInfo, char *buffer, size_t *position, size_t start, size_t length)
{
	size_t j = *position;

	if (shellInfo->commandBufferType == CMD_AND)
	{
		if (shellInfo->status)
		{
			buffer[start] = 0;
			j = length;
		}
	}
	if (shellInfo->commandBufferType == CMD_OR)
	{
		if (!shellInfo->status)
		{
			buffer[start] = 0;
			j = length;
		}
	}

	*position = j;
}

/**
 * replaceAlias - replaces an alias in the tokenized string
 * @shellInfo: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replaceAlias(info_t *shellInfo)
{
	int i;
	list_t *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = nodeStartsWith(shellInfo->alias, shellInfo->argv[0], '=');
		if (!node)
			return (0);
		free(shellInfo->argv[0]);
		p = _strchr(node->str, '=');
		if (!p)
			return (0);
		p = _strdup(p + 1);
		if (!p)
			return (0);
		shellInfo->argv[0] = p;
	}
	return (1);
}

/**
 * replaceVars - replaces vars in the tokenized string
 * @shellInfo: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replaceVars(info_t *shellInfo)
{
	int i = 0;
	list_t *node;

	for (i = 0; shellInfo->argv[i]; i++)
	{
		if (shellInfo->argv[i][0] != '$' || !shellInfo->argv[i][1])
			continue;

		if (!_strcmp(shellInfo->argv[i], "$?"))
		{
			replaceString(&(shellInfo->argv[i]),
				_strdup(convertNumber(shellInfo->status, 10, 0)));
			continue;
		}
		if (!_strcmp(shellInfo->argv[i], "$$"))
		{
			replaceString(&(shellInfo->argv[i]),
				_strdup(convertNumber(getpid(), 10, 0)));
			continue;
		}
		node = nodeStartsWith(shellInfo->env, &shellInfo->argv[i][1], '=');
		if (node)
		{
			replaceString(&(shellInfo->argv[i]),
				_strdup(_strchr(node->str, '=') + 1));
			continue;
		}
		replaceString(&shellInfo->argv[i], _strdup(""));

	}
	return (0);
}

/**
 * replaceString - replaces a string
 * @old: address of the old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replaceString(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}
