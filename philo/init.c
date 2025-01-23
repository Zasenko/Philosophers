/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 13:29:38 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/23 12:51:20 by dzasenko         ###   ########.fr       */
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

t_philo	*init_philos(t_prog *prog, int i)
{
	t_philo	*phil;

	if (!prog || !prog->forks)
		return (NULL);
	phil = malloc(sizeof(t_philo));
	if (phil == NULL)
		return (printf("malloc error\n"), NULL);
	phil->i = i + 1;
	phil->must_eat_times = prog->must_eat_times;
	phil->time_to_die = prog->time_to_die;
	phil->time_to_eat = prog->time_to_eat;
	phil->time_to_sleep = prog->time_to_sleep;
	phil->number_of_philosophers = prog->number_of_philosophers;
	phil->is_dead_mutex = prog->is_dead_mutex;
	phil->is_dead = prog->is_dead;
	phil->print = prog->print;
	phil->all_philos_created = prog->all_philos_created;
	phil->all_philos_created_mutex = prog->all_philos_created_mutex;
	phil->must_eat_times_mutex = init_mutex();
	phil->time_mutex = init_mutex();
	if (!phil->time_mutex || !phil->must_eat_times_mutex)
		return (free_phil(phil), NULL);
	if (!give_forks(prog, phil, i))
		return (free_phil(phil), NULL);
	return (phil);
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
