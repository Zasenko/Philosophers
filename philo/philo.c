/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 13:01:05 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/16 13:37:12 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*create_philosopher(void *arg)
{
	t_philo	*philo;
	
	philo = (t_philo *)arg;
	if (!philo || !philo->fork1)
		return (NULL);

	int all_philos_created;
	pthread_mutex_lock(philo->all_philos_created_mutex);
	all_philos_created = *philo->all_philos_created;
	pthread_mutex_unlock(philo->all_philos_created_mutex);
	
	while(!all_philos_created)
	{
		//usleep(50);
		pthread_mutex_lock(philo->all_philos_created_mutex);
		all_philos_created = *philo->all_philos_created;
		pthread_mutex_unlock(philo->all_philos_created_mutex);
	}
	
	pthread_mutex_lock(philo->time_mutex);
	philo->time = get_time();
	pthread_mutex_unlock(philo->time_mutex);

	if (philo->number_of_philosophers == 1)
	{
		pthread_mutex_lock(philo->print);
		long time = get_time();
		printf("%ld %d has taken a fork\n", time - philo->start_time, philo->i);
		pthread_mutex_unlock(philo->print);
		usleep(philo->time_to_die);
		return (arg);
	}
	else
	{
		//if (philo->i % 2 == 0 || (philo->number_of_philosophers % 2 != 0 && philo->i == philo->number_of_philosophers))
		if (philo->i % 2 == 0)
			usleep(philo->time_to_eat / 2 * 1000);
		
		while (1)
		{
			int result = philo_circle(philo);
			if (result == -1)
				return (NULL);
			else if (result == 0)
				return (arg);
			usleep(100);
		}
	}
	return (NULL);
}