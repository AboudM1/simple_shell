#include "shell.h"

/**
 * displayEnvironment - prints the current environment
 * @shellInfo: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0
 */
int displayEnvironment(shell_info_t *shellInfo)
{
    printListString(shellInfo->environment);
    return 0;
}

/**
 * getEnvironmentVariable - gets the value of an environment variable
 * @shellInfo: Structure containing potential arguments. Used to maintain
 * @name: env var name
 *
 * Return: the value
 */
char *getEnvironmentVariable(shell_info_t *shellInfo, const char *name)
{
    list_t *node = shellInfo->environment;
    char *p;

    while (node)
    {
        p = startsWith(node->str, name);
        if (p && *p)
            return p;
        node = node->next;
    }
    return NULL;
}

/**
 * setEnvironmentVariable - Initialize a new environment variable,
 *             or modify an existing one
 * @shellInfo: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int setEnvironmentVariable(shell_info_t *shellInfo)
{
    if (shellInfo->argCount != 3)
    {
        _eputs("Incorrect number of arguments\n");
        return 1;
    }
    if (_setenv(shellInfo, shellInfo->args[1], shellInfo->args[2]))
        return 0;
    return 1;
}

/**
 * unsetEnvironmentVariable - Remove an environment variable
 * @shellInfo: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int unsetEnvironmentVariable(shell_info_t *shellInfo)
{
    int i;

    if (shellInfo->argCount == 1)
    {
        _eputs("Too few arguments.\n");
        return 1;
    }
    for (i = 1; i <= shellInfo->argCount; i++)
        _unsetenv(shellInfo, shellInfo->args[i]);

    return 0;
}

/**
 * initializeEnvironmentList - populates env linked list
 * @shellInfo: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int initializeEnvironmentList(shell_info_t *shellInfo)
{
    list_t *node = NULL;
    size_t i;

    for (i = 0; shellInfo->environmentVariables[i]; i++)
        addNodeEnd(&node, shellInfo->environmentVariables[i], 0);
    shellInfo->environment = node;
    return 0;
}
