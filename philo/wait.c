/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 10:49:57 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/22 10:50:26 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	wait_philos_creations(t_philo *philo)
{
	int	all_philos_created;

	if (!philo)
		return (0);
	pthread_mutex_lock(philo->all_philos_created_mutex);
	all_philos_created = *philo->all_philos_created;
	pthread_mutex_unlock(philo->all_philos_created_mutex);
	while (!all_philos_created)
	{
		pthread_mutex_lock(philo->all_philos_created_mutex);
		all_philos_created = *philo->all_philos_created;
		pthread_mutex_unlock(philo->all_philos_created_mutex);
		usleep(100);
	}
	pthread_mutex_lock(philo->time_mutex);
	philo->time = get_time();
	if (!philo->time)
		return (pthread_mutex_unlock(philo->time_mutex), 0);
	pthread_mutex_unlock(philo->time_mutex);
	return (1);
}

int	wait_hungry(t_philo *philo)
{
	long	time;
	long	now;

	if (!philo || !philo->time_mutex)
		return (-1);
	pthread_mutex_lock(philo->time_mutex);
	time = philo->time;
	pthread_mutex_unlock(philo->time_mutex);
	now = get_time();
	if (!now)
		return (-1);
	while (now - time < (philo->time_to_die - (philo->time_to_eat / 2)))
	{
		usleep(1000);
		now = get_time();
		if (!now)
			return (-1);
	}
	return (1);
}
