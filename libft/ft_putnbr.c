/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvynogra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 15:51:38 by gvynogra          #+#    #+#             */
/*   Updated: 2017/11/06 15:52:15 by gvynogra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

static int		ft_isnegative(int n)
{
	if (n < 0)
		return (1);
	return (0);
}

void			ft_putnbr(int n)
{
	int neg;
	int nbr;

	if (n == -2147483648)
		write(1, "-2147483648", 11);
	else
	{
		nbr = n;
		neg = ft_isnegative(n);
		if (neg)
		{
			nbr = -n;
			ft_putchar('-');
		}
		if (nbr >= 10)
		{
			ft_putnbr(nbr / 10);
			ft_putnbr(nbr % 10);
		}
		else
			ft_putchar(nbr + '0');
	}
}
