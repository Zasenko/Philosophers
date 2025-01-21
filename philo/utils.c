/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 13:01:21 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/21 13:52:55 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (0);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	ft_sleep(int sleep_time)
{
	long	time;
	long	now;

	time = get_time();
	now = get_time();
	if (!time || !now)
		return (0);
	while (now - time < sleep_time)
	{
		usleep(1000);
		now = get_time();
		if (!now)
			return (0);
	}
	return (1);
}

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
