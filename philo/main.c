/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 11:06:05 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/23 12:12:07 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	wait_result(t_philo *philo)
{
	void	*result;

	if (!philo)
		return (0);
	if (pthread_join(philo->thread, &result) != 0)
		return (0);
	if (result == NULL)
		return (0);
	return (1);
}

int	wait_finishing(t_prog *prog)
{
	int	i;
	int	result;
	int	flag;

	if (!prog)
		return (0);
	flag = 1;
	i = 0;
	while (prog->philos[i])
	{
		result = wait_result(prog->philos[i]);
		if (!result)
			flag = 0;
		i++;
	}
	return (flag);
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

//valgrind --tool=helgrind ./philo 5 800 200 200 3