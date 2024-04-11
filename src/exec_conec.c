/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_conec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deordone <deordone@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 17:18:48 by deordone          #+#    #+#             */
/*   Updated: 2024/04/09 17:23:15 by deordone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_process(t_words *word, int *p, char **env)
{
	if (char_is_inside(word->cmd[0], '/') < 0)
		find_path(word);
	else
		word->path = ft_strdup(word->cmd[0]);
	if (dup2(word->in, STD_IN) == -1)
		exit(-1);
	close(p[0]);
	if (dup2(p[1], STD_OUT) == -1)
		exit(-1);
	close(p[1]);
	execve(word->path, word->cmd, env);
	exit(after_exec(word));
}

static void	parent_process(t_words *word, int *p, char **env)
{
	if (char_is_inside(word->cmd[0], '/') < 0)
		find_path(word);
	else
		word->path = ft_strdup(word->cmd[0]);
	if (dup2(word->out, STD_OUT) == -1)
		exit(-1);
	//close(word->out);
	close(p[1]);
	if (dup2(p[0], STD_IN) == -1)
		exit(-1);
	close(p[0]);
	execve(word->path, word->cmd, env);
	exit(after_exec(word));
}

int process_connector(t_words *word, char **env)
{
	pid_t pid;
	int	p[2];
	int parent_aux;
	int exit_status;

	exit_status = 0;	
	if (pipe(p))
		exit(1);
	pid = fork();
	if (pid == -1)
		exit(1);
	if (pid > 0)
		waitpid(0, &parent_aux, 0);
	else
	{
		pid = fork();
		if (pid == -1)
			exit(-1);	
		if (pid > 0)
			parent_process(word->next, p, env);
		else
			child_process(word, p, env);
	}
	if (WIFEXITED(parent_aux))
		exit_status = WEXITSTATUS(parent_aux);
	return (exit_status);
}