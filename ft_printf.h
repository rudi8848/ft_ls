/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvynogra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/16 17:09:25 by gvynogra          #+#    #+#             */
/*   Updated: 2018/03/16 17:09:35 by gvynogra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include "libft/includes/libft.h"
# include <stdarg.h>
# include <stdio.h>
# include <locale.h>
# include <wchar.h>
# include <inttypes.h>
# include <sys/types.h>
# define ERROR 1

typedef union	u_number
{
	intmax_t	i;
	uintmax_t	u;
}				t_number;

typedef struct	s_options
{
	int			left_align;
	int			show_sign;
	int			space_before;
	int			show_prefix;
	int			fill_by_zero;
	int			width;
	int			is_set_precision;
	int			precision;
	int			len_hh;
	int			len_h;
	int			len_l;
	int			len_ll;
	int			len_j;
	int			len_z;
}				t_options;

typedef ssize_t(*t_pf)(char **, va_list*, t_options*, int *res);

typedef enum
{
	CONV_s = 's',
	CONV_S = 'S',
	CONV_p = 'p',
	CONV_d = 'd',
	CONV_D = 'D',
	CONV_i = 'i',
	CONV_o = 'o',
	CONV_O = 'O',
	CONV_u = 'u',
	CONV_U = 'U',
	CONV_x = 'x',
	CONV_X = 'X',
	CONV_c = 'c',
	CONV_C = 'C',
	CONV_b = 'b',
	CONV_f = 'f',
	CONV_F = 'F',
	CONVERSIONS = 127
}	t_conv;

int				ft_printf(const char *format, ...);

#endif
