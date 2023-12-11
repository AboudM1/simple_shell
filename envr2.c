#include "shell.h"

/**
 * copyEnvironment - returns the string array copy of our environment
 * @shellInfo: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **copyEnvironment(shell_info_t *shellInfo)
{
    if (!shellInfo->environmentStrings || shellInfo->environmentChanged)
    {
        shellInfo->environmentStrings = listToStrings(shellInfo->envr);
        shellInfo->environmentChanged = 0;
    }

    return shellInfo->environmentStrings;
}

/**
 * removeEnvironmentVariable - Remove an environment variable
 * @shellInfo: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: 1 on delete, 0 otherwise
 * @variable: the string env var property
 */
int removeEnvironmentVariable(shell_info_t *shellInfo, char *variable)
{
    list_t *node = shellInfo->environment;
    size_t i = 0;
    char *p;

    if (!node || !variable)
        return 0;

    while (node)
    {
        p = startsWith(node->str, variable);
        if (p && *p == '=')
        {
            shellInfo->environmentChanged = deleteNodeAtIndex(&(shellInfo->envr), i);
            i = 0;
            node = shellInfo->environment;
            continue;
        }
        node = node->next;
        i++;
    }
    return shellInfo->environmentChanged;
}

/**
 * setEnvironmentVariable - Initialize a new environment variable,
 *             or modify an existing one
 * @shellInfo: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @variable: the string env var property
 * @value: the string env var value
 *  Return: Always 0
 */
int setEnvironmentVariable(shell_info_t *shellInfo, char *variable, char *value)
{
    char *buffer = NULL;
    list_t *node;
    char *p;

    if (!variable || !value)
        return 0;

    buffer = malloc(_strlen(variable) + _strlen(value) + 2);
    if (!buffer)
        return 1;
    _strcpy(buffer, variable);
    _strcat(buffer, "=");
    _strcat(buffer, value);
    node = shellInfo->envr;
    while (node)
    {
        p = startsWith(node->str, variable);
        if (p && *p == '=')
        {
            free(node->str);
            node->str = buffer;
            shellInfo->environmentChanged = 1;
            return 0;
        }
        node = node->next;
    }
    addNodeEnd(&(shellInfo->envr), buffer, 0);
    free(buffer);
    shellInfo->environmentChanged = 1;
    return 0;
}
