#include "shell.h"

/**
 * custom_memset - fills memory with a constant byte
 * @memory: the pointer to the memory area
 * @byte: the byte to fill *memory with
 * @size: the number of bytes to be filled
 * Return: (memory) a pointer to the memory area memory
 */
char *custom_memset(char *memory, char byte, unsigned int size)
{
	unsigned int i;

	for (i = 0; i < size; i++)
		memory[i] = byte;
	return (memory);
}

/**
 * free_string_array - frees a string of strings
 * @string_array: string of strings
 */
void free_string_array(char **string_array)
{
	char **temp = string_array;

	if (!string_array)
		return;
	while (*string_array)
		free(*string_array++);
	free(temp);
}

/**
 * custom_realloc - reallocates a block of memory
 * @ptr: pointer to previous malloc'ated block
 * @old_size: byte size of previous block
 * @new_size: byte size of new block
 * Return: pointer to the old block if reallocation fails, otherwise, pointer to the new block
 */
void *custom_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	char *new_ptr;

	if (!ptr)
		return (malloc(new_size));
	if (!new_size)
		return (free(ptr), NULL);
	if (new_size == old_size)
		return (ptr);

	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);

	old_size = old_size < new_size ? old_size : new_size;
	while (old_size--)
		new_ptr[old_size] = ((char *)ptr)[old_size];
	free(ptr);
	return (new_ptr);
}
