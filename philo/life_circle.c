/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_circle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 10:31:24 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/16 14:06:17 by dzasenko         ###   ########.fr       */
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
	int take_result;
	int eating_result;
	int sleeping_result;
	int thinking_result;

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

	thinking_result = thinking(philo);
	if (thinking_result == -1)
		return (-1);
	else if (thinking_result == 0)
		return (0);
	return (1);
}

int check_if_dead(t_philo *philo)
{
	if (!philo)
		return (-1);
	pthread_mutex_lock(philo->is_dead_mutex);
	if (philo->is_dead == 1)
			return (pthread_mutex_unlock(philo->is_dead_mutex), 1);
	pthread_mutex_unlock(philo->is_dead_mutex);
	return (0);
}

int take_fork(t_philo *philo, pthread_mutex_t *fork)
{
	long time;
	int death_res;

	if (!philo || !fork)
		return (-1);
	pthread_mutex_lock(fork);
	death_res = check_if_dead(philo);
	if (death_res == -1)
		return (pthread_mutex_unlock(fork), -1);
	else if (death_res)
		return (pthread_mutex_unlock(fork), 0);
	pthread_mutex_lock(philo->print);
	time = get_time();
	printf("%ld %d has taken a fork\n", time - philo->start_time, philo->i);
	pthread_mutex_unlock(philo->print);
	return (1);
}

int take_forks(t_philo *philo)
{
	int fork_res;

	if (!philo || !philo->fork1 || !philo->fork2)
		return (-1);

	fork_res = take_fork(philo, philo->fork2);
	if (fork_res == -1)
		return (-1);
	else if (fork_res == 0)
		return (0);
	fork_res = take_fork(philo, philo->fork1);
	if (fork_res == -1)
		return (pthread_mutex_unlock(philo->fork2), -1);
	else if (fork_res == 0)
		return (pthread_mutex_unlock(philo->fork2), 0);
	return (1);
}

int eating(t_philo *philo)
{
	long time;
	//long now;

	if (!philo)
		return (-1);

	time = get_time();
	// Обновление времени последней еды
	pthread_mutex_lock(philo->time_mutex);
	philo->time = time;
	pthread_mutex_unlock(philo->time_mutex);

	// Вывод сообщения
	pthread_mutex_lock(philo->print);
	printf("%ld %d is eating\n", time - philo->start_time, philo->i);
	pthread_mutex_unlock(philo->print);

	// Уменьшаем количество оставшихся приемов пищи
	pthread_mutex_lock(philo->must_eat_times_mutex);
	philo->must_eat_times--;
	pthread_mutex_unlock(philo->must_eat_times_mutex);

	// Эмулируем процесс еды
	while (get_time() - time < philo->time_to_eat)
	{
		usleep(1000); // Оптимизированная задержка
	}

	// Освобождаем вилки
	pthread_mutex_unlock(philo->fork2);
	pthread_mutex_unlock(philo->fork1);

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
	pthread_mutex_lock(philo->print);
	time = get_time();
	printf("%ld %d is sleeping\n", time - philo->start_time, philo->i);
	pthread_mutex_unlock(philo->print);

	while (get_time() - time < philo->time_to_sleep)
	{
		usleep(1000);
	}
	return (1);
}

int	thinking(t_philo *philo)
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
	pthread_mutex_lock(philo->print);
	time = get_time();
	printf("%ld %d is thinking\n", time - philo->start_time, philo->i);
	pthread_mutex_unlock(philo->print);
	return (1);
}
