/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_circle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 10:31:24 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/21 12:35:51 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int check_if_dead(t_philo *philo);
int take_fork(t_philo *philo, pthread_mutex_t *fork);
int take_forks(t_philo *philo);
int eating(t_philo *philo);
int sleeping(t_philo *philo);
int thinking(t_philo *philo);


int philo_circle(t_philo *philo)
{
	int result;

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
//+
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

//+
int	take_fork(t_philo *philo, pthread_mutex_t *fork)
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
//+
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
	return 1;
}

//+ 1 0
int update_time(t_philo *philo)
{
	long time;
	
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

//
int	eating(t_philo *philo)
{
	int		res;

	if (!philo || !philo->must_eat_times_mutex || !philo->fork1 || !philo->fork2)
	{
		pthread_mutex_unlock(philo->fork1);
		pthread_mutex_unlock(philo->fork2);
		return (-1);
	}
	if (!update_time(philo))
	{
		pthread_mutex_unlock(philo->fork1);
		pthread_mutex_unlock(philo->fork2);
		return (-1);
	}
	res = print_status(philo, 2);
	if (res == -1)
	{
		pthread_mutex_unlock(philo->fork1);
		pthread_mutex_unlock(philo->fork2);
		return (-1);
	}
	else if (res == 0)
	{
		pthread_mutex_unlock(philo->fork1);
		pthread_mutex_unlock(philo->fork2);
		return (0);
	}
	if (!ft_sleep(philo->time_to_eat))
	{
		pthread_mutex_unlock(philo->fork1);
		pthread_mutex_unlock(philo->fork2);
		return (-1);
	}
	pthread_mutex_lock(philo->must_eat_times_mutex);
	philo->must_eat_times--;
	pthread_mutex_unlock(philo->must_eat_times_mutex);
	pthread_mutex_unlock(philo->fork1);
	pthread_mutex_unlock(philo->fork2);
	return (1);
}

//+
int	sleeping(t_philo *philo)
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

//+
int	thinking(t_philo *philo)
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
