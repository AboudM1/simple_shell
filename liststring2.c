#include "shell.h"

/**
 * list_length - determines the length of linked list
 * @first_node: pointer to the first node
 *
 * Return: size of the list
 */
size_t list_length(const list_t *first_node)
{
	size_t size = 0;

	while (first_node)
	{
		first_node = first_node->next;
		size++;
	}
	return (size);
}

/**
 * list_to_string_array - returns an array of strings of the list->str
 * @head: pointer to the first node
 *
 * Return: array of strings
 */
char **list_to_string_array(list_t *head)
{
	list_t *node = head;
	size_t i = list_length(head), j;
	char **strings;
	char *str_value;

	if (!head || !i)
		return (NULL);
	strings = malloc(sizeof(char *) * (i + 1));
	if (!strings)
		return (NULL);
	for (i = 0; node; node = node->next, i++)
	{
		str_value = malloc(_strlen(node->str) + 1);
		if (!str_value)
		{
			for (j = 0; j < i; j++)
				free(strings[j]);
			free(strings);
			return (NULL);
		}

		str_value = _strcpy(str_value, node->str);
		strings[i] = str_value;
	}
	strings[i] = NULL;
	return (strings);
}

/**
 * print_list_nodes - prints all elements of a list_t linked list
 * @first_node: pointer to the first node
 *
 * Return: size of the list
 */
size_t print_list_nodes(const list_t *first_node)
{
	size_t size = 0;

	while (first_node)
	{
		_puts(convert_number(first_node->node_index, 10, 0));
		_putchar(':');
		_putchar(' ');
		_puts(first_node->str ? first_node->str : "(nil)");
		_puts("\n");
		first_node = first_node->next;
		size++;
	}
	return (size);
}

/**
 * find_node_starts_with - returns node whose string starts with a prefix
 * @head: pointer to list head
 * @prefix: string to match
 * @next_char: the next character after the prefix to match
 *
 * Return: matching node or NULL
 */
list_t *find_node_starts_with(list_t *head, char *prefix, char next_char)
{
	char *p = NULL;

	while (head)
	{
		p = starts_with(head->str, prefix);
		if (p && ((next_char == -1) || (*p == next_char)))
			return (head);
		head = head->next;
	}
	return (NULL);
}

/**
 * get_node_index - gets the index of a node
 * @head: pointer to list head
 * @node: pointer to the node
 *
 * Return: index of the node or -1
 */
ssize_t get_node_index(list_t *head, list_t *node)
{
	size_t index = 0;

	while (head)
	{
		if (head == node)
			return (index);
		head = head->next;
		index++;
	}
	return (-1);
}
