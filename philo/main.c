/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 11:06:05 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/02 11:53:18 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long get_time()
{
	struct timeval tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (perror("gettimeofday error"), -1);
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

void	*create_philosopher(void *arg)
{
	t_philo	*philo;
	
	philo = (t_philo *)arg;
	if (!philo || !philo->fork1)
		return (NULL);
	
	long time;
	time = get_time();
	if (time == -1)
		return (NULL);
	
	philo->time = time;
	if (!philo->fork2)
	{
		printf("NO FORK 2!!!!!\n");
		return ((void *)"0");
	}
	else
	{
		if (philo->must_eat_times > 0)
		{
			while (philo->must_eat_times > 0)
			{
				int result = philo_circle(philo);
				if (result == -1)
					return (NULL);
				else if (result == 0)
					return ((void *)"0");
				philo->must_eat_times--;
			}
			return ((void *)"1");
		}
		else
		{
			while (1)
			{
				int result = philo_circle(philo);
				if (result == -1)
					return (NULL);
				else if (result == 0)
					return ((void *)"0");
			}
		}
	// printf("timestamp_in_ms %d died\n", philo->i);
	// TODO pthread_mutex_destroy
	}
	return (NULL);
}

int	wait_results(t_philo *philo)
{
	void *result;
	
	if (!philo)
		return (printf("ERROR wait_results: !philo\n"), -1);
	
	if (pthread_join(philo->thread, &result) != 0)
		return (perror("ERROR pthread_join \n"), -1);
	if (result == NULL)
	{
		printf("Result %d NULL\n", philo->i);
		return (-1);
	}
	else
	{
		printf("Result %d: %s\n", philo->i, (char *)result);
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
	prog.forks = NULL;

	if (parse(&prog, argc, argv) == -1)
	{
		return (EXIT_FAILURE);
	}
	int i = 0;
	while (prog.philos[i])
	{
		pthread_t thread;
    	printf("creating philosopher: %d\n", prog.philos[i]->i);
		if (pthread_create(&thread, NULL, create_philosopher, (void *)prog.philos[i]) != 0)
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
		int result = wait_results(prog.philos[i]);

		if (result == -1)
		{
			printf("ERROR wait_results:	 philosopher: %d\n", prog.philos[i]->i);

			//return (EXIT_FAILURE);
		}
		else if (result == 0)
		{
			printf("Philosopher: %d dead!!!!!\n", prog.philos[i]->i);
		}
		i++;
	}
		
	printf("END\n");
	//free prog

	if (prog.forks)
	{
		int i = 0;
		while (prog.forks[i])
		{
			printf("free fork %d\n", i);
			pthread_mutex_destroy(prog.forks[i]);
			free(prog.forks[i]);
			prog.forks[i] = NULL;
			i++;
		}
		free(prog.forks);
		prog.forks = NULL;
	}
	if (prog.philos)
	{
		int i = 0;
		while (prog.philos[i])
		{
			printf("free philos %d\n", i);

			free(prog.philos[i]);
			prog.philos[i] = NULL;
			i++;
		}
		free(prog.philos);
		prog.philos = NULL;
	}
	return (EXIT_SUCCESS);
}


// Test 1 800 200 200. The philosopher should not eat and should die.
// Test 5 800 200 200. No philosopher should die.
// Test 5 800 200 200 7. No philosopher should die and the simulation should stop when every philosopher has eaten at least 7 times.
// Test 4 410 200 200. No philosopher should die.
// Test 4 310 200 100. One philosopher should die.