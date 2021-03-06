/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_miniprintf_parsing.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvynogra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 15:41:22 by gvynogra          #+#    #+#             */
/*   Updated: 2018/03/25 15:58:11 by gvynogra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_miniprintf.h"

int		ft_parse_length(char *fp, t_options *options)
{
	int i;

	i = 0;
	if (fp[i] == 'h')
	{
		i++;
		options->len_h = 1;
	}
	if (fp[i] == 'l')
	{
		i++;
		options->len_l = 1;
	}
	return (i);
}

int		ft_parse_flags(char *fp, t_options *options)
{
	int i;

	i = 0;
	while (fp[i] == '-' || fp[i] == '+' || fp[i] == ' '
			|| fp[i] == '#')
	{
		if (fp[i] == '+')
			options->show_sign = 1;
		else if (fp[i] == ' ')
			options->space_before = 1;
		else if (fp[i] == '-')
		{
			options->left_align = 1;
		}
		i++;
	}
	if (options->show_sign && options->space_before)
		options->space_before = 0;
	return (i);
}

int		ft_parse_width(char *fp, va_list *args, t_options *options)
{
	int i;
	int arg;

	i = 0;
	while (ft_isdigit(fp[i]) || fp[i] == '*')
	{
		if (ft_isdigit(fp[i]))
		{
			options->width = ft_atoi(fp + i);
			while (ft_isdigit(fp[i]))
				i++;
		}
		if (fp[i] == '*')
		{
			arg = va_arg(*args, int);
			if (arg < 0)
			{
				arg = -arg;
				options->left_align = 1;
			}
			options->width = arg;
			i++;
		}
	}
	return (i);
}

size_t	ft_parse_options(const char **format, va_list *args, int *res)
{
	t_options	*options;
	char		*fmtp;
	t_pf		ft_transformer;

	fmtp = (char*)++(*format);
	options = (t_options*)ft_memalloc(sizeof(t_options));
	if (!options)
		exit(EXIT_FAILURE);
	fmtp += ft_parse_flags(fmtp, options);
	fmtp += ft_parse_width(fmtp, args, options);
	fmtp += ft_parse_length(fmtp, options);
	if (check_type(*fmtp))
	{
		ft_transformer = ft_choose_type(*fmtp);
		ft_transformer(args, options, res);
	}
	else
		ft_miniprintf_putchar(NULL, options, res);
	if (options)
		free(options);
	return (fmtp - *format);
}

int		ft_miniprintf(const char *format, ...)
{
	va_list		args;
	int			res;
	char		*ptr;

	res = 0;
	va_start(args, format);
	while (*format)
	{
		if (*format == '%')
		{
			ptr = (char*)format + 1;
			if (!*ptr)
				return (res);
			format += ft_parse_options(&format, &args, &res);
		}
		else
		{
			ft_putchar(*format);
			res++;
		}
		format++;
	}
	va_end(args);
	return (res);
}
