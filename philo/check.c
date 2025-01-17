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
				int i = 0;
				while (prog->philos[i])
				{
					pthread_mutex_lock(prog->philos[i]->is_dead_mutex);
					prog->philos[i]->is_dead = 1;
					pthread_mutex_unlock(prog->philos[i]->is_dead_mutex);
					i++;
				}
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
		//usleep(100);
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

		if (prog->must_eat_times != -1)
		{

			pthread_mutex_lock(philos[i]->must_eat_times_mutex);
			if (philos[i]->must_eat_times < 1)
			{
				pthread_mutex_unlock(philos[i]->must_eat_times_mutex);
				i++;
			}
			else 
			{
				pthread_mutex_unlock(philos[i]->must_eat_times_mutex);

				pthread_mutex_lock(philos[i]->time_mutex);
				long now = get_time();
				if (now - philos[i]->time > philos[i]->time_to_die)
				{
					pthread_mutex_unlock(philos[i]->time_mutex);

					int z = 0;
					while (philos[z])
					{
						pthread_mutex_lock(philos[z]->is_dead_mutex);
						philos[z]->is_dead = 1;
						pthread_mutex_unlock(philos[z]->is_dead_mutex);
						z++;
					}
					pthread_mutex_lock(prog->print);
					long now2 = get_time();
					printf("%ld %ld %d died\n", now - philos[i]->time, now2 - now, philos[i]->i);
					printf("\033[32;1m%ld %d died\033[0m\n", now - prog->start_time, philos[i]->i);

					pthread_mutex_unlock(prog->print);
					return (1);
				}
				pthread_mutex_unlock(philos[i]->time_mutex);

				i++;
			}
			
		}
		else
		{
			pthread_mutex_lock(philos[i]->time_mutex);
			long now = get_time();
			if (now - philos[i]->time > philos[i]->time_to_die)
			{
				pthread_mutex_unlock(philos[i]->time_mutex);
				// pthread_mutex_lock(prog->is_dead_mutex);
				// *prog->is_dead = 1;
				// pthread_mutex_unlock(prog->is_dead_mutex);
				int z = 0;
				while (philos[z])
				{
					pthread_mutex_lock(philos[z]->is_dead_mutex);
					philos[z]->is_dead = 1;
					pthread_mutex_unlock(philos[z]->is_dead_mutex);
					i++;
				}
				pthread_mutex_lock(prog->print);
				long now2 = get_time();
				printf("%ld %ld %ld %d died\n", now, now2, now2 - now, philos[i]->i);
				// printf("%ld %d died\n", now - prog->start_time, philos[i]->i);
				pthread_mutex_unlock(prog->print);
				return (1);
			}
			pthread_mutex_unlock(philos[i]->time_mutex);
			i++;
		}
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
