/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:45:00 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/23 12:45:07 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_forks(pthread_mutex_t **forks)
{
	int	i;

	if (!forks)
		return ;
	i = 0;
	while (forks[i])
	{
		destroy_mutex(forks[i]);
		free(forks[i]);
		forks[i] = NULL;
		i++;
	}
	free(forks);
}

void	free_phil(t_philo *phil)
{
	if (!phil)
		return ;
	if (phil->must_eat_times_mutex)
	{
		destroy_mutex(phil->must_eat_times_mutex);
		free(phil->must_eat_times_mutex);
		phil->must_eat_times_mutex = NULL;
	}
	if (phil->time_mutex)
	{
		destroy_mutex(phil->time_mutex);
		free(phil->time_mutex);
		phil->time_mutex = NULL;
	}
	free(phil);
	return ;
}

void	free_philos(t_philo **philos)
{
	int	i;

	if (!philos)
		return ;
	i = 0;
	while (philos[i])
	{
		free_phil(philos[i]);
		philos[i] = NULL;
		i++;
	}
	free(philos);
}
