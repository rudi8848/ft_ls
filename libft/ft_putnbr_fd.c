/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvynogra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 15:59:06 by gvynogra          #+#    #+#             */
/*   Updated: 2017/11/06 15:59:28 by gvynogra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

static int		ft_isnegative(int n)
{
	if (n < 0)
		return (1);
	return (0);
}

void			ft_putnbr_fd(int n, int fd)
{
	int neg;
	int nbr;

	if (n == -2147483648)
		write(fd, "-2147483648", 11);
	else
	{
		nbr = n;
		neg = ft_isnegative(n);
		if (neg)
		{
			nbr = -n;
			ft_putchar_fd('-', fd);
		}
		if (nbr >= 10)
		{
			ft_putnbr_fd(nbr / 10, fd);
			ft_putnbr_fd(nbr % 10, fd);
		}
		else
			ft_putchar_fd(nbr + '0', fd);
	}
}
