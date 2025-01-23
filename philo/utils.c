/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 13:01:21 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/23 12:47:09 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	print_status(t_philo *phil, int status)
{
	long	time;
	int		death_res;

	if (!phil || !phil->print)
		return (-1);
	pthread_mutex_lock(phil->print);
	death_res = check_if_dead(phil);
	if (death_res == -1)
		return (pthread_mutex_unlock(phil->print), -1);
	else if (death_res)
		return (pthread_mutex_unlock(phil->print), 0);
	time = get_time();
	if (!time)
		return (pthread_mutex_unlock(phil->print), -1);
	if (status == 1)
		printf("%ld %d has taken a fork\n", time - phil->start_time, phil->i);
	else if (status == 2)
		printf("%ld %d is eating\n", time - phil->start_time, phil->i);
	else if (status == 3)
		printf("%ld %d is sleeping\n", time - phil->start_time, phil->i);
	else if (status == 4)
		printf("%ld %d is thinking\n", time - phil->start_time, phil->i);
	else
		return (pthread_mutex_unlock(phil->print), -1);
	return (pthread_mutex_unlock(phil->print), 1);
}

int	ft_strlen(const char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	check_simbols(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (ft_isdigit(str[i]))
			i++;
		else
		{
			return (0);
		}
	}
	return (1);
}

int	ft_atoi(const char *nptr)
{
	int		i;
	long	sing;
	long	result;

	i = 0;
	sing = 1;
	result = 0;
	while (nptr[i] == ' ' || (nptr[i] >= '\t' && nptr[i] <= '\r'))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sing *= -1;
		i++;
	}
	while (nptr[i] != '\0' && (nptr[i] >= '0' && nptr[i] <= '9'))
	{
		result = result * 10 + (nptr[i] - '0');
		i++;
	}
	if ((result * sing) < 1)
		return (printf("Error: Argument is negative or 0\n"), 0);
	else if ((result * sing) > 2147483647)
		return (printf("Error: Argument is too big number\n"), 0);
	return ((int)(result * sing));
}
