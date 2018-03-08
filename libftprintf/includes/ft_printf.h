#ifndef FT_PRINTF_H
# define FT_PRINTF_H

#include "libft.h"
#include <stdarg.h>
#include <stdio.h>
#include <locale.h>
#include <wchar.h>
//#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>

#define ERROR -1

typedef union u_number
{
	intmax_t 	i;
	uintmax_t 	u;
} t_number;


/*
			Modifier      |    d, i      |     o, u, x, X       |     n
         ---------------------------------------------------------------------
         hh                signed char    unsigned char         signed char *
         h                 short          unsigned short        short *
         l (ell)           long           unsigned long         long *
         ll (ell ell)      long long      unsigned long long    long long *
         j                 intmax_t       uintmax_t             intmax_t *		<stdint.h>
         z                 (see note)     size_t                (see note)		<stddef.h> 


*/

typedef		struct s_options
{
	/***----flags----*/
	int				left_align;			//-
	int				show_sign;			//+
	int				space_before;		//' '
	int				show_prefix;		//#
	int				fill_by_zero;		//0

	/***----width-----*/
	int			width;

	/***----precision-----*/
	int 		is_set_precision;
	int			precision;			//.
	/***-----length modificators------*/
	int 			len_hh;
	int 			len_h;
	int 			len_l;
	int 			len_ll;
	int 			len_j;
	int 			len_z;
}					t_options;


typedef		ssize_t(*t_pf)(char **, va_list*, t_options*, int *res);


typedef				enum
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
}					e_conv;

/*
**-----------------------------------BIT MASKS FOR UNICODE-------------------------------------------------
*/

typedef		enum
{
	ONE_B,
	TWO_B,
	THREE_B,
	FOUR_B,
	BIT_MASKS
}e_bit_masks;




int		ft_printf(const char *format, ...);


#endif
