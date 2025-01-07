/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 11:06:05 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/07 13:02:59 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
		printf("Result %d FINISH\n", philo->i);
		pthread_mutex_unlock(philo->print);
	}
	return (1);
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
		// pthread_mutex_lock(prog.print);
    	// printf("creating philosopher: %d\n", prog.philos[i]->i);
		// pthread_mutex_unlock(prog.print);
		if (pthread_create(&thread, NULL, create_philosopher, (void *)prog.philos[i]) != 0)
		{
			free_prog(&prog);
			perror("pthread_create");
			return (EXIT_FAILURE);
    	}
		prog.philos[i]->thread = thread;
		i++;
	}

	int	check_result = check(&prog);
	if (check_result == -1)
	{
		pthread_mutex_lock(prog.print);
		printf("check_result ERROR\n");
		pthread_mutex_unlock(prog.print);
		//return (free_prog(&prog), EXIT_FAILURE);	
	}
	if (check_result == 0)
	{
		pthread_mutex_lock(prog.print);
		printf("---------Someone dead\n");//todo delete
		pthread_mutex_unlock(prog.print);
	}
	else
	{
		pthread_mutex_lock(prog.print);
		printf("All eat\n");
		pthread_mutex_unlock(prog.print);
	}
	
	// remove!
	i = 0;
	while (prog.philos[i])
	{
		int result = wait_results(prog.philos[i]);
		if (result == -1)
		{
			free_prog(&prog);
			pthread_mutex_lock(prog.print);
			printf("ERROR wait_results:	 philosopher: %d\n", prog.philos[i]->i);
			pthread_mutex_unlock(prog.print);
			//return (EXIT_FAILURE);
		}
		else if (result == 0)
		{
			pthread_mutex_lock(prog.print);
			printf("Philosopher: %d dead!!!!!\n", prog.philos[i]->i);
			pthread_mutex_unlock(prog.print);
		}
		i++;
	}
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