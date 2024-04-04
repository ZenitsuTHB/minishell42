/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deordone <deordone@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 05:40:25 by deordone          #+#    #+#             */
/*   Updated: 2024/04/04 17:51:04 by deordone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

static int smpl_cmd(t_shell *sh, int *fds, char **env)
{
	if (sh->redir)
		fds = process_redir(sh->redir, fds);
	if (fds[0] == -1 || fds[1] == -1)
		return (EXIT_FAILURE);
	else
		word->in = fds[0];
		word->out = fds[1];
	if (sh->words)
			return (process_word(sh->words, fds, env));
	return (EXIT_SUCCESS);
}

static int connector(t_shell *sh, int *fds, char **env)
{
	t_words *word;
	t_redir *redir;
	int process;
	int final;

	process = sh->pipes + 1;
	while (process)
	{
		if (redir)
		{
			fds = process_redir(redir, fds);
			redir = redir->next;
		}
		if (fds[0] == -1 || fds[1] == -1)
			final = EXIT_FAILURE;
		else
			word->in = fds[0];
			word->out = fds[1];
		if (word)
		{
			final = process_connector(word, fds, env);
			word = word->next;
		}
		process--;
	}
	return (EXIT_SUCCESS);
}

void	executor(t_shell *sh, char **env)
{
	int *fds;

	fds = malloc(sizeof(int) * 2);
	if (!fds)
		exit(1);
	if (sh->pipes == 0)
		sh->exit_status = smpl_cmd(sh, fds, env);
	else
		sh->exit_status = connector(sh, fds, env);
	free(fds);
}
