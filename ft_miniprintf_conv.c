/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_miniprintf_conv.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvynogra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 16:04:40 by gvynogra          #+#    #+#             */
/*   Updated: 2018/03/25 16:07:06 by gvynogra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_miniprintf.h"

void	ft_set_array(t_pf *convert_functions)
{
	convert_functions[CONV_c] = &ft_miniprintf_putchar;
	convert_functions[CONV_s] = &ft_miniprintf_putstr;
	convert_functions[CONV_d] = &ft_miniprintf_putnbr_sdec;
	convert_functions[CONV_i] = &ft_miniprintf_putnbr_sdec;
}

t_pf	ft_choose_type(t_conv conv)
{
	static t_pf *convert_functions;

	if (!convert_functions)
	{
		convert_functions = (t_pf*)ft_memalloc(sizeof(t_pf) * CONVERSIONS);
		if (!convert_functions)
			exit(-1);
		ft_set_array(convert_functions);
	}
	return (convert_functions[conv]);
}

int		check_type(char c, t_options *options)
{
	if (c == 's' || c == 'S' || c == 'c' || c == 'C')
		return (1);
	else if (c == 'd' || c == 'i')
		return (1);
	else if (c == 'o' || c == 'u' || c == 'x' || c == 'X')
		return (1);
	else if (c == 'D' || c == 'U' || c == 'O' || c == 'p')
	{
		options->len_l = 1;
		return (1);
	}
	else
		return (0);
}
