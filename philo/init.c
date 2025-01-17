/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 13:29:38 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/16 13:00:57 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int init_prog(t_prog *prog)
{
	if (!prog)
		return (0);
	prog->number_of_philosophers = 0;
	prog->time_to_die = 0;
	prog->must_eat_times = -1;
	prog->time_to_eat = 0;
	prog->time_to_sleep = 0;
	prog->philos = NULL;
	prog->forks = NULL;
	prog->start_time = 0;

	prog->all_philos_created = malloc(sizeof(int));
	if (!prog->all_philos_created)
		return (0);
	*prog->all_philos_created = 0;

	prog->all_philos_created_mutex = malloc(sizeof(pthread_mutex_t));
	if (!prog->all_philos_created_mutex)
		return (0);
	if (pthread_mutex_init(prog->all_philos_created_mutex, NULL) != 0)
		return (0);

	prog->is_dead = malloc(sizeof(int));
	if (!prog->is_dead)
		return (0);
	*prog->is_dead = 0;

	prog->is_dead_mutex = malloc(sizeof(pthread_mutex_t));
	if (!prog->is_dead_mutex)
		return (0);
	if (pthread_mutex_init(prog->is_dead_mutex, NULL) != 0)
		return (0);

	prog->print = malloc(sizeof(pthread_mutex_t));
	if (!prog->print)
		return (0);
	if (pthread_mutex_init(prog->print, NULL) != 0)
		return (0);

	return (1);
}
