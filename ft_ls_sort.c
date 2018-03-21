/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvynogra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 17:30:19 by gvynogra          #+#    #+#             */
/*   Updated: 2018/03/20 17:30:55 by gvynogra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int				ft_cmp_ascending(int a, int b)
{
	return (a >= b);
}

int				ft_cmp_descending(int a, int b)
{
	return (a < b);
}

t_flist			*ft_sort_flist(t_opt options, t_flist *head)
{
	t_flist			*a;
	t_pf_compare	cmp[SORT_ORDERS];

	a = NULL;
	cmp[SO_ASC] = ft_cmp_ascending;
	cmp[SO_DESC] = ft_cmp_descending;
	if (!options.r && !options.t)
		head = ft_sort_by_name(a, head, cmp[SO_ASC]);
	else if (options.r && !options.t)
		head = ft_sort_by_name(a, head, cmp[SO_DESC]);
	else if (options.t && !options.r)
		head = ft_sort_by_mtime(a, head, cmp[SO_ASC]);
	else if (options.t && options.r)
		head = ft_sort_by_mtime(a, head, cmp[SO_DESC]);
	return (head);
}

t_flist			*ft_sort_by_mtime(t_flist *a, t_flist *head, t_pf_compare cmp)
{
	t_flist			*b;
	t_flist			*c;

	while (head != NULL)
	{
		if (head->next)
			b = head;
		else
			return (a);
		head = head->next;
		if (a == NULL || cmp(b->mtime, a->mtime))
		{
			b->next = a;
			a = b;
		}
		else
		{
			c = a;
			while (c->next != NULL && !cmp(b->mtime, c->next->mtime))
				c = c->next;
			b->next = c->next;
			c->next = b;
		}
	}
	return (a);
}

t_flist			*ft_sort_by_name(t_flist *a, t_flist *head, t_pf_compare cmp)
{
	t_flist			*b;
	t_flist			*c;

	while (head != NULL)
	{
		if (head->next)
			b = head;
		else
			return (a);
		head = head->next;
		if (a == NULL || cmp(0, ft_strcmp(b->name, a->name)))
		{
			b->next = a;
			a = b;
		}
		else
		{
			c = a;
			while (c->next && !cmp(0, ft_strcmp(b->name, c->next->name)))
				c = c->next;
			b->next = c->next;
			c->next = b;
		}
	}
	return (a);
}
