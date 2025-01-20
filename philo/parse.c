/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 12:19:47 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/20 12:11:38 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

pthread_mutex_t	**init_forks(int num)
{
	pthread_mutex_t	**forks;
	int				i;
	
	forks = (pthread_mutex_t **)malloc(sizeof(pthread_mutex_t *) * (num + 1));
	if (!forks)
		return (printf("Malloc error\n"), NULL);
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

int give_forks(t_prog *prog, t_philo *phil, int i)
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

t_philo *init_philos(t_prog *prog, int i)
{
	t_philo *phil;
	long now;

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
	now = get_time();
	if (now == -1)
		return (free_phil(phil), NULL);
	phil->time = now;
	if (!give_forks(prog, phil, i))
		return (free_phil(phil), NULL);
	phil->must_eat_times_mutex = init_mutex();
	if (!phil->must_eat_times_mutex)
		return (free_phil(phil), NULL);
	phil->time_mutex = init_mutex();
	if (!phil->time_mutex)
		return (free_phil(phil), NULL);
	return phil;
}

t_philo	**create_philos(t_prog *prog)
{
	t_philo	**philos;
	int		i;
	
	philos = (t_philo **)malloc(sizeof(t_philo *) * (prog->number_of_philosophers + 1));
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

int parse(t_prog *prog, int argc, char **argv)
{
	// TODO TRIM each arg
	if (!prog || !argv || !*argv)
		return (-1);
	if (argc < 5 || argc > 6)
		return (printf("Error: Wrong arguments count\n"), -1);

	int number_of_philosophers = atoi(argv[1]); // todo atoi
	if (number_of_philosophers < 1)
		return (printf("Wrong philosophers count. It should be > 0\n"), -1);
	prog->number_of_philosophers = number_of_philosophers;
	prog->time_to_die = atoi(argv[2]);	 // todo atoi
	prog->time_to_eat = atoi(argv[3]);	 // todo atoi
	prog->time_to_sleep = atoi(argv[4]); // todo atoi
	if (argc == 6)
	{
		prog->must_eat_times = atoi(argv[5]); // todo atoi
		if (prog->must_eat_times < 1)
			return (printf("number of times each philosopher must eat should be > 0\n"), -1);
	}

	prog->forks = init_forks(number_of_philosophers);
	if (!prog->forks)
		return (-1);
	prog->philos = create_philos(prog);
	if (!prog->philos)
		return (-1);
	return (1);
}
