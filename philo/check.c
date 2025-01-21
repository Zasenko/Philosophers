/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 15:07:14 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/21 13:44:06 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_phil_dead(t_philo **philos, t_prog *prog);
static int	is_all_philos_eat(t_philo **philos);
static int	make_philos_dead(t_prog *prog);

int	check(t_prog *prog)
{
	int	result;

	if (!prog || !prog->philos)
		return (-1);
	while (1)
	{
		if (prog->must_eat_times != -1)
		{
			result = is_all_philos_eat(prog->philos);
			if (result == -1)
				return (-1);
			if (result)
			{
				if (!make_philos_dead(prog))
					return (-1);
				return (1);
			}
		}
		result = is_phil_dead(prog->philos, prog);
		if (result == -1)
			return (-1);
		if (result)
			return (0);
		usleep(50);
	}
	return (-1);
}

int	print_death(t_prog *prog, int phil_i)
{
	long	time;

	if (!prog || !prog->print)
		return (0);
	pthread_mutex_lock(prog->print);
	time = get_time();
	if (!time)
		return (pthread_mutex_unlock(prog->print), -1);
	printf("\033[31m]%ld %d died\033[31m]\n", time - prog->start_time, phil_i);
	pthread_mutex_unlock(prog->print);
	return (1);
}

static int	is_phil_dead(t_philo **philos, t_prog *prog)
{
	int		i;
	long	now;

	if (!philos || !prog)
		return (-1);
	i = 0;
	while (philos[i])
	{
		pthread_mutex_lock(philos[i]->time_mutex);
		now = get_time();
		if (!now)
			return (pthread_mutex_unlock(philos[i]->time_mutex), -1);
		if (now - philos[i]->time > (long)philos[i]->time_to_die)
		{
			pthread_mutex_unlock(philos[i]->time_mutex);
			if (!make_philos_dead(prog))
				return (-1);
			if (!print_death(prog, philos[i]->i))
				return (-1);
			return (1);
		}
		pthread_mutex_unlock(philos[i]->time_mutex);
		i++;
	}
	return (0);
}

static int	is_all_philos_eat(t_philo **philos)
{
	int	i;

	if (!philos)
		return (-1);
	i = 0;
	while (philos[i])
	{
		pthread_mutex_lock(philos[i]->must_eat_times_mutex);
		if (philos[i]->must_eat_times > 0)
		{
			pthread_mutex_unlock(philos[i]->must_eat_times_mutex);
			return (0);
		}
		pthread_mutex_unlock(philos[i]->must_eat_times_mutex);
		i++;
	}
	return (1);
}

static int	make_philos_dead(t_prog *prog)
{
	if (!prog || !prog->is_dead_mutex)
		return (0);
	pthread_mutex_lock(prog->is_dead_mutex);
	*prog->is_dead = 1;
	pthread_mutex_unlock(prog->is_dead_mutex);
	return (1);
}
