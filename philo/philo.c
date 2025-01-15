/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 13:01:05 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/15 12:20:30 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*create_philosopher(void *arg)
{
	t_philo	*philo;
	//long	time_of_creations;
	
	philo = (t_philo *)arg;
	if (!philo || !philo->fork1)
		return (NULL);

	int all_philos_created;
	pthread_mutex_lock(philo->all_philos_created_mutex);
	all_philos_created = philo->all_philos_created;
	pthread_mutex_unlock(philo->all_philos_created_mutex);
	
	while(!all_philos_created)
	{
		usleep(100);
		pthread_mutex_lock(philo->all_philos_created_mutex);
		all_philos_created = philo->all_philos_created;
		pthread_mutex_unlock(philo->all_philos_created_mutex);
	}

	if (philo->i % 2 == 0 || (philo->number_of_philosophers % 2 != 0 && philo->i == philo->number_of_philosophers))
		usleep(philo->time_to_die / 2 * 1000);

	if (!philo->fork2)
	{
		printf("NO FORK 2 - DEATH !!!!!\n");
		return (arg);
	}
	else
	{
		
		pthread_mutex_lock(philo->must_eat_times_mutex);
		int must_eat_times = philo->must_eat_times;
		pthread_mutex_unlock(philo->must_eat_times_mutex);
		
		if (must_eat_times > 0)
		{
			while (must_eat_times > 0)
			{
				int result = philo_circle(philo);
				if (result == -1)
					return (NULL);
				else if (result == 0)
					return (arg);
				if (pthread_mutex_lock(philo->must_eat_times_mutex) != 0)
					return (NULL);
				philo->must_eat_times--;
				if (pthread_mutex_unlock(philo->must_eat_times_mutex) != 0)
					return (NULL);
				must_eat_times--;
			}
			return (arg);
		}
		else
		{
			while (1)
			{
				int result = philo_circle(philo);
				if (result == -1)
					return (NULL);
				else if (result == 0)
					return (arg);
			}
		}
	}
	return (NULL);
}