/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_circle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 10:31:24 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/04 14:03:01 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int check_if_dead(t_philo *philo)
{
	if (!philo)
		return (-1);
	pthread_mutex_lock(philo->phil);
	if (philo->is_dead == 1)
		return (pthread_mutex_unlock(philo->phil), 1);
	pthread_mutex_unlock(philo->phil);
	return (0);
}

int	take_forks(t_philo *philo)
{
	long	time;

	if (!philo)
		return (-1);
	int death_res = check_if_dead(philo);
	if (death_res == -1)
		return (-1);
	else if (death_res)
		return (0);
	if (philo->i % 2 != 0)
	{
		if (pthread_mutex_lock(philo->fork1) != 0)
			return (perror("pthread_mutex_lock error"), -1);
		time = get_time();
		if (time == -1)
			return (-1);
		pthread_mutex_lock(philo->print);
		printf("%ld %d has taken a fork 1\n", time, philo->i);
		pthread_mutex_unlock(philo->print);

		if (pthread_mutex_lock(philo->fork2) != 0)
			return (perror("pthread_mutex_lock error"), -1); // todo unlock fork1
		time = get_time();
		if (time == -1)
			return (-1);
		pthread_mutex_lock(philo->print);
		printf("%ld %d has taken a fork 2\n", time, philo->i);
		pthread_mutex_unlock(philo->print);
	}
	else
	{
		if (pthread_mutex_lock(philo->fork2) != 0)
			return (perror("pthread_mutex_lock error"), -1);
		time = get_time();
		if (time == -1)
			return (-1);
		pthread_mutex_lock(philo->print);
		printf("%ld %d has taken a fork 2\n", time, philo->i);
		pthread_mutex_unlock(philo->print);
		if (pthread_mutex_lock(philo->fork1) != 0)
			return (perror("pthread_mutex_lock error"), -1); // todo unlock fork2
		time = get_time();
		if (time == -1)
			return (-1);
		pthread_mutex_lock(philo->print);
		printf("%ld %d has taken a fork 1\n", time, philo->i);
		pthread_mutex_unlock(philo->print);
	}
	return (1);
}

int	eating(t_philo *philo)
{
	long	time;

	if (!philo)
		return (-1);
	time = get_time();
	if (time == -1)
		return (-1);
	int death_res = check_if_dead(philo);
	if (death_res == -1)
		return (-1);
	else if (death_res)
		return (0);
	if (pthread_mutex_lock(philo->phil) != 0)
		return (-1);
	philo->time = time;
	if (pthread_mutex_unlock(philo->phil) != 0)
		return (-1); 
	pthread_mutex_lock(philo->print);
	printf("%ld %d is eating\n", time, philo->i);
	pthread_mutex_unlock(philo->print);
	usleep(philo->time_to_eat * 1000);
	if (philo->i % 2 == 0) // todo check   philo->i == 1?
	{
		if (pthread_mutex_unlock(philo->fork2) != 0)
			return (perror("pthread_mutex_unlock error"), -1);
		if (pthread_mutex_unlock(philo->fork1) != 0)
			return (perror("pthread_mutex_unlock error"), -1);
	}
	else
	{
		if (pthread_mutex_unlock(philo->fork1) != 0)
			return (perror("pthread_mutex_unlock error"), -1);
		if (pthread_mutex_unlock(philo->fork2) != 0)
			return (perror("pthread_mutex_unlock error"), -1);
	}
	// if (pthread_mutex_unlock(philo->fork1) != 0)
	// 	return (perror("pthread_mutex_unlock error"), -1);

	// pthread_mutex_lock(philo->print);
	// printf("Philo %d put fork 1\n", philo->i);//DELETE
	// pthread_mutex_unlock(philo->print);
	
	// if (pthread_mutex_unlock(philo->fork2) != 0)
	// 	return (perror("pthread_mutex_unlock error"), -1);

	// pthread_mutex_lock(philo->print);
	// printf("Philo %d put fork 2\n", philo->i);//DELETE
	// pthread_mutex_unlock(philo->print);
	
	return (1);
}

int	sleeping(t_philo *philo)
{
	long time;
	
	if (!philo)
		return (-1);
	int death_res = check_if_dead(philo);
	if (death_res == -1)
		return (-1);
	else if (death_res)
		return (0);
	time = get_time();
	if (time == -1)
		return (-1);
	pthread_mutex_lock(philo->print);
	printf("%ld %d is sleeping\n", time, philo->i);
	pthread_mutex_unlock(philo->print);
	usleep(philo->time_to_sleep * 1000);
	return (1);
}

int	thinking(t_philo *philo)
{
	long time;
	
	if (!philo)
		return (-1);
	time = get_time();
	if (time == -1)
		return (-1);
	int death_res = check_if_dead(philo);
	if (death_res == -1)
		return (-1);
	else if (death_res)
		return (0);
	pthread_mutex_lock(philo->print);
	printf("%ld %d is thinking\n",time , philo->i);
	pthread_mutex_unlock(philo->print);
	return (1);
}

int	philo_circle(t_philo *philo)
{
	int	take_result;
	int	eating_result;
	int	sleeping_result;
	
	if (!philo)
		return (-1);
	
	take_result = take_forks(philo);
	if (take_result == -1)
		return (-1);
	else if (take_result == 0)
		return (0);
	eating_result = eating(philo);
	if (eating_result == -1)
		return (-1);
	else if (eating_result == 0)
		return (0);
	sleeping_result = sleeping(philo);
	if (sleeping_result == -1)
		return (-1);
	else if (sleeping_result == 0)
		return (0);
	if (thinking(philo) == -1)
		return (-1);
	return (1);
}

//  1     2     3     4     5
// 0 0 | 0 0 | 0 0 | 0 0 | 0 0