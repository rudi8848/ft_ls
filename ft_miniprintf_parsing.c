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

#include "ft_miniprintf.h"

int		ft_parse_flags(char *fp, t_options *options)
{
	int i;

	i = 0;
	while (fp[i] == '-' || fp[i] == '+' || fp[i] == ' '
			|| fp[i] == '#' || fp[i] == '0')
	{
		if (fp[i] == '+')
			options->show_sign = 1;
		else if (fp[i] == ' ')
			options->space_before = 1;
		else if (fp[i] == '#')
			options->show_prefix = 1;
		else if (fp[i] == '0')
			options->fill_by_zero = 1;
		else if (fp[i] == '-')
		{
			options->left_align = 1;
			options->fill_by_zero = 0;
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
/*
int		ft_parse_precision(char *fp, va_list *args, t_options *options)
{
	int i;
	int arg;

	i = 0;
	if (fp[i] && fp[i] == '.')
	{
		i++;
		options->is_set_precision = 1;
		if (ft_isdigit(fp[i]))
		{
			options->precision = ft_atoi(fp + i);
			while (ft_isdigit(fp[i]))
				i++;
		}
		if (fp[i] == '*')
		{
			arg = va_arg(*args, int);
			if (arg >= 0)
				options->precision = arg;
			else
				options->is_set_precision = 0;
			i++;
		}
	}
	return (i);
}

int		ft_parse_length(char *fp, t_options *options)
{
	int i;

	i = 0;
	while (fp[i] == 'h' || fp[i] == 'l' || fp[i] == 'j' || fp[i] == 'z')
	{
		if (fp[i] == 'h')
		{
			if (fp[i + 1] == 'h')
				options->len_hh = 1;
			options->len_h = 1;
		}
		if (fp[i] == 'l')
		{
			if (fp[i + 1] == 'l')
				options->len_ll = 1;
			options->len_l = 1;
		}
		if (fp[i] == 'j')
			options->len_j = 1;
		if (fp[i] == 'z')
			options->len_z = 1;
		i++;
	}
	return (i);
}
*/
ssize_t	ft_parse_options(const char **format, va_list *args, int *res)
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
	//fmtp += ft_parse_precision(fmtp, args, options);
	//fmtp += ft_parse_length(fmtp, options);
	if (check_type(*fmtp, options))
	{
		ft_transformer = ft_choose_type(*fmtp);
		ft_transformer(&fmtp, args, options, res);
	}
	else
		ft_miniprintf_putchar(&fmtp, NULL, options, res);
	if (options)
		free(options);
	return (fmtp - *format);
}
