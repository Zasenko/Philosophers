/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 10:49:57 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/23 12:52:49 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	wait_philos_creations(t_philo *philo)
{
	int	all_philos_created;
	int	death_res;

	if (!philo)
		return (-1);
	pthread_mutex_lock(philo->all_philos_created_mutex);
	all_philos_created = *philo->all_philos_created;
	pthread_mutex_unlock(philo->all_philos_created_mutex);
	while (!all_philos_created)
	{
		pthread_mutex_lock(philo->all_philos_created_mutex);
		all_philos_created = *philo->all_philos_created;
		pthread_mutex_unlock(philo->all_philos_created_mutex);
		death_res = check_if_dead(philo);
		if (death_res == -1)
			return (-1);
		else if (death_res)
			return (0);
		usleep(100);
	}
	pthread_mutex_lock(philo->time_mutex);
	philo->time = get_time();
	if (!philo->time)
		return (pthread_mutex_unlock(philo->time_mutex), -1);
	return (pthread_mutex_unlock(philo->time_mutex), 1);
}

int	wait_hungry(t_philo *philo)
{
	long	time;
	long	now;
	int		death_res;

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
		usleep(100);
		death_res = check_if_dead(philo);
		if (death_res == -1)
			return (-1);
		else if (death_res)
			return (0);
		now = get_time();
		if (!now)
			return (-1);
	}
	return (1);
}

int	wait_result(t_philo *philo)
{
	void	*result;

	if (!philo)
		return (0);
	if (pthread_join(philo->thread, &result) != 0)
		return (0);
	if (result == NULL)
		return (0);
	return (1);
}

int	wait_finishing(t_prog *prog)
{
	int	i;
	int	result;
	int	flag;

	if (!prog)
		return (0);
	flag = 1;
	i = 0;
	while (prog->philos[i])
	{
		result = wait_result(prog->philos[i]);
		if (!result)
			flag = 0;
		i++;
	}
	return (flag);
}
