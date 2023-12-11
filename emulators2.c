#include "shell.h"

/**
 * displayHistory - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @shellInfo: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int displayHistory(shell_info_t *shellInfo)
{
	printList(shellInfo->history);
	return (0);
}

/**
 * unsetAlias - sets alias to string
 * @shellInfo: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int unsetAlias(shell_info_t *shellInfo, char *str)
{
	char *p, c;
	int ret;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = deleteNodeAtIndex(&(shellInfo->alias),
		getNodeIndex(shellInfo->alias, nodeStartsWith(shellInfo->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * setAlias - sets alias to string
 * @shellInfo: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int setAlias(shell_info_t *shellInfo, char *str)
{
	char *p;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unsetAlias(shellInfo, str));

	unsetAlias(shellInfo, str);
	return (addNodeEnd(&(shellInfo->alias), str, 0) == NULL);
}

/**
 * printAlias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int printAlias(list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = _strchr(node->str, '=');
		for (a = node->str; a <= p; a++)
			_putchar(*a);
		_putchar('\'');
		_puts(p + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * aliasCommand - mimics the alias builtin (man alias)
 * @shellInfo: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int aliasCommand(shell_info_t *shellInfo)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (shellInfo->argc == 1)
	{
		node = shellInfo->alias;
		while (node)
		{
			printAlias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; shellInfo->argv[i]; i++)
	{
		p = _strchr(shellInfo->argv[i], '=');
		if (p)
			setAlias(shellInfo, shellInfo->argv[i]);
		else
			printAlias(nodeStartsWith(shellInfo->alias, shellInfo->argv[i], '='));
	}

	return (0);
}
