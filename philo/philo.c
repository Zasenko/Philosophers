/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 13:01:05 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/07 13:03:02 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*create_philosopher(void *arg)
{
	t_philo	*philo;
	long	time_of_creations;
	
	philo = (t_philo *)arg;
	if (!philo)
		return (NULL);
	if (!philo->fork1)
		return (NULL);
	time_of_creations = get_time();
	if (time_of_creations == -1)
		return (NULL);
	if (pthread_mutex_lock(philo->phil) != 0)
		return (NULL);
	philo->time = time_of_creations;
	if (pthread_mutex_unlock(philo->phil) != 0)
		return (NULL);

	if (!philo->fork2)
	{
		printf("NO FORK 2 - DEATH !!!!!\n");
		return (arg);
	}
	else
	{
		
		if (pthread_mutex_lock(philo->phil) != 0)
			return (NULL);
		int must_eat_times = philo->must_eat_times;
		if (pthread_mutex_unlock(philo->phil) != 0)
			return (NULL);
		if (must_eat_times > 0)
		{
			while (must_eat_times > 0)
			{
				int result = philo_circle(philo);
				if (result == -1)
					return (NULL);
				else if (result == 0)
				{
					pthread_mutex_lock(philo->print);// DELETE
					printf("--- Philo %d philo_circle RESULT - DEAD ---\n", philo->i); // DELETE
					pthread_mutex_unlock(philo->print);
					usleep(100000);
					pthread_mutex_lock(philo->print);// DELETE
					printf("--- AFTERSLEEP Philo %d philo_circle RESULT - DEAD ---\n", philo->i); // DELETE
					pthread_mutex_unlock(philo->print);
					return (arg);
				}
				if (pthread_mutex_lock(philo->phil) != 0)
					return (NULL);
				philo->must_eat_times--;
				must_eat_times--;
				if (pthread_mutex_unlock(philo->phil) != 0)
					return (NULL);
				// pthread_mutex_lock(philo->print);												// DELETE
				// printf("--- Philo %d must_eat_times %d---\n", philo->i, philo->must_eat_times); // DELETE
				// pthread_mutex_unlock(philo->print);												// DELETE
			}
			// pthread_mutex_lock(philo->print);												// DELETE
			// printf("--- create_philosopher PHILO EATED---\n");			// DELETE
			// pthread_mutex_unlock(philo->print);
			return (arg);// ? todo free *philo
		}
		else
		{
			while (1)
			{
				int result = philo_circle(philo);
				if (result == -1)
					return (NULL);// ? todo free *philo
				else if (result == 0)
					return (arg);// ? todo free *philo
			}
		}
	}
	return (NULL);// ? todo free *philo
}