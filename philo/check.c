/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 15:07:14 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/04 14:37:05 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int is_phil_dead(t_philo **philos);
static int is_all_phils_eat(t_philo **philos);

int check(t_prog *prog)
{
	if (!prog || !prog->philos)
		return (printf("ERROR wait_results: !philo\n"), -1);
	
	// while (1)
	// check if someome is dead -> all should dead
	// if all philos have eaten -> finish

	void *results[prog->number_of_philosophers];
	
	int i = 0;
	while (i < prog->number_of_philosophers)
	{
		if (!prog->philos[i])
			return (printf("check ERROR: no philo %d\n", i + 1), -1);
		if (pthread_join(prog->philos[i], results[i]) != 0)
			return (printf("check ERROR: pthread_join %d\n", i), -1);
		i++;
	}
	
	while(1)
	{
		i = 0;
		while(i < prog->number_of_philosophers)
		{
			if ((char *)results[i] == "0")
			{
				printf("Result %d NULL\n", i);
				return (0);
			}
			i++;
		}
		i = 0;
		while(i < prog->number_of_philosophers)
		{
			if ((char *)results[i] == 1)
				i++;
			else
		}
		return (0);

		// int eat_result = is_all_phils_eat(prog->philos);
		// int dead_result = is_phil_dead(prog->philos);

		// // todo check results! or error :) int	wait_results(t_philo *philo)

		// if (eat_result == -1 || dead_result == -1)
		// 	return (printf("ERROR check: eat_result || dead_result\n"), -1);
		
		// if (dead_result)
		// 	return (0);
		// if (eat_result)
		// 	return (1);
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
	pthread_mutex_lock(philos[i]->print);
	printf("CHECKER: All philos eat\n");
	pthread_mutex_unlock(philos[i]->print);
	return (1);
}