/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:39:57 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/23 12:47:54 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_trim(char c, char const *set)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	src_size;
	size_t	i;

	i = 0;
	src_size = ft_strlen(src);
	if (size > 0)
	{
		while (src[i] != '\0' && i < size - 1)
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (src_size);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub_s;
	size_t	s_len;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
	{
		sub_s = (char *)malloc(1 * sizeof(char));
		if (sub_s == NULL)
			return (NULL);
		sub_s[0] = '\0';
		return (sub_s);
	}
	if (len > s_len - start)
		len = s_len - start;
	sub_s = (char *)malloc((len + 1) * sizeof(char));
	if (sub_s == NULL)
		return (NULL);
	ft_strlcpy(sub_s, s + start, len + 1);
	return (sub_s);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*new_s;
	size_t	s_l;
	size_t	e_l;
	size_t	len;
	size_t	i;

	s_l = 0;
	e_l = 0;
	len = ft_strlen(s1);
	i = 0;
	while (s1[i] && ft_trim(s1[i], set) == 1)
	{
		s_l++;
		i++;
	}
	i = len - 1;
	while (s1[i] && ft_trim(s1[i], set) == 1)
	{
		e_l++;
		i--;
	}
	new_s = ft_substr(s1, s_l, len - e_l - s_l);
	if (new_s == NULL)
		return (NULL);
	return (new_s);
}

int	make_number(char *str)
{
	int		num;
	char	*trim_str;

	if (!str)
		return (0);
	trim_str = ft_strtrim(str, " \t");
	if (!trim_str)
		return (0);
	if (!ft_strlen(trim_str))
		return (free_str(trim_str), printf("Error: empty argument\n"), 0);
	if (!check_simbols(trim_str))
		return (free_str(trim_str), printf("Error: Wrong argument\n"), 0);
	if (ft_strlen(trim_str) > 11)
		return (free_str(trim_str), printf("Error: Argument is not INT\n"), 0);
	num = ft_atoi(trim_str);
	return (free_str(trim_str), num);
}
