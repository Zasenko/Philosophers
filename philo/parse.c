/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 12:19:47 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/02 14:56:42 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	parse(t_prog *prog, int argc, char **argv)
{
	int	number_of_philosophers;
	
	// TODO TRIM each arg
	if (!prog || !argv || !*argv)
		return (-1);
	if (argc < 5 || argc  > 6)
		return (printf("Wrong argumnts count\n"), -1);
	
	number_of_philosophers = atoi(argv[1]);	//todo atoi
	if (number_of_philosophers < 1)
		return (printf("Wrong philosophers count. It should be > 0\n"), -1);
	prog->time_to_die = atoi(argv[2]);//todo atoi
	prog->time_to_eat = atoi(argv[3]);//todo atoi
	prog->time_to_sleep = atoi(argv[4]);//todo atoi
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
		philos[i]->time = -1;
		philos[i]->fork1 = forks[i];
		if (number_of_philosophers == 1)
			philos[i]->fork2 = NULL;
		else
		{
			if (i == 0)
				philos[i]->fork2 = forks[number_of_philosophers - 1];
			else
				philos[i]->fork2 = forks[i - 1];
		}
		
		if (pthread_mutex_init(philos[i]->phil, NULL) != 0)
		{
			printf("ERROR pthread_mutex_init Phil index: %d\n", i);
			// free
		}

		philos[i]->phil = malloc(sizeof(pthread_mutex_t));
		if (philos[i]->phil == NULL)
		{
			printf("ERROR malloc. phil phil index: %d\n", i);
			free_philos(philos);
			return (-1);
		}
		if (pthread_mutex_init(philos[i]->phil, NULL) != 0)
		{
			printf("ERROR pthread_mutex_init. Fork index: %d\n", i);
			free_philos(philos);
			return (-1);
		}
		i++;
	}
	printf("----------------\n");
	prog->philos = philos;
	return (1);
}