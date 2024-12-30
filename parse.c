/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 12:19:47 by dzasenko          #+#    #+#             */
/*   Updated: 2024/12/30 15:07:49 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int parse(t_prog *prog, int argc, char **argv)
{
	// TODO TRIM each arg
	if (!prog || !argv || !*argv)
	{
		return (-1);
	}
	if (argc < 5 || argc  > 6)
	{
		write(1, "Wrong argumnts count\n", 21);
		return (-1);
	}
	
	// The number of philosophers and also the number of forks.
	int number_of_philosophers = atoi(argv[1]);	//todo atoi
	if (number_of_philosophers < 1)
	{
		write(1, "Wrong philosophers count\n", 21);
		return (-1);
	}

	// time_to_die
	// If a philosopher didn’t start eating time_to_die milliseconds since
	// the beginning of their last meal or the beginning of the simulation, they die.
	
	prog->time_to_die = atoi(argv[2]);//todo atoi

	//time_to_eat
	prog->time_to_eat = atoi(argv[3]);//todo atoi

	//time_to_sleep
	prog->time_to_sleep = atoi(argv[4]);//todo atoi
	
	//[number_of_times_each_philosopher_must_eat]
	//TODO!!! ?? why array?
	if (argc == 6)
	{
		prog->must_eat_times = atoi(argv[5]);//todo atoi		
	}


	t_philo **philos;
	philos = (t_philo **)malloc(sizeof(t_philo *) * (number_of_philosophers +1));
	if (!philos)
	{
		write(1, "malloc error\n", 13);
		return (-1);
	}
	int i = 0;
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
			// free **philos
		}
		philos[i]->must_eat_times = prog->must_eat_times;
		philos[i]->time_to_die = prog->time_to_die;
		philos[i]->time_to_eat = prog->time_to_eat;
		philos[i]->time_to_sleep = prog->time_to_sleep;
		philos[i]->result = NULL;
		philos[i]->thread = -1;
		philos[i]->i = i + 1;
		i++;
	}
	prog->philos = philos;
	return (1);
}