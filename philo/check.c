/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 15:07:14 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/03 15:08:34 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int is_phil_dead(t_philo **philos)
{
	int i;
	
	if (!philos)
		return (printf("ERROR: is_phil_dead !philo\n"), -1);
	i = 0;
	while (philos[i])
	{
		pthread_mutex_lock(philos[i]->phil);
		if ((long)philos[i]->time_to_sleep < philos[i]->time)
		{
			// pthread_mutex_lock(philos[i]->print);
			// printf("Philos %d DEAD!!!!!!!\n", i + 1);
			// pthread_mutex_unlock(philos[i]->print);
			pthread_mutex_unlock(philos[i]->phil);
			return (1);
		}
		pthread_mutex_unlock(philos[i]->phil);
		i++;
	}
	return (0);
}

int is_all_phils_eat(t_philo **philos)
{
	int i;
	
	if (!philos)
		return (printf("ERROR: is_all_phils_eat !philo\n"), -1);
	i = 0;
	while (philos[i])
	{
		pthread_mutex_lock(philos[i]->print);
    	printf("check is_all_phils_eat: %d\n", i);
		pthread_mutex_unlock(philos[i]->print);


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

int	check(t_prog *prog)
{
	if (!prog)
		return (printf("ERROR wait_results: !philo\n"), -1);
	
	// while (1)
	// check if someome is dead -> all should dead
	// if all philos have eaten -> finish
	while(1)
	{
		int eat_result = is_all_phils_eat(prog->philos);
		int dead_result = is_phil_dead(prog->philos);

		if (eat_result == -1 || dead_result == -1)
			return (printf("ERROR check: eat_result || dead_result\n"), -1);
		
		if (dead_result)
			return (0);
		if (eat_result)
			return (1);
		usleep(5);
	}
	return -1;
}