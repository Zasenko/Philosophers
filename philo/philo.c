/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 13:01:05 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/21 12:34:25 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//+
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

//+
int	one_philo_life(t_philo *phil)
{
	if (!phil)
		return (0);
	if (print_status(phil, 1) == -1)
		return (0);
	usleep(phil->time_to_die);
	return 1;
}

int philo_life(t_philo *philo)
{
	if (!philo)
		return (0);
	
	if (philo->number_of_philosophers % 2 != 0)
	{
		if (philo->i == 1)
			usleep((philo->time_to_eat / 2) * 3 * 1000);
		if (philo->i % 2 == 0)
			usleep((philo->time_to_eat / 2) * 1000);
	}
	else
	{
		if (philo->i % 2 == 0)
			usleep((philo->time_to_eat / 2) * 1000);
	}

	int result = philo_circle(philo);
	if (result == -1)
		return (0);
	else if (result == 0)
		return (1);

	while (1)
	{
		pthread_mutex_lock(philo->time_mutex);
		long time = philo->time;
		pthread_mutex_unlock(philo->time_mutex);
		long now = get_time();
		if (!now)
			return (0);
		while (now - time < (philo->time_to_die - (philo->time_to_eat / 2)))
		{
			usleep(1000);
			now = get_time();
			if (!now)
				return (0);
		}
		int result = philo_circle(philo);
		if (result == -1)
			return (0);
		else if (result == 0)
			return (1);
	}
	return (0);
}

//+
void	*create_philosopher(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (!philo || !philo->fork1)
		return (NULL);
	if (!wait_philos_creations(philo))
		return (NULL);
	if (philo->number_of_philosophers == 1)
	{
		if (!one_philo_life(philo))
			return (NULL);
		return (arg);
	}
	else
	{
		if (!philo_life(philo))
			return (NULL);
		return (arg);
	}
	return (NULL);
}
