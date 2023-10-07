/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabriela <gabriela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 14:04:50 by gabtan            #+#    #+#             */
/*   Updated: 2023/06/19 13:19:22 by gabriela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (s1[i] == s2[i])
	{
		if (i + 1 == n)
			break ;
		if (s1[i + 1] == '\0' || s2[i + 1] == '\0')
			return ((unsigned char)s1[i + 1] - (unsigned char)s2[i + 1]);
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
