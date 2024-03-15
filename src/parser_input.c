/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 17:15:46 by deordone          #+#    #+#             */
/*   Updated: 2024/03/15 14:05:59 by deordone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_great_less_case(t_token *tok)
{
	if (tok->type != PIPE)
	{
		if ((tok->next && is_redir(tok->next->type) > 0) && (tok->prev
				&& is_redir(tok->prev->type) > 0))
		{
			ft_dprintf(2, "Parse error near '%s'\n", tok->next->data);
			return (-1);
		}
		else if (!tok->next)
		{
			ft_dprintf(2, "Parse error near '\\n'\n", tok->data);
			return (-1);
		}
	}
	return (0);
}

static int	check_pipe_case(t_token *tok)
{
	if (tok->type == PIPE)
	{
		if (tok->prev && is_redir(tok->prev->type) > 0)
		{
			ft_dprintf(2, "Parse error near '%s'\n", tok->data);
			return (-1);
		}
		else if (tok->next && is_redir(tok->next->type) > 0 && tok->next->type != PIPE)
		{
			return (check_great_less_case(tok->next));
		}
		else if (!tok->prev || tok->data[0] == tok->data[1])
		{
			ft_dprintf(2, "Parse error near '%c%c'\n", tok->data[0], tok->data[1]);
			return (-1);
		}
	}
	return (0);
}

int	syntax_error(t_token *tok)
{
	while (tok)
	{
		if (is_redir(tok->type) > 0)
		{
			if (check_great_less_case(tok) < 0)
				return (-1);
			if (check_pipe_case(tok) < 0)
				return (-1);
		}
		tok = tok->next;
	}
	return (0);
}

void   incomplete_entry(t_shell *sh)
{
        char *new_input;
        char *last_input;

        new_input = readline(GREEN"\n> "NC);
        last_input = add_space(new_input);
        free(new_input);
        sh->line = ft_imp_strjoin(sh->line, last_input);
        ft_deltoken(&sh->tokens);
        sh->tokens = generate_tokens(sh->line);
}


int input_incomplete(t_shell *sh)
{
	t_token *tmp_tok;

	tmp_tok = sh->tokens;
	while (tmp_tok)
	{
		if (tmp_tok->type == PIPE && !tmp_tok->next)
			return (-1);
		tmp_tok = tmp_tok->next;
	}
	return (0);
}

