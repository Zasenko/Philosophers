/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 12:19:47 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/23 12:50:18 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	give_forks(t_prog *prog, t_philo *phil, int i)
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

t_philo	**create_philos(t_prog *prog)
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
		philos[i] = init_philos(prog, i);
		if (!philos[i])
			return (free_philos(philos), NULL);
		i++;
	}
	return (philos);
}

int	parse_arg(t_prog *prog, int argc, char **argv)
{
	if (!prog || !argv)
		return (0);
	if (argc < 5 || argc > 6)
		return (printf("Error: Wrong arguments count\n"), 0);
	prog->number_of_philosophers = make_number(argv[1]);
	if (!prog->number_of_philosophers)
		return (0);
	prog->time_to_die = make_number(argv[2]);
	if (!prog->time_to_die)
		return (0);
	prog->time_to_eat = make_number(argv[3]);
	if (!prog->time_to_eat)
		return (0);
	prog->time_to_sleep = make_number(argv[4]);
	if (!prog->time_to_sleep)
		return (0);
	if (argc == 6)
	{
		prog->must_eat_times = make_number(argv[5]);
		if (!prog->must_eat_times)
			return (0);
	}
	return (1);
}

int	parse(t_prog *prog, int argc, char **argv)
{
	if (!prog || !argv)
		return (0);
	if (!parse_arg(prog, argc, argv))
		return (0);
	prog->forks = init_forks(prog->number_of_philosophers);
	if (!prog->forks)
		return (0);
	prog->philos = create_philos(prog);
	if (!prog->philos)
		return (0);
	return (1);
}
