/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 13:29:38 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/27 11:19:18 by dzasenko         ###   ########.fr       */
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

pthread_mutex_t	**init_forks(int num)
{
	pthread_mutex_t	**forks;
	int				i;

	forks = (pthread_mutex_t **)malloc(sizeof(pthread_mutex_t *) * (num + 1));
	if (!forks)
		return (NULL);
	i = 0;
	while (i <= num)
	{
		forks[i] = NULL;
		i++;
	}
	i = 0;
	while (i < num)
	{
		forks[i] = init_mutex();
		if (forks[i] == NULL)
			return (free_forks(forks), NULL);
		i++;
	}
	return (forks);
}
