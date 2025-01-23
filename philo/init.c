/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 13:29:38 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/23 11:20:07 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

pthread_mutex_t	*init_mutex(void)
{
	pthread_mutex_t	*mutex;

	mutex = malloc(sizeof(pthread_mutex_t));
	if (!mutex)
		return (NULL);
	if (pthread_mutex_init(mutex, NULL) != 0)
		return (NULL);
	return (mutex);
}

int	init_prog_mutexes(t_prog *prog)
{
	if (!prog)
		return (0);
	prog->all_philos_created_mutex = init_mutex();
	if (!prog->all_philos_created_mutex)
		return (0);
	prog->is_dead_mutex = init_mutex();
	if (!prog->is_dead_mutex)
		return (0);
	prog->print = init_mutex();
	if (!prog->print)
		return (0);
	return (1);
}

t_prog	new_prog(void)
{
	t_prog	prog;

	prog.number_of_philosophers = 0;
	prog.time_to_die = 0;
	prog.must_eat_times = -1;
	prog.time_to_eat = 0;
	prog.time_to_sleep = 0;
	prog.philos = NULL;
	prog.forks = NULL;
	prog.start_time = 0;
	prog.all_philos_created = NULL;
	prog.all_philos_created_mutex = NULL;
	prog.is_dead_mutex = NULL;
	prog.print = NULL;
	prog.is_dead = NULL;
	return (prog);
}

int	init_prog(t_prog *prog)
{
	if (!prog)
		return (0);
	prog->all_philos_created = malloc(sizeof(int));
	if (!prog->all_philos_created)
		return (0);
	*prog->all_philos_created = 0;
	prog->is_dead = malloc(sizeof(int));
	if (!prog->is_dead)
		return (0);
	*prog->is_dead = 0;
	if (!init_prog_mutexes(prog))
		return (0);
	return (1);
}
