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

#include "../includes/ft_miniprintf.h"

int			ft_print_null_string(void)
{
	write(1, "(null)", 6);
	return (6);
}

int			fillnchar(int len, int width, char c)
{
	int			i;
	char		str[256];

	i = 0;
	while (len < width)
	{
		str[i] = c;
		len++;
		i++;
	}
	return (write(1, str, i));
}

size_t		ft_miniprintf_putstr(va_list *args, t_options *options, int *res)
{
	int			len;
	int			ret;
	char		*str;

	str = (char*)va_arg(*args, const char*);
	ret = 0;
	len = ft_strlen(str);
	if (len < options->width && !options->left_align)
	{
		ret += fillnchar(len, options->width, ' ');
		ft_putstr(str);
	}
	else if (len < options->width && options->left_align)
	{
		ft_putstr(str);
		ret += fillnchar(len, options->width, ' ');
	}
	else
		ft_putstr(str);
	ret += len;
	*res += ret;
	return (len);
}

size_t		ft_miniprintf_putchar(va_list *args, t_options *options, int *res)
{
	int			symb;
	int			ret;

	ret = 0;
	symb = va_arg(*args, int);
	if (options->width && !options->left_align)
	{
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
