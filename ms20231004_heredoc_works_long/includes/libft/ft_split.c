/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabriela <gabriela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 17:52:32 by gabtan            #+#    #+#             */
/*   Updated: 2023/09/05 15:38:55 by gabriela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_elem(const char *str, char c)
{
	int	count;

	count = 0;
	while (*str != 0)
	{
		while (*str == c)
			str++;
		while (*str != c && *str != 0)
		{
			str++;
			if (*str == '\0' || *str == c)
				count++;
		}
	}
	count++;
	return (count);
}

static char	**split_string(char const *s, char **array, char c, int str_index)
{
	int	len;
	int	i;

	i = 0;
	while (s[str_index])
	{
		while (s[str_index] == c)
			str_index++;
		if (s[str_index] != 0)
		{
			len = 0;
			while (s[str_index + len] != c && s[str_index + len] != 0)
				len++;
			array[i] = ft_calloc(sizeof(char), len + 1);
			ft_memcpy(array[i], &s[str_index], len);
			i++;
			str_index = str_index + len;
		}
	}
	array[i] = 0;
	return (array);
}

char	**ft_split(char const *s, char c)
{
	char	**array;
	int		str_index;

	if (s == 0)
		return (0);
	str_index = 0;
	array = (char **)malloc(sizeof(char *) * count_elem(s, c));
	if (!array)
		return (0);
	return (split_string(s, array, c, str_index));
}
