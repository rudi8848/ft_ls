#include "ft_miniprintf.h"
  
int		ft_print_null_string(void)
{
	write(1, "(null)", 6);
	return (6);
}

ssize_t		ft_miniprintf_putstr(char **fmt, va_list *args, t_options *options, int *res)
{
	int len;
	int ret = 0;

	len = 0;
	if (!fmt)		//<-------- to do
		exit(ERROR);
	char *str = (char*)va_arg(*args, const char*);
	char *tmp = NULL;
	if (str)
	{
		len = ft_strlen(str);
		if (options->is_set_precision && options->precision < len)
		{
			tmp = (char*)ft_memalloc((options->precision + 1)*sizeof(char));
			tmp = ft_strncpy(tmp, str, options->precision);
			len = options->precision;
		}
		if (len < options->width && !options->left_align)
		{
			if (options->fill_by_zero)
				ret += fillnchar(len, options->width, '0');
			else
				ret += fillnchar(len, options->width, ' ');
			tmp ? ft_putstr(tmp) : ft_putstr(str);
			ret += len;
		}
		else if (len < options->width && options->left_align)
		{
			tmp ? ft_putstr(tmp) : ft_putstr(str);
			ret += len;
			ret += fillnchar(ret, options->width, ' ');
		}
		else
		{
			tmp ? ft_putstr(tmp) : ft_putstr(str);
			ret += len;
		}
		*res += ret;
		if (tmp)
			free(tmp);
	}
	else
		(options->width && options->is_set_precision) ? (*res += fillnchar(0, options->width, '0')) : (*res += ft_print_null_string());
	return (len);
}


