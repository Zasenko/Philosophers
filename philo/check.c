/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 15:07:14 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/16 14:07:58 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int is_phil_dead(t_philo **philos, t_prog *prog);
static int is_all_phils_eat(t_philo **philos);

int check(t_prog *prog)
{
	if (!prog || !prog->philos)
		return (printf("ERROR wait_results: !philo\n"), -1);
	while(1)
	{
		if (prog->must_eat_times != -1)
		{
			int eat_result = is_all_phils_eat(prog->philos);
			if (eat_result == -1)
				return (printf("ERROR check: eat_result\n"), -1);
			if (eat_result)
			{
				pthread_mutex_lock(prog->is_dead_mutex);
				*prog->is_dead = 1;
				pthread_mutex_unlock(prog->is_dead_mutex);
				return (1);
			}
		}
		int dead_result = is_phil_dead(prog->philos, prog);
		if (dead_result == -1)
		 	return (printf("ERROR check: dead_result\n"), -1);
		if (dead_result)
		{
			return 0;
		}
		usleep(50);
	}
	return -1;
}

static int is_phil_dead(t_philo **philos, t_prog *prog)
{
	int i;

	if (!philos || !prog)
		return (-1);
	i = 0;
	while (philos[i])
	{

		pthread_mutex_lock(philos[i]->time_mutex);
		long now = get_time();
		if (now == -1)
			return (pthread_mutex_unlock(philos[i]->time_mutex), -1);
		if (now - philos[i]->time > (long)philos[i]->time_to_die)
		{
			pthread_mutex_unlock(philos[i]->time_mutex);
			pthread_mutex_lock(prog->is_dead_mutex);
			*prog->is_dead = 1;
			pthread_mutex_unlock(prog->is_dead_mutex);
			pthread_mutex_lock(prog->print);
			long now2 = get_time();
			//printf("%ld %ld %ld %d died\n", now, now2, now2 - now, philos[i]->i);
			printf("%ld %d died\n", now2 - prog->start_time, philos[i]->i);
			pthread_mutex_unlock(prog->print);
			return (1);
		}
		pthread_mutex_unlock(philos[i]->time_mutex);
		i++;
	}
	return (0);
}

static int is_all_phils_eat(t_philo **philos)
{
	int i;

	if (!philos)
		return (printf("ERROR: is_all_phils_eat !philo\n"), -1);
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
