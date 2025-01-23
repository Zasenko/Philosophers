/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 13:29:27 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/23 12:46:22 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "errno.h"

void	free_prog(t_prog *prog)
{
	if (!prog)
		return ;
	free_forks(prog->forks);
	prog->forks = NULL;
	free_philos(prog->philos);
	prog->philos = NULL;
	if (prog->all_philos_created)
	{
		free(prog->all_philos_created);
		prog->all_philos_created = NULL;
	}
	if (prog->is_dead)
	{
		free(prog->is_dead);
		prog->is_dead = NULL;
	}
	free_prog_mutexes(prog);
}

void	free_prog_mutexes(t_prog *prog)
{
	if (!prog)
		return ;
	if (prog->print != NULL)
	{
		destroy_mutex(prog->print);
		free(prog->print);
		prog->print = NULL;
	}
	if (prog->all_philos_created_mutex)
	{
		destroy_mutex(prog->all_philos_created_mutex);
		free(prog->all_philos_created_mutex);
		prog->all_philos_created_mutex = NULL;
	}
	if (prog->is_dead_mutex)
	{
		destroy_mutex(prog->is_dead_mutex);
		free(prog->is_dead_mutex);
		prog->is_dead_mutex = NULL;
	}
}

void	destroy_mutex(pthread_mutex_t *mutex)
{
	int	result;

	if (!mutex)
		return ;
	result = pthread_mutex_destroy(mutex);
	if (result == EBUSY)
		pthread_mutex_unlock(mutex);
}

void	free_str(char *s)
{
	if (s)
		free(s);
}
