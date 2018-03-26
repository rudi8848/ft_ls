#include "ft_miniprintf.h"

intmax_t	ft_cut_signed(va_list *args, t_options *options)
{
	intmax_t nbr;

	nbr = va_arg(*args, intmax_t);
	nbr = (long)nbr;
	return (nbr);
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

int		fillnchar(int len, int width, char c)
{
	int i = 0;
	char str[256];

	while (len < width)
	{
		str[i] = c;
		len++;
		i++;
	}
	return (write(1, str, i));
}

size_t	ft_miniprintf_putnbr_sdec(va_list *args, t_options *options, int *res)
{
	intmax_t	nbr;
	int		len;
	int ret = 0;

	nbr = ft_cut_signed(args, options);
	len = ft_snbr_length(&nbr, 10);

	if (options->width > len && !options->left_align)
	{
		if (options->fill_by_zero)
		{
			if (nbr < 0)
			{
				ft_putchar('-');
				ret += fillnchar(len, options->width, '0');
				nbr = -nbr;
			}
			else if (options->space_before)
			{
				ft_putchar(' ');
				ret++;
			}
			else if (options->show_sign && nbr >= 0)
			{
				ft_putchar('+');
				ret++;
			}
					//ret += fillnchar(len + ret, options->width, '0');
		}
		//}
		else
		{
			/*if (options->precision < len) */
				ret += fillnchar(len + ret, options->width, ' ');
			/*else if (options->precision > len && nbr >= 0)
				ret += fillnchar(options->precision + options->show_sign, options->width, ' ');
			else if (options->precision > len && nbr < 0)
				ret += fillnchar(options->precision + 1, options->width, ' ');*/
			if (options->show_sign && !options->fill_by_zero && nbr >= 0)
			{
				ft_putchar('+');
				ret++;
			}	
			/*if (options->precision >= len)
			{
				if (nbr < 0)
				{
					ft_putchar('-');
					nbr = -nbr;
					ret++;
					len--;
				}
				ret += fillnchar(len, options->precision, '0');
			}*/
		}
			print_sdec(nbr);
			ret += len;
			//ret += fillnchar(ret, options->width, ' ');
			
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
		/*if (options->precision > len)
		{
			if (nbr < 0)
			{
				ft_putchar('-');
				nbr = -nbr;
			}
			ret += fillnchar(len, options->precision, '0');
		}*/
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
		/*if (options->precision > len)
		{
			if (nbr < 0)
			{
				ft_putchar('-');
				nbr = -nbr;
				len--;
				ret++;
			}
			ret += fillnchar(len, options->precision, '0');
		}*/
		print_sdec(nbr);
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
