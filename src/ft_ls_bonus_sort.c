/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls_bonus_sort.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvynogra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/26 15:40:23 by gvynogra          #+#    #+#             */
/*   Updated: 2018/03/26 15:40:27 by gvynogra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

t_flist			*ft_sort_by_size(t_flist *a, t_flist *head, t_pf_compare cmp)
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
		if (a == NULL || cmp(b->size, a->size))
		{
			b->next = a;
			a = b;
		}
		else
		{
			c = a;
			while (c->next != NULL && !cmp(b->size, c->next->size))
				c = c->next;
			b->next = c->next;
			c->next = b;
		}
	}
	return (a);
}
