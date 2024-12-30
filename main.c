/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 11:06:05 by dzasenko          #+#    #+#             */
/*   Updated: 2024/12/30 12:19:25 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_prog prog;
	
	
	if (argc < 5 || argc  > 6)
	{
		write(1, "Wrong argumnts count\n", 21);
		return (EXIT_FAILURE);
	}
	
	// The number of philosophers and also the number of forks.
	int number_of_philosophers = atoi(argv[1]);	//todo atoi
	if (number_of_philosophers < 1)
	{
		write(1, "Wrong philosophers count\n", 21);
		return (EXIT_FAILURE);
	}

	// time_to_die
	// If a philosopher didn’t start eating time_to_die milliseconds since
	// the beginning of their last meal or the beginning of the simulation, they die.
	prog.time_to_die = atoi(argv[2]);//todo atoi

	//time_to_eat
	prog.time_to_eat = atoi(argv[3]);//todo atoi

	//time_to_sleep
	prog.time_to_sleep = atoi(argv[4]);//todo atoi
	
	//[number_of_times_each_philosopher_must_eat]
	//TODO!!! ?? why array?
	prog.number_of_times_each_philosopher_must_eat = -1;
	if (argc == 6)
	{
		prog.number_of_times_each_philosopher_must_eat = atoi(argv[5]);//todo atoi		
	}

	return (EXIT_SUCCESS);
}


// Test 1 800 200 200. The philosopher should not eat and should die.
// Test 5 800 200 200. No philosopher should die.
// Test 5 800 200 200 7. No philosopher should die and the simulation should stop when every philosopher has eaten at least 7 times.
// Test 4 410 200 200. No philosopher should die.
// Test 4 310 200 100. One philosopher should die.