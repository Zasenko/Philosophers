/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 11:06:05 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/03 14:40:54 by dzasenko         ###   ########.fr       */
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
	long	time_of_creations;
	
	philo = (t_philo *)arg;
	if (!philo)
		return (NULL);
	if (!philo->fork1)
		return (NULL);// ? todo free *philo
	time_of_creations = get_time();
	if (time_of_creations == -1)
		return (NULL);	// ? todo free *philo
	
	if (pthread_mutex_lock(philo->phil) != 0)
		return (NULL);	// ? todo free *philo
	philo->time = time_of_creations;
	if (pthread_mutex_unlock(philo->phil) != 0)
		return (NULL); // ? todo free *philo // how to unlock phil ahaha ????

	if (!philo->fork2)
	{
		printf("NO FORK 2 - DEATH !!!!!\n");
		return ((void *)"0");// ? todo free *philo
	}
	else
	{
		if (philo->must_eat_times > 0)
		{
			while (philo->must_eat_times > 0)
			{
				int result = philo_circle(philo);
				if (result == -1)
					return (NULL);// ? todo free *philo
				else if (result == 0)
					return ((void *)"0");// ? todo free *philo
				if (pthread_mutex_lock(philo->phil) != 0)
					return (NULL);	// ? todo free *philo
				philo->must_eat_times--;
				if (pthread_mutex_unlock(philo->phil) != 0)
					return (NULL); // ? todo free *philo // how to unlock if error?
			}
			return ((void *)"1");// ? todo free *philo
		}
		else
		{
			while (1)
			{
				int result = philo_circle(philo);
				if (result == -1)
					return (NULL);// ? todo free *philo
				else if (result == 0)
					return ((void *)"0");// ? todo free *philo
			}
		}
	}
	return (NULL);// ? todo free *philo
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
		pthread_mutex_lock(philo->print);
		printf("Result %d: %s\n", philo->i, (char *)result);
		pthread_mutex_unlock(philo->print);
	}
	return (1);
}

int is_phil_dead(t_philo **philos)
{
	int i;
	
	if (!philos)
		return (printf("ERROR: is_phil_dead !philo\n"), -1);
	i = 0;
	while (philos[i])
	{
		pthread_mutex_lock(philos[i]->phil);
		if ((long)philos[i]->time_to_sleep < philos[i]->time)
		{
			// pthread_mutex_lock(philos[i]->print);
			// printf("Philos %d DEAD!!!!!!!\n", i + 1);
			// pthread_mutex_unlock(philos[i]->print);
			return (1);
		}
		pthread_mutex_unlock(philos[i]->phil);
		i++;
	}
	return (0);
}

int is_all_phils_eat(t_philo **philos)
{
	int i;
	
	if (!philos)
		return (printf("ERROR: is_all_phils_eat !philo\n"), -1);
	i = 0;
	while (philos[i])
	{
		pthread_mutex_lock(philos[i]->print);
    	printf("check is_all_phils_eat: %d\n", i);
		pthread_mutex_unlock(philos[i]->print);

		
		pthread_mutex_lock(philos[i]->phil);
		if (philos[i]->must_eat_times != 0)
		{
			pthread_mutex_unlock(philos[i]->phil);
			return (0);
		}
		pthread_mutex_unlock(philos[i]->phil);
		i++;
	}
	return (1);
}

int	check(t_prog *prog)
{
	if (!prog)
		return (printf("ERROR wait_results: !philo\n"), -1);
	
	// while (1)
	// check if someome is dead -> all should dead
	// if all philos have eaten -> finish
	while(1)
	{
		int eat_result = is_all_phils_eat(prog->philos);
		int dead_result = is_phil_dead(prog->philos);

		if (eat_result == -1 || dead_result == -1)
			return (printf("ERROR check: eat_result || dead_result\n"), -1);
		
		if (dead_result)
			return (0);
		if (eat_result)
			return (1);
		usleep(5);
	}
	return -1;
}

int	main(int argc, char **argv)
{
	t_prog prog;

	if (!init_prog(&prog))
		return (free_prog(&prog), EXIT_FAILURE);
	if (parse(&prog, argc, argv) == -1)
		return (free_prog(&prog), EXIT_FAILURE);
	
	int i = 0;
	while (prog.philos[i])
	{
		pthread_t thread;
		pthread_mutex_lock(prog.print);
    	printf("creating philosopher: %d\n", prog.philos[i]->i);
		pthread_mutex_unlock(prog.print);
		if (pthread_create(&thread, NULL, create_philosopher, (void *)prog.philos[i]) != 0)
		{
			free_prog(&prog);
			perror("pthread_create");
			return (EXIT_FAILURE);
    	}
		prog.philos[i]->thread = thread;
		i++;
	}

	// wait result
	// while (1)
	// check if someome is dead -> all should dead
	// if all philos have eaten -> finish

	int	check_result = check(&prog);
	if (check_result == -1)
	{
		pthread_mutex_lock(prog.print);
		printf("check_result ERROR\n");
		pthread_mutex_unlock(prog.print);
		//return (free_prog(&prog), EXIT_FAILURE);	
	}
	else if (check_result == 0)
	{
		pthread_mutex_lock(prog.print);
		printf("Someone dead\n");
		pthread_mutex_unlock(prog.print);
		//return (free_prog(&prog), EXIT_SUCCESS);
	}
	else
	{
		//return (free_prog(&prog), EXIT_SUCCESS);
		pthread_mutex_lock(prog.print);
		printf("All eat\n");
		pthread_mutex_unlock(prog.print);
	}
	
	// remove!
	// i = 0;
	// while (prog.philos[i])
	// {
	// 	int result = wait_results(prog.philos[i]);
	// 	if (result == -1)
	// 	{
	// 		free_prog(&prog);
	// 		pthread_mutex_lock(prog.print);
	// 		printf("ERROR wait_results:	 philosopher: %d\n", prog.philos[i]->i);
	// 		pthread_mutex_unlock(prog.print);
	// 		//return (EXIT_FAILURE);
	// 	}
	// 	else if (result == 0)
	// 	{
	// 		pthread_mutex_lock(prog.print);
	// 		printf("Philosopher: %d dead!!!!!\n", prog.philos[i]->i);
	// 		pthread_mutex_unlock(prog.print);
	// 	}
	// 	i++;
	// }
	//
	pthread_mutex_lock(prog.print);
	printf("END\n");
	pthread_mutex_unlock(prog.print);
	free_prog(&prog);
	return (EXIT_SUCCESS);
}
//valgrind --tool=helgrind ./philo 5 800 200 200 3

// Test 1 800 200 200. The philosopher should not eat and should die.
// Test 5 800 200 200. No philosopher should die.
// Test 5 800 200 200 7. No philosopher should die and the simulation should stop when every philosopher has eaten at least 7 times.
// Test 4 410 200 200. No philosopher should die.
// Test 4 310 200 100. One philosopher should die.