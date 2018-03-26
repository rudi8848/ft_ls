/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_miniprintf_num_align.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvynogra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/26 16:29:42 by gvynogra          #+#    #+#             */
/*   Updated: 2018/03/26 16:29:46 by gvynogra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_miniprintf.h"

int			ft_check_options(intmax_t nbr, t_options options)
{
	int			ret;

	ret = 0;
	if (options.space_before && nbr > 0)
	{
		ft_putchar(' ');
		ret++;
	}
	else if (options.show_sign && nbr >= 0)
	{
		ft_putchar('+');
		ret++;
	}
	return (ret);
}

int			ft_right_align(intmax_t nbr, t_options options, int len)
{
	int			ret;

	ret = 0;
	ret += ft_check_options(nbr, options);
	ret += fillnchar(len + ret, options.width, ' ');
	print_sdec(nbr);
	ret += len;
	return (ret);
}

int			ft_left_align(intmax_t nbr, t_options options, int len)
{
	int			ret;

	ret = 0;
	ret += ft_check_options(nbr, options);
	print_sdec(nbr);
	ret += len;
	ret += fillnchar(ret, options.width, ' ');
	return (ret);
}
