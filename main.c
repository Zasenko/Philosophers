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
	if (!philo)
		return (NULL);
	struct timeval t;

	if (gettimeofday(&t, NULL) == -1)
	{
		printf("ERROR gettimeofday\n");
		return (NULL);
	}
	printf("gettimeofday %lu, %d \n", t.tv_sec, t.tv_usec);

	// gettimeofday 1735624879, 358013
	// gettimeofday 1735624914, 530660

		// When a philosopher has finished eating, they put their forks back on the table and
		// start sleeping.Once awake, they start thinking again.The simulation stops when
		// a philosopher dies of starvation.

	if (philo->must_eat_times > 0) // TODO!!!! must_eat_times > 0    ???????
	{
		while (philo->must_eat_times > 0)
		{

			if (philo->i % 2 != 0)//async lock NOT WORKING sometimes
			{
				pthread_mutex_lock(philo->fork1);
				printf("Phil %d has taken a fork 1\n", philo->i);
				pthread_mutex_lock(philo->fork2);
				printf("Phil %d has taken a fork 2\n", philo->i);
			}
			else
			{
				pthread_mutex_lock(philo->fork2);
				printf("Phil %d has taken a fork 2\n", philo->i);
				pthread_mutex_lock(philo->fork1);
				printf("Phil %d has taken a fork 1\n", philo->i);
			}
			//sleep(5);
			printf("Phil %d is eating\n", philo->i);
			usleep(philo->time_to_eat);

			pthread_mutex_unlock(philo->fork1);
			printf("Phil %d has put a fork1\n", philo->i);

			pthread_mutex_unlock(philo->fork2);
			printf("Phil %d has put a fork2\n", philo->i);

			printf("Phil %d is sleeping\n", philo->i);
			usleep(philo->time_to_sleep);
			printf("Phil %d is thinking\n", philo->i);
			philo->must_eat_times--;
			printf("Phil %d must_eat_times: %d\n", philo->i, philo->must_eat_times);
		}
		return ((void *)"1");
	}
	else
	{
		while(1)
		{
			printf("timestamp_in_ms %d has taken a fork\n", philo->i);
			printf("timestamp_in_ms %d is eating\n", philo->i);
			usleep(philo->time_to_eat);
			printf("timestamp_in_ms %d is sleeping\n", philo->i);
			usleep(philo->time_to_sleep);
			printf("timestamp_in_ms %d is thinking\n", philo->i);
			return ((void *)"0");
		}
		return ((void *)"0");
	}
	// printf("timestamp_in_ms %d died\n", philo->i);
	// TODO pthread_mutex_destroy
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
		if (wait_results(prog.philos[i]) == -1)
		{
			printf("ERROR wait_results:	 philosopher: %d\n", prog.philos[i]->i);

			//return (EXIT_FAILURE);
		}
		else if (wait_results(prog.philos[i]) == 0)
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
			printf("free fork %d: %ld\n", i, prog.forks[i]->__sig);
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