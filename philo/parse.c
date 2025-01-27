/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 12:19:47 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/27 11:25:55 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	parse_arg(t_prog *prog, int argc, char **argv)
{
	if (!prog || !argv)
		return (0);
	if (argc < 5 || argc > 6)
		return (printf("Error: Wrong arguments count\n"), 0);
	prog->number_of_philosophers = make_number(argv[1]);
	if (!prog->number_of_philosophers)
		return (0);
	prog->time_to_die = make_number(argv[2]);
	if (!prog->time_to_die)
		return (0);
	prog->time_to_eat = make_number(argv[3]);
	if (!prog->time_to_eat)
		return (0);
	prog->time_to_sleep = make_number(argv[4]);
	if (!prog->time_to_sleep)
		return (0);
	if (argc == 6)
	{
		prog->must_eat_times = make_number(argv[5]);
		if (!prog->must_eat_times)
			return (0);
	}
	return (1);
}

int	parse(t_prog *prog, int argc, char **argv)
{
	if (!prog || !argv)
		return (0);
	if (!parse_arg(prog, argc, argv))
		return (0);
	prog->forks = init_forks(prog->number_of_philosophers);
	if (!prog->forks)
		return (0);
	prog->philos = init_philosophers(prog);
	if (!prog->philos)
		return (0);
	return (1);
}
