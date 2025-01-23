/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 11:09:42 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/23 11:33:04 by dzasenko         ###   ########.fr       */
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

int	ft_sleep(int sleep_time, t_philo *phil)
{
	long	time;
	long	now;
	int		death_res;

	time = get_time();
	now = get_time();
	if (!time || !now)
		return (-1);
	while (now - time < sleep_time)
	{
		usleep(100);
		death_res = check_if_dead(phil);
		if (death_res == -1)
			return (-1);
		else if (death_res)
			return (0);
		now = get_time();
		if (!now)
			return (-1);
	}
	return (1);
}

int	update_time(t_philo *philo)
{
	long	time;

	if (!philo || !philo->time_mutex)
		return (0);
	pthread_mutex_lock(philo->time_mutex);
	time = get_time();
	if (!time)
		return (pthread_mutex_unlock(philo->time_mutex), 0);
	philo->time = time;
	pthread_mutex_unlock(philo->time_mutex);
	return (1);
}
