/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 11:06:24 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/15 11:06:18 by dzasenko         ###   ########.fr       */
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
	int time_to_die;   // in milliseconds
	int time_to_eat;   // in milliseconds
	int time_to_sleep; // in milliseconds
	int	must_eat_times;
	pthread_mutex_t *must_eat_times_mutex;
	int 	number_of_philosophers;//todo delete
	pthread_mutex_t *fork1;
	pthread_mutex_t *fork2;
	pthread_mutex_t	*print;
	pthread_t thread;
	long time;
	pthread_mutex_t *time_mutex;
	int	is_dead;
	pthread_mutex_t *is_dead_mutex;

	int	all_philos_created;
	pthread_mutex_t *all_philos_created_mutex;
	long start_time;
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
	pthread_mutex_t	*print;
	pthread_mutex_t *all_philos_created_mutex;
	long start_time;
} t_prog;

int		init_prog(t_prog *prog);
int		parse(t_prog *prog, int argc, char **argv);
int		philo_circle(t_philo *philo);
long	get_time();
void	free_philos(t_philo **philos);
void	free_forks(pthread_mutex_t **forks);
void	free_prog(t_prog *prog);
void	destroy_mutex(pthread_mutex_t *mutex);
int		check(t_prog *prog);
int		check_if_dead(t_philo *philo);
void	*create_philosopher(void *arg);
long	get_time();

#endif
