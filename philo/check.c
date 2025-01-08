/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 15:07:14 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/08 11:01:53 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int is_phil_dead(t_philo **philos);
static int is_all_phils_eat(t_philo **philos);

int check(t_prog *prog)
{
	if (!prog || !prog->philos)
		return (printf("ERROR wait_results: !philo\n"), -1);
	while(1)
	{
		int eat_result = is_all_phils_eat(prog->philos);
		if (eat_result == -1)
			return (printf("ERROR check: eat_result\n"), -1);
		if (eat_result)
			return (1);
		int dead_result = is_phil_dead(prog->philos);
		if (dead_result == -1)
		 	return (printf("ERROR check: dead_result\n"), -1);
		if (dead_result)
		{
			// int i = 0;
			// while (prog->philos[i])
			// {
			// 	pthread_mutex_lock(prog->philos[i]->is_dead_mutex);
			// 	prog->philos[i]->is_dead = 1;
			// 	pthread_mutex_unlock(prog->philos[i]->is_dead_mutex);
			// 	i++;
			// }
			return 0;
		}
		//usleep(1 * 1000);
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

		// pthread_mutex_lock(philos[i]->is_dead_mutex); // error
		// if (philos[i]->is_dead == 1)
		// {
		// 	//	printf("----life circle: %d already died\n", philo->i);
		// 	return (pthread_mutex_unlock(philos[i]->is_dead_mutex), 1); // error
		// }
		// pthread_mutex_unlock(philos[i]->is_dead_mutex);
		// long now = get_time();
		// if (now == -1)
		// 	return (-1); // unlock
		pthread_mutex_lock(philos[i]->time_mutex);
		long now = get_time();
		if (now == -1)
			return (pthread_mutex_unlock(philos[i]->time_mutex), - 1);

		if (now - philos[i]->time > (long)philos[i]->time_to_die)
		{
			pthread_mutex_unlock(philos[i]->time_mutex);

			pthread_mutex_lock(philos[i]->is_dead_mutex);
			philos[i]->is_dead = 1;
			pthread_mutex_unlock(philos[i]->is_dead_mutex);

			int z = 0;
			while (philos[z])
			{
				if (z != i)
				{
					pthread_mutex_lock(philos[z]->is_dead_mutex);
					philos[z]->is_dead = 1;
					pthread_mutex_unlock(philos[z]->is_dead_mutex);
				}
				z++;
			}
			pthread_mutex_lock(philos[i]->print);
			// printf("-----------------\n");
			printf("%ld %d died\n", now, philos[i]->i);
			printf("WHY DEAD: time now - last eat time: %ld\n", now - philos[i]->time);

			long new_now = get_time();
			if (new_now == -1)
				return (pthread_mutex_unlock(philos[i]->time_mutex), -1); // unlock

			printf("print different (new_now - (last eat time + time_to_die) ): %ld\n", new_now - (philos[i]->time + (long)philos[i]->time_to_die));
			//	printf("-----------------\n");
			pthread_mutex_unlock(philos[i]->print);
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
		if (philos[i]->must_eat_times != 0)
		{
			pthread_mutex_unlock(philos[i]->must_eat_times_mutex);
			return (0);
		}
		pthread_mutex_unlock(philos[i]->must_eat_times_mutex);
		i++;
	}
	return (1);
}