ssize_t		ft_miniprintf_putchar(char **fmt, va_list *args, t_options *options, int *res)
{
	int symb;
	char *ptr;
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

uintmax_t	ft_cut_unsigned(va_list *args, t_options *options)
{
	uintmax_t nbr;

	nbr = va_arg(*args, uintmax_t);
	if (options->len_l || options->len_ll)
	{
		options->len_h = 0;
		options->len_hh = 0;
	}
	if (options->len_hh)
		nbr = (unsigned char)nbr;
	else if (options->len_h)
		nbr = (unsigned short)nbr;
	else if (options->len_l)
		nbr = (unsigned long)nbr;
	else if (options->len_ll)
		nbr = (unsigned long long)nbr;
	else if (options->len_j)
		nbr = (uintmax_t)nbr;
	else if (options->len_z)
		nbr = (size_t)nbr;
	else
		nbr = (unsigned)nbr;
	return(nbr);
}

intmax_t	ft_cut_signed(va_list *args, t_options *options)
{
	intmax_t nbr;

	nbr = va_arg(*args, intmax_t);
	if (options->len_l || options->len_ll)
	{
		options->len_h = 0;
		options->len_hh = 0;
	}
	if (options->len_hh && !options->len_l)
		nbr = (char)nbr;
	else if (options->len_h && !options->len_l)
		nbr = (short)nbr;
	else if (options->len_l)
		nbr = (long)nbr;
	else if (options->len_ll)
		nbr = (long long)nbr;
	else if (options->len_j)
		nbr = (intmax_t)nbr;
	else if (options->len_z)
		nbr = (ssize_t)nbr;
	else
		nbr = (int)nbr;
	return (nbr);
}

int		ft_unbr_length(uintmax_t *n,int  base)
{
	int len;
	uintmax_t nbr;

	len = 0;
	nbr = 0;
	if (*n == 0)
		len++;
	else
	{
		nbr = *n;
		while (nbr)
		{
			nbr /= base;
			len++;
		}
	}
	return (len);
}

int		ft_snbr_length(intmax_t *n,int  base)
{
	int len;
	intmax_t nbr;

	len = 0;
	nbr = 0;
	if (*n == 0)
		len++;
	else if (*n < 0)
	{
		if (base == 10)
			len++;
		nbr = - *n;
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

void	print_oct(uintmax_t n)
{
	if (n >= 8)
	{
		print_oct(n >> 3);
		ft_putchar(n % 8 + '0');
	}
	else
		ft_putchar(n + '0');
}

void	print_hex(uintmax_t n, char a)
{
	if (n >= 16)
	{
		print_hex(n >> 4, a);
		print_hex(n % 16, a);
	}
	else if (n >= 10 && n <= 15)
		ft_putchar(n - 10 + a);
	else
		ft_putchar(n + '0');
}


void	print_sdec(intmax_t n)
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

void	print_udec(uintmax_t n)
{	
	if (n >= 10)
	{
		print_udec(n / 10);
		ft_putchar(n % 10 + '0');
	}
	else
		ft_putchar(n + '0');
}

int		fillnchar(int len, int width, char c)
{
	int i = 0;
	char str[256];	//a little faster than output in loop

	while (len < width)
	{
		str[i] = c;
		len++;
		i++;
	}
	return (write(1, str, i));
}

ssize_t	ft_miniprintf_putnbr_oct(char **fmt, va_list *args, t_options *options, int *res)
{
	uintmax_t	nbr;
	int		len;
	int 		ret = 0;
	if (!fmt)
		exit(ERROR);
	nbr = ft_cut_unsigned(args, options);
	if (!nbr && options->is_set_precision &&!options->precision && !options->show_prefix)
	{
		ret = fillnchar(0, options->width, ' ');
		*res += ret;
		return (ret);
	}
	len = ft_unbr_length(&nbr, 8/*, options*/);

	if (nbr != 0  && options->show_prefix)
	{
		if (options->precision < len)
			options->precision = len + 1;
	}
	if (options->width > len && !options->left_align)
	{
		if (options->fill_by_zero && !options->precision)
			ret += fillnchar(len, options->width, '0');
		else if (options->precision > len)
		{
			ret += fillnchar(options->precision, options->width, ' ');
			ret += fillnchar(len, options->precision, '0');
		}
		else
			ret += fillnchar(len, options->width, ' ');
		print_oct(nbr);
		ret += len;
	}

	else if (options->width > len && options->left_align)
	{
		if (options->precision > len)
			ret += fillnchar(len, options->precision, '0');
		print_oct(nbr);
		ret += len;		
		ret += fillnchar(ret, options->width, ' ');
	}
	else
	{
		if (options->precision > len)
			ret += fillnchar(len, options->precision, '0');
		print_oct(nbr);
		ret += len;
	}
	*res += ret;
	return (ret);
}

ssize_t	ft_miniprintf_putnbr_hex(char **fmt, va_list *args, t_options *options, int *res)
{
	uintmax_t	nbr;
	int		len;
	char *ptr;
	int ret = 0;

	ptr = (char*)*fmt;	
	nbr = ft_cut_unsigned(args, options);

	if (nbr == 0)
		(options->show_prefix = 0);
	if (*ptr == 'p')
		(options->show_prefix = 1);
	if (!nbr && options->is_set_precision &&!options->precision)
	{
		if (*ptr == 'p')
		{
			write(1, "0x", 2);
			ret += 2;
		}
		ret += fillnchar(ret, options->width, ' ');
		*res += ret;
		return (ret);
	}
	len = ft_unbr_length(&nbr, 16/*, options*/);
	if (options->width > len && !options->left_align)
	{
		if (options->fill_by_zero && !options->precision)
		{
			if (options->show_prefix)
			{
				*ptr == 'X' ? write(1, "0X", 2) : write(1, "0x", 2);
				ret += 2;
				ret += fillnchar(len + ret, options->width, '0');
				options->show_prefix = 0;
			}
			else
			ret += fillnchar(len, options->width, '0');
		}
		else if (!(options->precision > len))
			ret += fillnchar(len + options->show_prefix * 2, options->width, ' ');
		if (options->show_prefix)
		{
			*ptr == 'X' ? write(1, "0X", 2) : write(1, "0x", 2);
			ret += 2;
		}
		if (options->precision > len)
			ret += fillnchar(len, options->precision, '0');
		print_hex(nbr, *ptr == 'X' ? 'A' : 'a');
		ret += len;
	}
	else if (options->width > len && options->left_align)
	{
		if (options->show_prefix)
		{
			*ptr == 'X' ? write(1, "0X", 2) : write(1, "0x", 2);
			ret += 2;
		}
		if (options->precision > len)
			ret += fillnchar(len, options->precision, '0');
		print_hex(nbr, *ptr == 'X' ? 'A' : 'a');
		ret += len;
		ret += fillnchar(ret, options->width, ' ');
	}
	else
	{
		if (options->show_prefix)
		{
			*ptr == 'X' ? write(1, "0X", 2) : write(1, "0x", 2);
			ret += 2;
		}
		if (options->precision > len)
			ret += fillnchar(len, options->precision, '0');
		print_hex(nbr, *ptr == 'X' ? 'A' : 'a');
		ret += len;
	}
	*res += ret;
	return (ret);
}

ssize_t	ft_miniprintf_putnbr_sdec(char **fmt, va_list *args, t_options *options, int *res)
{
	intmax_t	nbr;
	int		len;
	int ret = 0;

	if (!fmt)
		exit(ERROR);
	nbr = ft_cut_signed(args, options);
	len = ft_snbr_length(&nbr, 10/*, options*/);

	if (!nbr && options->is_set_precision &&!options->precision)
	{
		ret = fillnchar(0, options->width, ' ');
		*res += ret;
		return (ret);
	}
	if (options->width > len && !options->left_align)
	{
		if (options->fill_by_zero && !options->precision)
		{
			if (nbr < 0)
			{
				ft_putchar('-');
				ret += fillnchar(len, options->width, '0');
				nbr = -nbr;
			}
			else
			{
				if (options->space_before)
				{
					ft_putchar(' ');
					ret++;
				}
				else if (options->show_sign && nbr >= 0)
				{
					ft_putchar('+');
					ret++;
				}
				ret += fillnchar(len + ret, options->width, '0');
			}
		}
		else
		{
			/*if (options->space_before)
				{
					ft_putchar(' ');
					options->space_before = 0;
					ret++;
				}*/
			if (options->precision < len) 
				ret += fillnchar(len, options->width, ' ');
			else if (options->precision > len && nbr >= 0)
				ret += fillnchar(options->precision + options->show_sign, options->width, ' ');
			else if (options->precision > len && nbr < 0)
				ret += fillnchar(options->precision + 1, options->width, ' ');
			if (options->show_sign && !options->fill_by_zero && nbr >= 0)
			{
				ft_putchar('+');
				ret++;
			}	
			if (options->precision >= len)
			{
				if (nbr < 0)
				{
					ft_putchar('-');
					nbr = -nbr;
					ret++;
					len--;
				}
				ret += fillnchar(len, options->precision, '0');
			}
		}
			print_sdec(nbr);
			ret += len;
	}

	else if (options->width > len && options->left_align)
	{
		if (options->space_before && nbr > 0)
		{
			ft_putchar(' ');
			ret++;
		}
		else if (options->show_sign && nbr >= 0)
		{
			ft_putchar('+');
			ret++;
		}
		if (options->precision > len)
		{
			if (nbr < 0)
			{
				ft_putchar('-');
				nbr = -nbr;
			}
			ret += fillnchar(len, options->precision, '0');
		}
		print_sdec(nbr);
		ret += len;
		ret += fillnchar(ret, options->width, ' ');
	}
	else
	{
		if(options->space_before && nbr > 0)
		{
			ft_putchar(' ');
			ret++;
		}
		else if (options->show_sign && nbr >= 0)
		{
			ft_putchar('+');
			ret++;
		}
		if (options->precision > len)
		{
			if (nbr < 0)
			{
				ft_putchar('-');
				nbr = -nbr;
				len--;
				ret++;
			}
			ret += fillnchar(len, options->precision, '0');
		}
		print_sdec(nbr);
		ret += len;
	}
	*res += ret;
	return (ret);
}


ssize_t	ft_miniprintf_putnbr_udec(char **fmt, va_list *args, t_options *options, int *res)
{
	uintmax_t	nbr;
	int		len;
	int ret = 0;

	if (!fmt)
		exit(ERROR);
	nbr = ft_cut_unsigned(args, options);
	len = ft_unbr_length(&nbr, 10);

	if (!nbr && options->is_set_precision &&!options->precision)
	{
		ret = fillnchar(0, options->width, ' ');
		*res += ret;
		return (ret);
	}
	if (options->width > len && !options->left_align)
	{
		if (options->fill_by_zero && !options->precision)
		{
			ret += fillnchar(len + ret, options->width, '0');
		}
		else
		{
			if (options->precision < len) 
				ret += fillnchar(len, options->width, ' ');
			else if (options->precision > len && nbr)
				ret += fillnchar(options->precision + options->show_sign, options->width, ' ');
			
			if (options->precision >= len)
			{
				ret += fillnchar(len, options->precision, '0');
			}
		}
		print_udec(nbr);
		ret += len;
	}

	else if (options->width > len && options->left_align)
	{
		if (options->precision > len)
		{
			ret += fillnchar(len, options->precision, '0');
		}
		print_udec(nbr);
		ret += len;
		ret += fillnchar(ret, options->width, ' ');
	}

	else
	{
		if (options->precision > len)
		{
			ret += fillnchar(len, options->precision, '0');
		}
		print_udec(nbr);
		ret += len;
	}
	*res += ret;
	return (ret);
}

int		ft_miniprintf(const char *format, ...)
{
	va_list		args;
	int			res = 0;
	char		*ptr;

	va_start(args, format);
	while (*format)
	{
		if (*format == '%')
		{
			ptr = (char*)format + 1;
			if (!*ptr)
				return res;
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
