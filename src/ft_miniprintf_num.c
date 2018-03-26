/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_miniprintf.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvynogra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/26 12:51:43 by gvynogra          #+#    #+#             */
/*   Updated: 2018/03/26 12:52:27 by gvynogra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_miniprintf.h"

intmax_t	ft_cut_signed(va_list *args, t_options *options)
{
	intmax_t	nbr;

	nbr = va_arg(*args, intmax_t);
	if (options->len_h && !options->len_l)
		nbr = (short)nbr;
	else if (options->len_l)
		nbr = (long)nbr;
	else
		nbr = (int)nbr;
	return (nbr);
}

int			ft_snbr_length(intmax_t *n, int base)
{
	int			len;
	intmax_t	nbr;

	len = 0;
	nbr = 0;
	if (*n == 0)
		len++;
	else if (*n < 0)
	{
		len++;
		nbr = -*n;
	}
	else
		nbr = *n;
	while (nbr)
	{
		nbr /= base;
		len++;
	}
	return (len);
}

void		print_sdec(intmax_t n)
{
	uintmax_t	nbr;

	if (n < 0)
	{
		ft_putchar('-');
		nbr = -n;
	}
	else
		nbr = n;
	if (nbr >= 10)
	{
		print_sdec(nbr / 10);
		ft_putchar(nbr % 10 + '0');
	}
	else
		ft_putchar(nbr + '0');
}

size_t		ft_miniprintf_putnbr_sdec(va_list *args, t_options *opt, int *res)
{
	intmax_t	nbr;
	int			len;
	int			ret;

	ret = 0;
	nbr = ft_cut_signed(args, opt);
	len = ft_snbr_length(&nbr, 10);
	if (opt->width > len && !opt->left_align)
		ret += ft_right_align(nbr, *opt, len);
	else if (opt->width > len && opt->left_align)
		ret += ft_left_align(nbr, *opt, len);
	else
	{
		ret += ft_check_options(nbr, *opt);
		print_sdec(nbr);
		ret += len;
	}
	*res += ret;
	return (ret);
}
