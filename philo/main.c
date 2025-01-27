/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 11:06:05 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/27 12:02:29 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_prog	prog;
	int		result;

	prog = new_prog();
	if (!init_prog(&prog))
		return (free_prog(&prog), EXIT_FAILURE);
	if (!parse(&prog, argc, argv))
		return (free_prog(&prog), EXIT_FAILURE);
	if (!create_threads(&prog))
	{
		make_philos_dead(&prog);
		wait_finishing(&prog);
		return (free_prog(&prog), EXIT_FAILURE);
	}
	result = check(&prog);
	if (result == -1)
	{
		make_philos_dead(&prog);
		wait_finishing(&prog);
		return (free_prog(&prog), EXIT_FAILURE);
	}
	result = wait_finishing(&prog);
	if (!result)
		return (free_prog(&prog), EXIT_FAILURE);
	return (free_prog(&prog), EXIT_SUCCESS);
}

t_prog	new_prog(void)
{
	t_prog	prog;

	prog.number_of_philosophers = 0;
	prog.time_to_die = 0;
	prog.must_eat_times = -1;
	prog.time_to_eat = 0;
	prog.time_to_sleep = 0;
	prog.philos = NULL;
	prog.forks = NULL;
	prog.start_time = 0;
	prog.all_philos_created = NULL;
	prog.all_philos_created_mutex = NULL;
	prog.is_dead_mutex = NULL;
	prog.print = NULL;
	prog.is_dead = NULL;
	return (prog);
}

int	create_thread(t_prog *prog, t_philo *phil)
{
	pthread_t	thread;

	if (!prog || !phil)
		return (0);
	phil->time = prog->start_time;
	phil->start_time = prog->start_time;
	if (pthread_create(&thread, NULL, create_philosopher, (void *)phil) != 0)
		return (0);
	phil->is_thread_created = 1;
	phil->thread = thread;
	return (1);
}

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
