/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_conec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deordone <deordone@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 17:18:48 by deordone          #+#    #+#             */
/*   Updated: 2024/04/22 17:54:53 by deordone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	kill_child(t_process *pro, char **env)
{
	ft_dprintf(2, "in -> %d\n", pro->in);
	if (pro->in != STD_IN)
	{
		if (dup2(pro->in, STD_IN) == -1)
		{
			BUG;
			exit(1);
		}
		close(pro->in);
	}
	else
		close(pro->p[0]);
	ft_dprintf(2, "out -> %d\n", pro->out);
	if (pro->out != STD_OUT)
	{
		if (dup2(pro->out, STD_OUT) == -1)
			exit(1);
		close(pro->out);
	}
	else
		close(pro->p[1]);
	execve(pro->words->path, pro->words->cmd, env);
	exit(after_exec(pro->words));
}

static void	child_process(t_process *pro, char **env)
{
	if (pro->words)
	{
		if (char_is_inside(pro->words->cmd[0], '/') < 0)
			find_path(pro->words);
		else
			pro->words->path = ft_strdup(pro->words->cmd[0]);
	}
	kill_child(pro, env);
}

static void before_fork(int process, t_process *pro, t_shell *sh)
{
	if (process <= sh->pipes)
	{
		pro->in	= pro->p[0];
		if (pipe(pro->p) < 0)
			exit(EXIT_FAILURE);
		pro->out = pro->p[1];
	}
	if (process == 0)
		pro->in = 0;
	else if (process == sh->pipes)
		pro->out = 1;
	//hacer redirecciones
}

static void after_fork(int process, t_process *pro)
{
	(void)process;
	if (pro->in != STD_IN)
		close(pro->in);
	//else
	//	close(pro->p[0]);
	if (pro->out != STD_OUT)
		close(pro->out);
	else
		close(pro->p[1]);
	while (pro->redir && pro->redir->type != PIPE)
		pro->redir = pro->redir->next;
	pro->words = pro->words->next;			
}

int process_connector(t_shell *sh, int process, char **env, int *fds)
{
	(void)fds;
	t_process pro;
	pid_t pid;
	
	pro.in = 0;
	pro.words = sh->words;
	pro.redir = sh->redir;
	process = -1;
	while (++process <= sh->pipes)
	{
		before_fork(process, &pro, sh);
		pid = fork();
		if (pid == -1)
			exit(1);
		if (pid == 0)
		{
			if (process > 0)
				close(pro.p[0]);
			child_process(&pro, env);
		}
		after_fork(process, &pro);
	}
	close(pro.p[0]);
	while(pid > 0)
		pid = waitpid (-1, &pro.wstatus, 0);
	if (WIFEXITED(pro.wstatus))
		return (WEXITSTATUS(pro.wstatus));
	return (0);
}
