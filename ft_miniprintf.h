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

#ifndef FT_MINIPRINTF_H
# define FT_MINIPRINTF_H
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

int				ft_miniprintf(const char *format, ...);

ssize_t	ft_parse_options(const char **format, va_list *args, int *res);
int		ft_parse_length(char *fp, t_options *options);
int		ft_parse_precision(char *fp, va_list *args, t_options *options);
int		ft_parse_width(char *fp, va_list *args, t_options *options);
int		ft_parse_flags(char *fp, t_options *options);
int check_type(char c, t_options *options);
t_pf	ft_choose_type(t_conv conv);
void	ft_set_array(t_pf *convert_functions);
ssize_t		ft_miniprintf_putchar(char **fmt, va_list *args, t_options *options, int *res);
int	fillnchar(int len, int width, char c);
ssize_t		ft_miniprintf_putstr(char **fmt, va_list *args, t_options *options, int *res);
ssize_t		ft_miniprintf_putchar(char **fmt, va_list *args, t_options *options, int *res);
uintmax_t	ft_cut_unsigned(va_list *args, t_options *options);
intmax_t	ft_cut_signed(va_list *args, t_options *options);
int		ft_unbr_length(uintmax_t *n,int  base);
int		ft_snbr_length(intmax_t *n,int  base);
void	print_oct(uintmax_t n);
void	print_hex(uintmax_t n, char a);
void	print_sdec(intmax_t n);
void	print_udec(uintmax_t n);
ssize_t	ft_miniprintf_putnbr_oct(char **fmt, va_list *args, t_options *options, int *res);
ssize_t	ft_miniprintf_putnbr_hex(char **fmt, va_list *args, t_options *options, int *res);
ssize_t	ft_miniprintf_putnbr_sdec(char **fmt, va_list *args, t_options *options, int *res);
ssize_t	ft_miniprintf_putnbr_udec(char **fmt, va_list *args, t_options *options, int *res);



#endif
