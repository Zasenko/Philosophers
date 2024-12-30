/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 11:06:05 by dzasenko          #+#    #+#             */
/*   Updated: 2024/12/30 15:22:47 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*create_philosopher(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	// (void)arg;
	// int *result = malloc(sizeof(int));
	// if (!result)
	// {
	// 	perror("malloc");
    //     return NULL;	
	// }
	printf("timestamp_in_ms %d has taken a fork\n", philo->i);
	printf("timestamp_in_ms %d is eating\n", philo->i);
	sleep(2);

	printf("timestamp_in_ms %d is sleeping\n", philo->i);
	sleep(1);

	printf("timestamp_in_ms %d is thinking\n", philo->i);
	sleep(2);

	printf("timestamp_in_ms %d died\n", philo->i);
	return (NULL);
}

int	wait_results(t_philo *philo)
{
	void *result;
	
	if (!philo)
		return (printf("ERROR wait_results: !philo\n"), -1);
	
	pthread_join(philo->thread, &result);
	if (result == NULL)
	{
		printf("Result %d NULL\n", philo->i);
		return (-1);
	}
	else
	{
		printf("Result %d not null\n", philo->i);
	}
	
	
	return (1);
}

int	main(int argc, char **argv)
{
	
	t_prog prog;

	prog.number_of_philosophers = 0;
	prog.time_to_die = 0;
	prog.must_eat_times = -1;
	prog.time_to_eat = 0;
	prog.time_to_sleep = 0;
	prog.philos = NULL;

	if (parse(&prog, argc, argv) == -1)
	{
		return (EXIT_FAILURE);
	}
	int i = 0;
	while (prog.philos[i])
	{
		pthread_t thread;
    	printf("creating philosopher: %d\n", prog.philos[i]->i);
		if (pthread_create(&thread, NULL, create_philosopher, (void *)prog.philos[i]))
		{
			perror("pthread_create");
			return (EXIT_FAILURE);
    	}
		prog.philos[i]->thread = thread;
		i++;
	}

	i = 0;
	
	while (prog.philos[i])
	{
		if (wait_results(prog.philos[i]) == -1)
		{
			printf("ERROR wait_results:	 philosopher: %d\n", prog.philos[i]->i);

			//return (EXIT_FAILURE);

		}
		else
		{
			printf("wait_results OK:  philosopher: %d\n", prog.philos[i]->i);

		}
		i++;
	}
		
	printf("END\n");
	//free prog
	return (EXIT_SUCCESS);
}


// Test 1 800 200 200. The philosopher should not eat and should die.
// Test 5 800 200 200. No philosopher should die.
// Test 5 800 200 200 7. No philosopher should die and the simulation should stop when every philosopher has eaten at least 7 times.
// Test 4 410 200 200. No philosopher should die.
// Test 4 310 200 100. One philosopher should die.