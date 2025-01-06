/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 15:07:14 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/06 13:35:14 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int is_phil_dead(t_philo **philos);
static int is_all_phils_eat(t_philo **philos);

int check(t_prog *prog)
{
	if (!prog || !prog->philos)
		return (printf("ERROR wait_results: !philo\n"), -1);
	void *results[prog->number_of_philosophers];
	int i = 0;
	while (i < prog->number_of_philosophers)
	{
		if (!prog->philos[i])
			return (printf("check ERROR: no philo %d\n", i + 1), -1);
		if (pthread_join(prog->philos[i]->thread, results[i]) != 0)
			return (printf("check ERROR: pthread_join %d\n", i), -1);
		i++;
	}
	while(1)
	{
		int eat_result = is_all_phils_eat(prog->philos);
		if (eat_result == -1)
			return (printf("ERROR check: eat_result\n"), -1);
		if (eat_result)
			return (1); // todo check result == 1 ???

		int dead_result = is_phil_dead(prog->philos);
		if (dead_result == -1)
		 	return (printf("ERROR check: dead_result\n"), -1);
		if (dead_result)
		{
			i = 0;
			while (prog->philos[i])
			{
				pthread_mutex_lock(prog->philos[i]->phil);
				prog->philos[i]->is_dead = 1;
				pthread_mutex_unlock(prog->philos[i]->phil);
				i++;
			}
			// todo wait result???
			return 0;
		}
		usleep(10 * 1000);
	}

	return -1;
}

static int is_phil_dead(t_philo **philos)
{
	int i;

	if (!philos)
		return (printf("ERROR: is_phil_dead !philo\n"), -1);
	i = 0;
	while (philos[i])
	{

		pthread_mutex_lock(philos[i]->phil);
		if (philos[i]->must_eat_times != 0)
		{
			long now = get_time();
			if (now == -1)
				return -1;
			if (now - philos[i]->time > (long)philos[i]->time_to_die)
			{
				pthread_mutex_lock(philos[i]->print);
				printf("CHECKER - Philos %d DEAD!!!!!!!\n", i + 1);
				printf("time_to_die: %ld\n", (long)philos[i]->time_to_die);
				printf("philos.time: %ld\n", philos[i]->time);
				printf("now time: %ld\n", now);
				printf("WHY DEAD now - philos.time: %ld\n", now - philos[i]->time);
				printf("MAX displayes time: %ld\n", philos[i]->time + (long)philos[i]->time_to_die);
				printf("print different (if > 10 error): %ld\n", now - philos[i]->time - (long)philos[i]->time_to_die);
				pthread_mutex_unlock(philos[i]->print);
				pthread_mutex_unlock(philos[i]->phil);
				return (1);
			}
		}
		pthread_mutex_unlock(philos[i]->phil);
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
		pthread_mutex_lock(philos[i]->phil);
		if (philos[i]->must_eat_times != 0)
		{
			pthread_mutex_unlock(philos[i]->phil);
			return (0);
		}
		pthread_mutex_unlock(philos[i]->phil);
		i++;
	}
	return (1);
}
