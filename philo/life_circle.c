/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_circle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 10:31:24 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/08 14:27:02 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int check_if_dead(t_philo *philo);
int take_fork(t_philo *philo, pthread_mutex_t *fork);
int take_forks_test(t_philo *philo, pthread_mutex_t *fork1, pthread_mutex_t *fork2);
int take_forks(t_philo *philo);
int eating(t_philo *philo);
int sleeping(t_philo *philo);
int thinking(t_philo *philo);

int philo_circle(t_philo *philo)
{
	int take_result;
	int eating_result;
	int sleeping_result;

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

int check_if_dead(t_philo *philo)
{
	if (!philo)
		return (-1);
	pthread_mutex_lock(philo->is_dead_mutex); // error
	if (philo->is_dead == 1)
			return (pthread_mutex_unlock(philo->is_dead_mutex), 1); // error
	pthread_mutex_unlock(philo->is_dead_mutex);
	return (0);
}

int take_fork(t_philo *philo, pthread_mutex_t *fork)
{
	long time;
	int death_res;

	if (!philo || !fork)
		return (-1);
	if (pthread_mutex_lock(fork) != 0)
		return (-1);
	death_res = check_if_dead(philo);
	if (death_res == -1)
		return (pthread_mutex_unlock(fork), -1);
	else if (death_res)
		return (pthread_mutex_unlock(fork), 0);
	time = get_time();
	if (time == -1)
		return (pthread_mutex_unlock(fork), -1);

	if (pthread_mutex_lock(philo->print) != 0)
		return (pthread_mutex_unlock(fork), -1);
	printf("%ld %d has taken a fork\n", time, philo->i);
	if (pthread_mutex_unlock(philo->print) != 0)
		return (pthread_mutex_unlock(fork), -1);
	return (1);
}

int take_forks_test(t_philo *philo, pthread_mutex_t *fork1, pthread_mutex_t *fork2)
{
	int res;

	if (!philo || !fork1 || !fork2)
		return (perror("take_forks_test error"), -1);
	res = take_fork(philo, fork1);
	if (res == -1)
		return (-1);
	else if (res == 0)
		return (0);
	res = take_fork(philo, fork2);
	if (res == -1)
		return (pthread_mutex_unlock(fork1), -1);
	else if (res == 0)
		return (pthread_mutex_unlock(fork1), 0);
	return 1;
}

int take_forks(t_philo *philo)
{
	int fork_res;

	if (!philo)
		return (-1);
	long time = get_time();
	if (time == -1)
		return (-1);

	if (philo->i % 2 != 0)
	{
		fork_res = take_forks_test(philo, philo->fork1, philo->fork2);
		if (fork_res == -1)
			return (-1);
		else if (!fork_res)
			return (0);
	}
	else
	{
		fork_res = take_forks_test(philo, philo->fork2, philo->fork1);
		if (fork_res == -1)
			return (-1);
		else if (!fork_res)
			return (0);
	}
	return (1);
}

int	eating(t_philo *philo)
{
	long	time;
//	int death_res;

	if (!philo)
		return (-1);
	time = get_time();
	if (time == -1)
		return (-1);
	if (pthread_mutex_lock(philo->time_mutex) != 0)
		return (-1);
	philo->time = time;
	if (pthread_mutex_unlock(philo->time_mutex) != 0)
		return (-1);

	time = get_time();
	if (time == -1)
		return (-1);
	pthread_mutex_lock(philo->print);
	printf("%ld %d is eating\n", time, philo->i);
	pthread_mutex_unlock(philo->print);

	long now;
	now = get_time();
	while (now - time < philo->time_to_eat)
	{
		usleep(100);
		// death_res = check_if_dead(philo);
		// if (death_res == -1)
		// 	return (-1);
		// else if (death_res)
		// 	return (0);
		now = get_time();
	}
	if (philo->i % 2 != 0)
	{
		if (pthread_mutex_unlock(philo->fork1) != 0)
			return (perror("pthread_mutex_unlock error"), -1);
		if (pthread_mutex_unlock(philo->fork2) != 0)
			return (perror("pthread_mutex_unlock error"), -1);
	}
	else
	{
		if (pthread_mutex_unlock(philo->fork2) != 0)
			return (perror("pthread_mutex_unlock error"), -1);
		if (pthread_mutex_unlock(philo->fork1) != 0)
			return (perror("pthread_mutex_unlock error"), -1);
	}
	return (1);
}

int sleeping(t_philo *philo)
{
	long time;
	int death_res;

	if (!philo)
		return (-1);
	death_res = check_if_dead(philo);
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

	long now;
	now = get_time();
	while (now - time < philo->time_to_sleep)
	{
		usleep(100);
		// death_res = check_if_dead(philo);
		// if (death_res == -1)
		// 	return (-1);
		// else if (death_res)
		// 	return (0);
		
		now = get_time();
	}
	return (1);
}

int	thinking(t_philo *philo)
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
	printf("%ld %d is thinking\n",time , philo->i);
	pthread_mutex_unlock(philo->print);
	return (1);
}
