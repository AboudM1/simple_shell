#include "shell.h"

/**
 * getHistoryFile - gets the history file
 * @shellInfo: parameter struct
 *
 * Return: allocated string containing history file
 */
char *getHistoryFile(shell_info_t *shellInfo)
{
	char *buf, *dir;

	dir = _getenv(shellInfo, "HOME=");
	if (!dir)
		return (NULL);
	buf = malloc(sizeof(char) * (_strlen(dir) + _strlen(HIST_FILE) + 2));
	if (!buf)
		return (NULL);
	buf[0] = 0;
	_strcpy(buf, dir);
	_strcat(buf, "/");
	_strcat(buf, HIST_FILE);
	return (buf);
}

/**
 * writeHistory - creates a file, or appends to an existing file
 * @shellInfo: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int writeHistory(shell_info_t *shellInfo)
{
	ssize_t fd;
	char *filename = getHistoryFile(shellInfo);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = shellInfo->history; node; node = node->next)
	{
		_putsfd(node->str, fd);
		_putfd('\n', fd);
	}
	_putfd(BUF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * readHistory - reads history from file
 * @shellInfo: the parameter struct
 *
 * Return: histCount on success, 0 otherwise
 */
int readHistory(shell_info_t *shellInfo)
{
	int i, last = 0, lineCount = 0;
	ssize_t fd, rdlen, fileSize = 0;
	struct stat st;
	char *buf = NULL, *filename = getHistoryFile(shellInfo);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fileSize = st.st_size;
	if (fileSize < 2)
		return (0);
	buf = malloc(sizeof(char) * (fileSize + 1));
	if (!buf)
		return (0);
	rdlen = read(fd, buf, fileSize);
	buf[fileSize] = 0;
	if (rdlen <= 0)
		return free(buf), 0;
	close(fd);
	for (i = 0; i < fileSize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0;
			buildHistoryList(shellInfo, buf + last, lineCount++);
			last = i + 1;
		}
	if (last != i)
		buildHistoryList(shellInfo, buf + last, lineCount++);
	free(buf);
	shellInfo->histCount = lineCount;
	while (shellInfo->histCount-- >= HIST_MAX)
		deleteNodeAtIndex(&(shellInfo->history), 0);
	renumberHistory(shellInfo);
	return (shellInfo->histCount);
}

/**
 * buildHistoryList - adds entry to a history linked list
 * @shellInfo: Structure containing potential arguments. Used to maintain
 * @buf: buffer
 * @lineCount: the history line count, histCount
 *
 * Return: Always 0
 */
int buildHistoryList(shell_info_t *shellInfo, char *buf, int lineCount)
{
	list_t *node = NULL;

	if (shellInfo->history)
		node = shellInfo->history;
	addNodeEnd(&node, buf, lineCount);

	if (!shellInfo->history)
		shellInfo->history = node;
	return (0);
}

/**
 * renumberHistory - renumbers the history linked list after changes
 * @shellInfo: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histCount
 */
int renumberHistory(shell_info_t *shellInfo)
{
	list_t *node = shellInfo->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (shellInfo->histCount = i);
}
