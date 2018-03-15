#include "includes/ft_printf.h"

size_t	unicode_masks[BIT_MASKS] = {
	0,									//"0xxx xxxx",
	0xC080,			//49280,			//"110x xxxx    10xx xxxx",
	0xE08080,		//14712960,			//"1110 xxxx    10xx xxxx    10xx xxxx",
	0xF0808080		//4034953344		//"1111 0xxx    10xx xxxx    10xx xxxx    10xx xxxx"
};

int	fillnchar(int len, int width, char c);
static int check_type(char c, t_options *options);
  
int		ft_print_null_string(void)
{
	write(1, "(null)", 6);
	return (6);
}

ssize_t		ft_printf_putstr(char **fmt, va_list *args, t_options *options, int *res)
{
	int len;
	int ret = 0;

	len = 0;
	if (!fmt || !options)		//<-------- to do
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


int		write_two_bytes(size_t symb)
{
	int 			res;
	unsigned char 	o2;
	unsigned char 	o1;
	unsigned char 	octet;

	res = 0;
	o2 = (symb << 26) >> 26;
	o1 = ((symb >> 6) << 27) >> 27;
	octet = (unicode_masks[TWO_B] >> 8) | o1;
	write(1, &octet, 1);
	res++;
	octet = ((unicode_masks[TWO_B] << 24) >> 24) | o2;
	write(1, &octet, 1);
	res++;
	return (res);
}

int		write_three_bytes(size_t symb)
{
	int 			res;
	unsigned char 	o3;
	unsigned char 	o2;
	unsigned char 	o1;
	unsigned char 	octet;

	res = 0;
	o3 = (symb << 26) >> 26;
	o2 = ((symb >> 6) << 26) >> 26;
	o1 = ((symb >> 12) << 28) >> 28;
	octet = (unicode_masks[THREE_B] >> 16) | o1;
	write(1, &octet, 1);
	res++;
	octet = ((unicode_masks[THREE_B] << 16) >> 24) | o2;
	write(1, &octet, 1);
	octet = ((unicode_masks[THREE_B] << 24) >> 24) | o3;
	write(1, &octet, 1);
	res++;
	return (res);
}

int		write_four_bytes(size_t symb)
{
	int 			res;
	unsigned char 	o4;
	unsigned char 	o3;
	unsigned char	o2;
	unsigned char 	o1;
	unsigned char 	octet;

	res = 0;
	o4 = (symb << 26) >> 26;
	o3 = ((symb >> 6) << 26) >> 26;
	o2 = ((symb >> 12) << 26) >> 26;
	o1 = ((symb >> 18) << 29) >> 29;

	octet = (unicode_masks[FOUR_B] >> 24) | o1;
	write(1, &octet, 1);
	res++;
	octet = ((unicode_masks[FOUR_B] << 8) >> 24) | o2;
	write(1, &octet, 1);
	res++;
	octet = ((unicode_masks[FOUR_B] << 16) >> 24) | o3;
	write(1, &octet, 1);
	res++;
	octet = ((unicode_masks[FOUR_B] << 24) >> 24) | o4;
	write(1, &octet, 1);
	res++;
	return (res);
}

/*
int		ft_putwchar(wchar_t chr)

	if (chr <= 0x7F)
        ft_putchar(chr);

    
    else if (chr <= 0x7FF)
    {
        ft_putchar((chr >> 6) + 0xC0);
        ft_putchar((chr & 0x3F) + 0x80);
    }
    else if (chr <= 0xFFFF)
    {
        ft_putchar((chr >> 12) + 0xE0);
        ft_putchar(((chr >> 6) & 0x3F) + 0x80);
        ft_putchar((chr & 0x3F) + 0x80);
    }
    else if (chr <= 0x10FFFF)
    {
        ft_putchar((chr >> 18) + 0xF0);
        ft_putchar(((chr >> 12) & 0x3F) + 0x80);
        ft_putchar(((chr >> 6) & 0x3F) + 0x80);
        ft_putchar((chr & 0x3F) + 0x80);
    }
        	
    
    
	else if (chr <= 0x7FF)
		return write_two_bytes(chr);
	else if (chr <= 0xFFFF)
		return write_three_bytes(chr);
	else if (chr <= 0x10FFFF)
		return write_four_bytes(chr);
	return (1);
}
*/

int ft_nb_bits(wchar_t wc)
{
 int i = 0;
 while (wc)
  {
    wc/=2;
    i++;
    }
    return i;
  }
int ft_nb_bytes(unsigned int n)
{
  if (n <= 7)
    return 1;
    else if (n <= 11)
    return 2;
    else if (n <= 16)
    return 3;
    else
    return 4;
  }

int   ft_putwchar(wchar_t wc)
{
    char    tmp[4];
    int  	bits;
    int		 bytes;

    bits = ft_nb_bits(wc);
    bytes = ft_nb_bytes(wc);

    if (bytes <= bits && bytes <= MB_CUR_MAX)
    {
        if (bytes == 1)
            tmp[0] = wc;
        else
        {
            if (bytes == 2)
                tmp[0] = ((wc & (0x1f << 6)) >> 6) | 0xC0;
            else
            {
                if (bytes == 3)
                    tmp[0] = ((wc >> 12) & 0xf) | 0xE0;
                else
                {
                    tmp[0] = ((wc >> 18) & 7) | 0xF0;
                    tmp[1] = ((wc >> 12) & 0x3f) | 0x80;
                }
                tmp[bytes - 2] = ((wc >> 6) & 0x3f) | 0x80;
            }
            tmp[bytes - 1] = (wc & 0x3f) | 0x80;
        }
        write(1, tmp, bytes);
    }
    return (bytes);
}

ssize_t		ft_printf_putchar(char **fmt, va_list *args, t_options *options, int *res)
{
	int symb;
	char *ptr;
	int ret = 0;

	if (options->width && !options->left_align)
	{
		if (options->fill_by_zero)
			ret += fillnchar(1, options->width, '0');
		else
			ret += fillnchar(1, options->width, ' ');
	}
	ptr = *fmt;
	if (args)
	{
		symb = va_arg(*args, int);
		if (*ptr == 'c' && !options->len_l)
		{
			ft_putchar(symb);
			ret += 1;
		}
		else
				ret += ft_putwchar(symb);
	}
	else
	{
		ft_putchar(*ptr);
		ret += 1;
	}
	if (options->width && options->left_align)
		ret += fillnchar(1, options->width, ' ');
	*res += ret;
	return (ret);
}
//int		print_wstr(int *wstr);


/*
**----------------------------------------------------------------------------------------------------------
*/



/*
**------------------------------------- NUMBER CONVERSIONS ------------------------------------------------
*/


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

int		ft_unbr_length(uintmax_t *n,int  base/*, t_options *options*/)
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
int		ft_snbr_length(intmax_t *n,int  base/*, t_options *options*/)
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

ssize_t	ft_printf_putnbr_oct(char **fmt, va_list *args, t_options *options, int *res)
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

ssize_t	ft_printf_putnbr_hex(char **fmt, va_list *args, t_options *options, int *res)
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

ssize_t	ft_printf_putnbr_sdec(char **fmt, va_list *args, t_options *options, int *res)
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


ssize_t	ft_printf_putnbr_udec(char **fmt, va_list *args, t_options *options, int *res)
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



/*
**----------------------------------------------------------------------------------------------------------
*/


/*
int ft_max(int a, int b, int c)
{
	if (a >= b && a >=c)
		return a;
	else if (b >= a && b >= c)
		return b;
	else
		return c;
}
char	*print_udec1(char *ptr, uintmax_t n, int len)
{	
	while (len)
	{
		*(ptr + len) = n % 10 + '0';
		n /= 10;
		len--;
	}
	return (ptr);

}
ssize_t	ft_printf_putnbr_udec(char **fmt, va_list *args, t_options *options, int *res)
{
	uintmax_t	nbr;
	int		len;
	int ret = 0;
	char *str;
	int slen;
	char * ptr;

	if (!fmt)
		exit(ERROR);
	nbr = ft_cut_unsigned(args, options);
	len = ft_unbr_length(&nbr, 10);

	//slen = ft_max(len, options->precision, options->width)
	slen = len + options->space_before + options->show_sign;
	str = ft_strnew(slen);
	if (str)
	{
		
		*str = options->show_sign ? '+' : ' ';
		ptr = str + 1;
		ptr = print_udec1(ptr, nbr, len);
		
		ft_printf_putstr(fmt, NULL, options, res);
		
		free(str);
	}
	*res += slen;
	return slen;
}
*/

/*
**----------------------------------------------------------------------------------------------------------
*/

size_t		ft_wstrlen(wchar_t *wstr)
{
	size_t len = 0;

	while (wstr[len] != L'\0')
		len++;
	return (len);
}

ssize_t	print_wstr(char **fmt, va_list *args, t_options *options, int *res)
{
	/*
	if (fmt && args && options && res)
	return 1;
else
	return 0;
	*/
	size_t i;
	wchar_t *wstr = NULL;
	int ret = 0;

	i = 0;
	if (!fmt || !options)
		exit(ERROR);
	wstr = (wchar_t *)va_arg(*args, wchar_t*);
	if (!wstr)
	{
		ft_print_null_string();
		*res += 6;
		return (6);
	}
	while (wstr[i] != L'\0')
	{
		ret += ft_putwchar(wstr[i]);
		i++;
	}
		*res += ret;
	return (ret);
}

/*
**----------------------------------------------------------------------------------------------------------
*/

int		ft_parse_flags(char *fp, t_options *options)
{
	int i;

	i = 0;
	
	while (fp[i] == '-' || fp[i] == '+' || fp[i] == ' ' || fp[i] == '#' || fp[i] == '0' )
	{
		if (fp[i] && fp[i + 1])
		{
			if(fp[i] == '+')
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
		}
		else
			exit(0);
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
	if (fp[i] && fp[i + 1])
	{
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
	}
	return (i);
}

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
	if (fp[i] && fp[i + 1])
	{
		while (fp[i] == 'h' || fp[i] == 'l' || fp[i] == 'j' || fp[i] == 'z')
		{
			if (fp[i] == 'h')
			{
				i++;
				if (fp[i] == 'h')
				{
					options->len_hh = 1;
					i++;
				}
				options->len_h = 1;
			}
			if (fp[i] == 'l')
			{
				i++;
				if (fp[i] == 'l')
				{
					options->len_ll = 1;
					i++;
				}
				options->len_l = 1;
			}
			if (fp[i] == 'j')
			{
				options->len_j = 1;
				i++;
			}
			if (fp[i] == 'z')
			{
				options->len_z = 1;
				i++;
			}
		}
	}
	return (i);
}

void	ft_set_array(t_pf *convert_functions)
{
	convert_functions[CONV_c] = &ft_printf_putchar;
	convert_functions[CONV_s] = &ft_printf_putstr;
	//if (conv == CONV_d && options->len_z)
	//	convert_functions[CONV_d] = &ft_printf_putnbr_udec;
	//else
		convert_functions[CONV_d] = &ft_printf_putnbr_sdec;
	convert_functions[CONV_D] = &ft_printf_putnbr_sdec;
	//if (conv == CONV_d && options->len_z)
		//convert_functions[CONV_i] = &ft_printf_putnbr_udec;
	//else
		convert_functions[CONV_i] = &ft_printf_putnbr_sdec;
	convert_functions[CONV_u] = &ft_printf_putnbr_udec;
	convert_functions[CONV_U] = &ft_printf_putnbr_udec;
	convert_functions[CONV_o] = &ft_printf_putnbr_oct;
	convert_functions[CONV_O] = &ft_printf_putnbr_oct;
	convert_functions[CONV_x] = &ft_printf_putnbr_hex;
	convert_functions[CONV_X] = &ft_printf_putnbr_hex;
	convert_functions[CONV_p] = &ft_printf_putnbr_hex;
	//convert_functions[CONV_b] = &ft_printf_putnbr_bin;
	//convert_functions[CONV_f] = &ft_prinft_putnbr_float;
	//convert_functions[CONV_F] = &ft_prinft_putnbr_float;
	convert_functions[CONV_C] = &ft_printf_putchar;
	convert_functions[CONV_S] = &print_wstr;
}

t_pf	ft_choose_type(e_conv conv/*, t_options *options*/)
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

static int check_type(char c, t_options *options)
{
	if (c == 's' || c == 'S' || c == 'c' || c == 'C')
		return (1);
	else if ( c == 'd' || c == 'i')
		return (1);
	else if (c == 'o'|| c == 'u' || c == 'x' || c == 'X')
		return (1);
	else if (c == 'D' || c == 'U'  || c == 'O' || c == 'p')
	{
		options->len_l = 1;
		return (1);
	}
	else
		return (0);
}


ssize_t	ft_parse_options(const char **format, va_list *args, int *res)
{
	t_options *options;
	char *fmtp;
	t_pf ft_transformer;

	fmtp = (char*)++(*format);
	options = (t_options*)ft_memalloc(sizeof(t_options));
	if (!options)
		exit(ERROR);
	if (*fmtp)
	{
		fmtp += ft_parse_flags(fmtp, options);
		fmtp += ft_parse_width(fmtp, args, options);
		fmtp += ft_parse_precision(fmtp, args, options);
		fmtp += ft_parse_length(fmtp, options);
		if (*fmtp != '\0')
		{
			if (check_type(*fmtp, options))
			{
				ft_transformer = ft_choose_type(*fmtp/*, options*/);
				ft_transformer(&fmtp, args, options, res);
			}
			else
				ft_printf_putchar(&fmtp, NULL, options, res);
		}
		if (options)
			free(options);
		return (fmtp - *format);
	}
	return 0;
}


int		ft_printf(const char *format, ...)
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
