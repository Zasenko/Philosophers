/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 11:06:24 by dzasenko          #+#    #+#             */
/*   Updated: 2024/12/30 14:56:18 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <signal.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_philo
{
	int	i;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	must_eat_times;
	pthread_mutex_t *fork1;
	pthread_mutex_t *fork2;
	pthread_t thread;
	long time;
} t_philo;

typedef struct s_prog
{
	int 	number_of_philosophers;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		must_eat_times;
	t_philo	**philos;
	pthread_mutex_t **forks;
} t_prog;

int		parse(t_prog *prog, int argc, char **argv);

#endif