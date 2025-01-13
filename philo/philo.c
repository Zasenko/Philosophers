/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 13:01:05 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/08 13:52:02 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*create_philosopher(void *arg)
{
	t_philo	*philo;
	//long	time_of_creations;
	
	philo = (t_philo *)arg;
	if (!philo)
		return (NULL);
	if (!philo->fork1)
		return (NULL);

	if (philo->i % 2 == 0)
	{
		usleep(philo->time_to_eat / 2);
	}
	
	if (!philo->fork2)
	{
		printf("NO FORK 2 - DEATH !!!!!\n");
		return (arg);
	}
	else
	{
		
		if (pthread_mutex_lock(philo->must_eat_times_mutex) != 0)
			return (NULL);
		int must_eat_times = philo->must_eat_times;
		if (pthread_mutex_unlock(philo->must_eat_times_mutex) != 0)
			return (NULL);
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