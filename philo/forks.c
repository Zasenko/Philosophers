/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 11:05:49 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/22 11:14:01 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	take_fork(t_philo *philo, pthread_mutex_t *fork);

int	take_forks(t_philo *philo)
{
	int	res;

	if (!philo || !philo->fork1 || !philo->fork2)
		return (-1);
	res = take_fork(philo, philo->fork1);
	if (res == -1)
		return (-1);
	else if (res == 0)
		return (0);
	res = take_fork(philo, philo->fork2);
	if (res == -1)
		return (pthread_mutex_unlock(philo->fork1), -1);
	else if (res == 0)
		return (pthread_mutex_unlock(philo->fork1), 0);
	return (1);
}

int	unlock_forks(t_philo *philo)
{
	if (!philo || !philo->fork1 || !philo->fork2)
		return (-1);
	pthread_mutex_unlock(philo->fork1);
	pthread_mutex_unlock(philo->fork2);
	return (1);
}

static int	take_fork(t_philo *philo, pthread_mutex_t *fork)
{
	int		result;

	if (!philo || !fork)
		return (-1);
	pthread_mutex_lock(fork);
	result = print_status(philo, 1);
	if (result == -1)
		return (pthread_mutex_unlock(fork), -1);
	else if (result == 0)
		return (pthread_mutex_unlock(fork), 0);
	return (1);
}
