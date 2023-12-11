#include "shell.h"

/**
 * custom_shell - main shell loop
 * @shell_info: the parameter & return info structure
 * @arguments: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int custom_shell(shell_info_t *shell_info, char **arguments)
{
	ssize_t input_size = 0;
	int builtin_result = 0;

	while (input_size != -1 && builtin_result != -2)
	{
		clear_shell_info(shell_info);
		if (is_interactive(shell_info))
			_puts("$ ");
		_eputchar(BUF_FLUSH);
		input_size = get_user_input(shell_info);
		if (input_size != -1)
		{
			setup_shell_info(shell_info, arguments);
			builtin_result = find_and_execute_builtin(shell_info);
			if (builtin_result == -1)
				find_and_execute_command(shell_info);
		}
		else if (is_interactive(shell_info))
			_putchar('\n');
		free_shell_info(shell_info, 0);
	}
	write_shell_history(shell_info);
	free_shell_info(shell_info, 1);
	if (!is_interactive(shell_info) && shell_info->exit_status)
		exit(shell_info->exit_status);
	if (builtin_result == -2)
	{
		if (shell_info->error_number == -1)
			exit(shell_info->exit_status);
		exit(shell_info->error_number);
	}
	return (builtin_result);
}

/**
 * find_and_execute_builtin - finds and executes a builtin command
 * @shell_info: the parameter & return info structure
 *
 * Return: -1 if builtin not found,
 *         0 if builtin executed successfully,
 *         1 if builtin found but not successful,
 *         -2 if builtin signals exit()
 */
int find_and_execute_builtin(shell_info_t *shell_info)
{
	int index, builtin_result = -1;
	builtin_table builtins[] = {
		{"exit", _custom_exit},
		{"env", _custom_env},
		{"help", _custom_help},
		{"history", _custom_history},
		{"setenv", _custom_setenv},
		{"unsetenv", _custom_unsetenv},
		{"cd", _custom_cd},
		{"alias", _custom_alias},
		{NULL, NULL}
	};

	for (index = 0; builtins[index].type; index++)
		if (_strcmp(shell_info->arguments[0], builtins[index].type) == 0)
		{
			shell_info->line_count++;
			builtin_result = builtins[index].func(shell_info);
			break;
		}
	return (builtin_result);
}

/**
 * find_and_execute_command - finds and executes a command in PATH
 * @shell_info: the parameter & return info structure
 *
 * Return: void
 */
void find_and_execute_command(shell_info_t *shell_info)
{
	char *command_path = NULL;
	int i, token_count;

	shell_info->path = shell_info->arguments[0];
	if (shell_info->line_count_flag == 1)
	{
		shell_info->line_count++;
		shell_info->line_count_flag = 0;
	}
	for (i = 0, token_count = 0; shell_info->tokens[i]; i++)
		if (!is_delimiter(shell_info->tokens[i], " \t\n"))
			token_count++;
	if (!token_count)
		return;

	command_path = find_command_path(shell_info, _getenv(shell_info, "PATH="), shell_info->arguments[0]);
	if (command_path)
	{
		shell_info->path = command_path;
		fork_and_execute_command(shell_info);
	}
	else
	{
		if ((is_interactive(shell_info) || _getenv(shell_info, "PATH=")
			|| shell_info->arguments[0][0] == '/') && is_valid_command(shell_info, shell_info->arguments[0]))
			fork_and_execute_command(shell_info);
		else if (*(shell_info->tokens) != '\n')
		{
			shell_info->exit_status = 127;
			print_error(shell_info, "not found\n");
		}
	}
}

/**
 * fork_and_execute_command - forks an exec thread to run a command
 * @shell_info: the parameter & return info structure
 *
 * Return: void
 */
void fork_and_execute_command(shell_info_t *shell_info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(shell_info->path, shell_info->arguments, get_environment(shell_info)) == -1)
		{
			free_shell_info(shell_info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
	}
	else
	{
		wait(&(shell_info->exit_status));
		if (WIFEXITED(shell_info->exit_status))
		{
			shell_info->exit_status = WEXITSTATUS(shell_info->exit_status);
			if (shell_info->exit_status == 126)
				print_error(shell_info, "Permission denied\n");
		}
	}
}
