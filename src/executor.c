/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deordone <deordone@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 12:55:14 by deordone          #+#    #+#             */
/*   Updated: 2024/03/21 17:57:21 by deordone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.c"
/*
static int child_process(t_block *block)
{
	if (block->in != STDIN)
		printf("hacer redireccion de la entrada");
	if (block->out != STDOUT)	
		printf("hacer redireccion de la salida");
	execve(path, cmd, env);
}*/

static char	*ft_aux_check(char *new_path, char *new_cmd)
{
	if (access(new_cmd, F_OK | X_OK) == 0)
		return (new_cmd);
	else if (access(new_cmd, X_OK) == -1)
	{
		free(new_path);
		free(new_cmd);
		return (NULL);
	}
	else
	{
		free(new_path);
		free(new_cmd);
	}
	return (NULL);
}

static char	*ft_check_path(char **paths, char **cmd)
{
	int		i;
	int		j;
	char	*new_cmd;
	char	*new_path;

	i = 0;
	j = 0;
	if (access(cmd[0], X_OK) == 0)
		return (ft_strdup(cmd[0]));
	while (paths[i] != NULL)
		i++;
	while (i > j)
	{
		new_path = ft_strjoin(paths[j], "/");
		new_cmd = ft_strjoin(new_path, cmd[0]);
		new_cmd = ft_aux_check(new_path, new_cmd);
		if (new_cmd)
		{
			free(new_path);
			return (new_cmd);
		}
		j++;
	}
	return (NULL);
}

static void find_path(t_block *block)
{
	char **paths;
	char *path;
	int i;
	
	i = -1;
	path = "PATH:g";
	paths = ft_split(path, ':');
	
 /*	por si sale mal el get_env
	  while (sh->env[++i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			paths = ft_split(envp[i], ':');
			break ;
		}
	}*/
	block->path = ft_check_path(paths, block->cmd);
	print_blocks(block);
}

void	execute_cmd(t_block *block)
{
	find_path(block);
/*	pid_t pid;

	pid = fork();
	if (!fork)
		exit (1);
	if (pid == 0)
	{
		child_process(block);
	}
	else
	{
		//esperar al hijo
	}*/
}

void	executor(t_shell *sh, char **env)
{
	t_block *block;

	(void)env;
	block = sh->block;
	while (block)
	{
		//if (block->type == B_PIPE)	
		//if (block->type == B_REDIR)	
		//if (block->type == B_BUILT)	
		//if (block->type == B_SQUOTES)	
		//if (block->type == B_DQUOTES)	
		if (block->type == B_CMD)	
			execute_cmd(block);
		//if (block->type == B_FILE)	
		block = block->next;
	}	
}
