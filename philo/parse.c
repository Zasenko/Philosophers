/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 12:19:47 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/16 13:17:24 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	parse(t_prog *prog, int argc, char **argv)
{
	// TODO TRIM each arg
	if (!prog || !argv || !*argv)
		return (-1);
	if (argc < 5 || argc  > 6)
		return (printf("Wrong argumnts count\n"), -1);
	
	int number_of_philosophers = atoi(argv[1]);	//todo atoi
	if (number_of_philosophers < 1)
		return (printf("Wrong philosophers count. It should be > 0\n"), -1);
	prog->number_of_philosophers = number_of_philosophers;
	prog->time_to_die = atoi(argv[2]); // todo atoi
	prog->time_to_eat = atoi(argv[3]); // todo atoi
	prog->time_to_sleep = atoi(argv[4]); // todo atoi
	if (argc == 6)
	{
		prog->must_eat_times = atoi(argv[5]);//todo atoi
		if (prog->must_eat_times < 1)
			return (printf("number of times each philosopher must eat should be > 0\n"), -1);
	}

	pthread_mutex_t **forks;
	forks = (pthread_mutex_t **)malloc(sizeof(pthread_mutex_t *) * (number_of_philosophers + 1));
	if (!forks)
		return (printf("malloc error\n"), -1);
	int i = 0;
	while (i <= number_of_philosophers)
	{
		forks[i] = NULL;
		i++;
	}
	i = 0;
	while (i < number_of_philosophers)
	{
		forks[i] = malloc(sizeof(pthread_mutex_t));
		if (forks[i] == NULL)
		{
			printf("ERROR malloc. Fork index: %d\n", i);
			free_forks(forks);
			return(-1);
		}
		if (pthread_mutex_init(forks[i], NULL) != 0)
		{
			printf("ERROR pthread_mutex_init. Fork index: %d\n", i);
			free_forks(forks);
			return (-1);
		}
		i++;
	}
	prog->forks = forks;

	t_philo **philos;
	philos = (t_philo **)malloc(sizeof(t_philo *) * (number_of_philosophers +1));
	if (!philos)
	{
		return (printf("malloc error\n"), -1);
	}
	i = 0;
	while (i <= number_of_philosophers)
	{
		philos[i] = NULL;
		i++;
	}
	i = 0;
	while (i < number_of_philosophers)
	{
		philos[i] = malloc(sizeof(t_philo));
		if (philos[i] == NULL)
		{
			free_philos(philos);
			return (printf("malloc error\n"), -1);
		}
		philos[i]->i = i + 1;
		philos[i]->must_eat_times = prog->must_eat_times;
		philos[i]->time_to_die = prog->time_to_die;
		philos[i]->time_to_eat = prog->time_to_eat;
		philos[i]->time_to_sleep = prog->time_to_sleep;
		philos[i]->number_of_philosophers = prog->number_of_philosophers;

		long now = get_time();
		if (now == -1)
			return (free_philos(philos), -1);
		philos[i]->time = now;

		if (number_of_philosophers == 1)
		{
			philos[i]->fork1 = forks[i];
			philos[i]->fork2 = NULL;
		}
		else
		{
			if (philos[i]->i % 2 == 0)
			{
				philos[i]->fork1 = forks[i - 1];
				philos[i]->fork2 = forks[i];
			}
			else
			{
				philos[i]->fork1 = forks[i];
				if (i == 0)
					philos[i]->fork2 = forks[number_of_philosophers - 1];
				else
					philos[i]->fork2 = forks[i - 1];
			}
		}
		philos[i]->must_eat_times_mutex = malloc(sizeof(pthread_mutex_t));
		if (philos[i]->must_eat_times_mutex == NULL)
		{
			printf("ERROR malloc. phil must_eat_times_mutex index: %d\n", i);
			free_philos(philos);
			return (-1);
		}
		if (pthread_mutex_init(philos[i]->must_eat_times_mutex, NULL) != 0)
		{
			// todo free must eat time ///
			printf("ERROR pthread_mutex_init. Fork index: %d\n", i);
			free_philos(philos);
			return (-1);
		}
		philos[i]->time_mutex = malloc(sizeof(pthread_mutex_t));
		if (philos[i]->time_mutex == NULL)
		{
			printf("ERROR malloc. phil time_mutex index: %d\n", i);
			free_philos(philos);
			return (-1);
		}
		if (pthread_mutex_init(philos[i]->time_mutex, NULL) != 0)
		{
			// todo free must eat time ///
			printf("ERROR pthread_mutex_init phil time_mutex %d\n", i);
			free_philos(philos);
			return (-1);
		}
		philos[i]->is_dead_mutex = prog->is_dead_mutex;
		philos[i]->is_dead = prog->is_dead;
		philos[i]->print = prog->print;
		philos[i]->all_philos_created = prog->all_philos_created;
		philos[i]->all_philos_created_mutex = prog->all_philos_created_mutex;
		i++;
	}
	prog->philos = philos;
	return (1);
}
