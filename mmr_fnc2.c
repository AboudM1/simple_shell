#include "shell.h"

/**
 * custom_free_and_null - frees a pointer and NULLs the address
 * @pointer: address of the pointer to free
 *
 * Return: 1 if freed, otherwise 0.
 */
int custom_free_and_null(void **pointer)
{
	if (pointer && *pointer)
	{
		free(*pointer);
		*pointer = NULL;
		return (1);
	}
	return (0);
}
