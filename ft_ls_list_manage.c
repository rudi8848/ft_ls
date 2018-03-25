/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls_list_manage.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvynogra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 13:29:00 by gvynogra          #+#    #+#             */
/*   Updated: 2018/03/25 13:29:50 by gvynogra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_flist			*ft_get_nth(t_flist *head, int n)
{
	int			i;

	i = 0;
	while (i < n && head)
	{
		head = head->next;
		i++;
	}
	return (head);
}

long			ft_count_blocks(t_flist *head)
{
	long		total;

	total = 0;
	while (head)
	{
		total += head->blocks;
		head = head->next;
	}
	return (total);
}

void			ft_clean_flist(t_opt options, t_flist **file)
{
	ft_strdel(&(*file)->name);
	ft_strdel(&(*file)->path);
	if (options.l)
	{
		ft_strdel(&(*file)->user);
		ft_strdel(&(*file)->group);
		ft_strdel(&(*file)->date);
	}
	if ((*file)->ref[0] != '\0')
		ft_strdel(&(*file)->ref);
}

void			ft_delete_flist(t_opt options, t_flist **head)
{
	t_flist		*tmp;

	while ((*head))
	{
		tmp = (*head);
		(*head) = (*head)->next;
		ft_clean_flist(options, &tmp);
		free(tmp);
	}
	if (*head)
	{
		ft_clean_flist(options, head);
		free(head);
		(*head) = NULL;
	}
}

int				ft_flist_counter(t_flist *head)
{
	int			i;

	i = 0;
	if (!head)
		return (0);
	while (head)
	{
		head = head->next;
		i++;
	}
	return (i);
}
