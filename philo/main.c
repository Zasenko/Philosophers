/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 11:06:05 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/21 13:07:29 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	wait_results(t_philo *philo)
{
	void	*result;

	if (!philo)
		return (-1);
	if (pthread_join(philo->thread, &result) != 0)
		return (-1);
	if (result == NULL)
		return (-1);
	return (1);
}

//+
int	create_thread(t_prog *prog, t_philo *phil)
{
	pthread_t	thread;

	if (!prog || !phil)
		return (0);
	phil->time = prog->start_time;
	phil->start_time = prog->start_time;
	if (pthread_create(&thread, NULL, create_philosopher, (void *)phil) != 0)
		return (0);
	phil->thread = thread;
	return (1);
}

//+
int	create_threads(t_prog *prog)
{
	int	i;

	if (!prog)
		return (0);
	prog->start_time = get_time();
	if (!prog->start_time)
		return (0);
	i = 0;
	while (prog->philos[i])
	{
		if (!create_thread(prog, prog->philos[i]))
			return (0);
		i++;
	}
	pthread_mutex_lock(prog->all_philos_created_mutex);
	*prog->all_philos_created = 1;
	pthread_mutex_unlock(prog->all_philos_created_mutex);
	return (1);
}

int	main(int argc, char **argv)
{
	t_prog	prog;

	prog = new_prog();
	if (!init_prog(&prog))
		return (free_prog(&prog), EXIT_FAILURE);
	if (!parse(&prog, argc, argv))
		return (free_prog(&prog), EXIT_FAILURE);
	if (!create_threads(&prog))
		return (free_prog(&prog), EXIT_FAILURE);

	int	check_result = check(&prog);
	if (check_result == -1)
	{
		//todo should i kill them?
		return (free_prog(&prog), EXIT_FAILURE);
	}
	int i = 0;
	while (prog.philos[i])
	{
		int result = wait_results(prog.philos[i]);
		if (result == -1)
		{
			free_prog(&prog);
			return (EXIT_FAILURE);
		}
		i++;
	}
	free_prog(&prog);
	return (EXIT_SUCCESS);
}

//valgrind --tool=helgrind ./philo 5 800 200 200 3

// ---die---

// 1 800 200 100 10 +
// 1 800 200 200 10 +
// 3 599 200 200 10 +
// 31 599 200 200 10 +
// 131 596 200 200 10 +

// 4 310 200 100 10 +

// ---not die---

// 5 601 200 200 10 ++
// 5 610 200 100 10 -
// 5 610 200 200 10 ++
// 5 800 200 200 7 +
// 5 800 200 200 10 +-
// 31 601 200 200 10
// 31 605 200 200 10 -
// 31 610 200 100 10 -
// 31 610 200 200 10 +
// 131 601 200 200 10
// 131 605 200 200 10
// 131 610 200 100 10 -
// 131 610 200 200 10
// 199 601 200 200 10 +
// 199 605 200 200 10 +
// 199 610 200 100 10 -
// 199 610 200 200 10 ++

// 4 410 200 100 10 ++
// 4 410 200 200 10 ++
// 50 401 200 200 10 -
// 50 405 200 200 10 +
// 50 410 200 200 10 +
// 50 410 200 100 10 +
// 130 401 200 200 10 -
// 130 405 200 100 10 -
// 130 410 200 100 10 --
// 130 410 200 200 10 -
// 198 410 200 100 10 ++
// 198 405 200 200 10 ++
// 198 610 200 200 10 ++
// 198 800 200 200 10 --