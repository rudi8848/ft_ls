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
# include "../libft/includes/libft.h"
# include <stdarg.h>
# include <stdio.h>
# include <inttypes.h>
# include <sys/types.h>
# define ERROR 1

typedef struct	s_options
{
	int			left_align;
	int			show_sign;
	int			space_before;
	int			len_l;
	int			len_h;
	int			width;
}				t_options;

typedef size_t(*t_pf)(va_list*, t_options*, int *res);

typedef enum
{
	CONV_s = 's',
	CONV_d = 'd',
	CONV_i = 'i',
	CONV_c = 'c',
	CONVERSIONS = 127
}	t_conv;

int				ft_miniprintf(const char *format, ...);
size_t			ft_parse_options(const char **format, va_list *args, int *res);
int				ft_parse_width(char *fp, va_list *args, t_options *options);
int				ft_parse_length(char *fp, t_options *options);
int				ft_parse_flags(char *fp, t_options *options);
int				check_type(char c);
t_pf			ft_choose_type(t_conv conv);
void			ft_set_array(t_pf *convert_functions);
int				fillnchar(int len, int width, char c);
int				ft_print_null_string(void);
size_t			ft_miniprintf_putstr(va_list *args, t_options *opt, int *res);
size_t			ft_miniprintf_putchar(va_list *args, t_options *opt, int *res);
intmax_t		ft_cut_signed(va_list *args, t_options *options);
int				ft_snbr_length(intmax_t *n, int base);
void			print_sdec(intmax_t n);
size_t			ft_miniprintf_putnbr_sdec(va_list *arg, t_options *op, int *rs);
int				ft_check_options(intmax_t nbr, t_options options);
int				ft_right_align(intmax_t nbr, t_options options, int len);
int				ft_left_align(intmax_t nbr, t_options options, int len);

#endif
