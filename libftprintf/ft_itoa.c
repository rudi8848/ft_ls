/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvynogra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 15:45:06 by gvynogra          #+#    #+#             */
/*   Updated: 2017/11/06 15:45:32 by gvynogra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

static	void	reverse(char *s, int len)
{
	int		i;
	int		j;
	char	c;

	i = 0;
	j = len - 1;
	while (i < j)
	{
		c = s[i];
		s[i] = s[j];
		s[j] = c;
		i++;
		j--;
	}
}

static int		ft_len(int nb)
{
	int len;

	len = 1;
	while (nb /= 10)
		len++;
	return (len);
}

static	int		ft_isnegative(int n)
{
	if (n < 0)
		return (1);
	return (0);
}

char			*ft_itoa(int n)
{
	char	*str;
	int		i;
	int		nbr;

	i = 0;
	nbr = n;
	if (nbr == -2147483648)
		return (ft_strdup("-2147483648"));
	if (!(str = (char*)malloc(sizeof(char) *
					ft_len(n) + 1 + ft_isnegative(n))))
		return (NULL);
	if (ft_isnegative(n))
		nbr = -nbr;
	while (i < ft_len(n))
	{
		str[i++] = nbr % 10 + '0';
		nbr /= 10;
	}
	if (ft_isnegative(n))
		str[i++] = '-';
	str[i] = '\0';
	reverse(str, ft_len(n) + ft_isnegative(n));
	return (str);
}
