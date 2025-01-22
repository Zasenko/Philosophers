/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_circle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 10:31:24 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/22 11:11:10 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	eating(t_philo *philo);
static int	sleeping(t_philo *philo);
static int	thinking(t_philo *philo);

int	philo_circle(t_philo *philo)
{
	int	result;

	if (!philo)
		return (-1);
	result = take_forks(philo);
	if (result == -1)
		return (-1);
	else if (result == 0)
		return (0);
	result = eating(philo);
	if (result == -1)
		return (-1);
	else if (result == 0)
		return (0);
	result = sleeping(philo);
	if (result == -1)
		return (-1);
	else if (result == 0)
		return (0);
	return (thinking(philo));
}

int	check_if_dead(t_philo *philo)
{
	if (!philo)
		return (-1);
	pthread_mutex_lock(philo->is_dead_mutex);
	if (*philo->is_dead == 1)
		return (pthread_mutex_unlock(philo->is_dead_mutex), 1);
	pthread_mutex_unlock(philo->is_dead_mutex);
	return (0);
}

static int	eating(t_philo *philo)
{
	int		res;

	if (!philo || !philo->must_eat_times_mutex || !update_time(philo))
		return (unlock_forks(philo), -1);
	res = print_status(philo, 2);
	if (res == -1)
		return (unlock_forks(philo), -1);
	else if (res == 0)
		return (unlock_forks(philo), 0);
	if (!ft_sleep(philo->time_to_eat))
		return (unlock_forks(philo), -1);
	pthread_mutex_lock(philo->must_eat_times_mutex);
	philo->must_eat_times--;
	pthread_mutex_unlock(philo->must_eat_times_mutex);
	return (unlock_forks(philo));
}

static int	sleeping(t_philo *philo)
{
	int		res;

	if (!philo)
		return (-1);
	res = print_status(philo, 3);
	if (res == -1)
		return (-1);
	else if (res == 0)
		return (0);
	if (!ft_sleep(philo->time_to_sleep))
		return (-1);
	return (1);
}

static int	thinking(t_philo *philo)
{
	int		res;

	if (!philo)
		return (-1);
	res = print_status(philo, 4);
	if (res == -1)
		return (-1);
	else if (res == 0)
		return (0);
	return (1);
}
