/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabriela <gabriela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 17:55:02 by gabtan            #+#    #+#             */
/*   Updated: 2023/09/05 15:39:25 by gabriela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_size(int long n, int sign)
{
	int	size;

	size = 0;
	while (n >= 10 || n <= -10)
	{
		n = n / 10;
		size++;
	}
	if (sign == -1)
		size = size + 2;
	else if (sign == 1)
		size = size + 1;
	return (size);
}

static char	*copy_digits(long int b, int sign, int size)
{
	char	c;
	int		i;
	char	*str;

	i = 0;
	str = malloc(sizeof(char) * (size + 1));
	if (str == NULL)
		return (NULL);
	if (b == 0)
		str[0] = 0 + '0';
	if (sign == -1)
		str[0] = '-';
	i = size - 1;
	while (b > 0 || (b * -1) > 0)
	{
		c = b % 10 + '0';
		str[i] = c;
		i--;
		b = b / 10;
	}
	str[size] = '\0';
	return (str);
}

char	*ft_itoa(int n)
{
	int			sign;
	int			size;
	char		*str;
	int long	b;

	sign = 1;
	b = (int long)n;
	if (n < 0)
	{
		b = -b;
		sign = -sign;
	}
	size = get_size((int long)n, sign);
	return (str = copy_digits(b, sign, size));
}
