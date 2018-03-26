/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_miniprintf_char.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvynogra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/26 09:52:43 by gvynogra          #+#    #+#             */
/*   Updated: 2018/03/26 09:52:46 by gvynogra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_miniprintf.h"

int		ft_print_null_string(void)
{
	write(1, "(null)", 6);
	return (6);
}

size_t		ft_miniprintf_putstr(va_list *args, t_options *options, int *res)
{
	int len = 0;
	int ret = 0;

	char *str = (char*)va_arg(*args, const char*);
	if (str)
	{
		len = ft_strlen(str);
		if (len < options->width && !options->left_align)
		{
			ret += fillnchar(len, options->width, ' ');
			ft_putstr(str);
			ret += len;
		}
		else if (len < options->width && options->left_align)
		{
			ft_putstr(str);
			ret += len;
			ret += fillnchar(ret, options->width, ' ');
		}
		else
		{
			ft_putstr(str);
			ret += len;
		}
		*res += ret;
	}
	return (len);
}


size_t		ft_miniprintf_putchar(va_list *args, t_options *options, int *res)
{
	int symb;
	int ret = 0;

	symb = va_arg(*args, int);
	if (options->width && !options->left_align)
	{
		if (options->fill_by_zero)
			ret += fillnchar(1, options->width, '0');
		else
			ret += fillnchar(1, options->width, ' ');
		ft_putchar(symb);
		ret += 1;
	}
	else
	{
		ft_putchar(symb);
		ret += 1;
	}
	if (options->width && options->left_align)
		ret += fillnchar(1, options->width, ' ');
	*res += ret;
	return (ret);
}
