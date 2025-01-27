/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_phil.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:18:56 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/27 11:30:15 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static t_philo	*init_phil(t_prog *prog, int i);
static int		set_phil_properties(t_prog *prog, t_philo *phil, int i);
static int		give_forks(t_prog *prog, t_philo *phil, int i);

t_philo	**init_philosophers(t_prog *prog)
{
	t_philo	**philos;
	int		i;

	philos = malloc(sizeof(t_philo *) * (prog->number_of_philosophers + 1));
	if (!philos)
		return (printf("Malloc error\n"), NULL);
	i = 0;
	while (i <= prog->number_of_philosophers)
	{
		philos[i] = NULL;
		i++;
	}
	i = 0;
	while (i < prog->number_of_philosophers)
	{
		philos[i] = init_phil(prog, i);
		if (!philos[i])
			return (free_philos(philos), NULL);
		i++;
	}
	return (philos);
}

static t_philo	*init_phil(t_prog *prog, int i)
{
	t_philo	*phil;

	if (!prog || !prog->forks)
		return (NULL);
	phil = malloc(sizeof(t_philo));
	if (phil == NULL)
		return (printf("malloc error\n"), NULL);
	if (!set_phil_properties(prog, phil, i))
		return (free_phil(phil), NULL);
	phil->must_eat_times_mutex = init_mutex();
	phil->time_mutex = init_mutex();
	if (!phil->time_mutex || !phil->must_eat_times_mutex)
		return (free_phil(phil), NULL);
	if (!give_forks(prog, phil, i))
		return (free_phil(phil), NULL);
	return (phil);
}

static int	set_phil_properties(t_prog *prog, t_philo *phil, int i)
{
	if (!prog || !phil)
		return (0);
	phil->i = i + 1;
	phil->is_thread_created = 0;
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
	return (1);
}

static int	give_forks(t_prog *prog, t_philo *phil, int i)
{
	if (!prog || !phil)
		return (0);
	if (prog->number_of_philosophers == 1)
	{
		phil->fork1 = prog->forks[i];
		phil->fork2 = NULL;
	}
	else
	{
		if (phil->i % 2 == 0)
		{
			phil->fork1 = prog->forks[i - 1];
			phil->fork2 = prog->forks[i];
		}
		else
		{
			phil->fork1 = prog->forks[i];
			if (i == 0)
				phil->fork2 = prog->forks[prog->number_of_philosophers - 1];
			else
				phil->fork2 = prog->forks[i - 1];
		}
	}
	return (1);
}
