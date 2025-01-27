/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 11:06:24 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/27 11:29:36 by dzasenko         ###   ########.fr       */
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
	int				i;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_times;
	int				number_of_philosophers;
	long			start_time;
	long			time;
	int				*is_dead;
	int				*all_philos_created;
	pthread_t		thread;
	int				is_thread_created;
	pthread_mutex_t	*fork1;
	pthread_mutex_t	*fork2;
	pthread_mutex_t	*print;
	pthread_mutex_t	*must_eat_times_mutex;
	pthread_mutex_t	*time_mutex;
	pthread_mutex_t	*is_dead_mutex;
	pthread_mutex_t	*all_philos_created_mutex;
}	t_philo;

typedef struct s_prog
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_times;
	long			start_time;
	t_philo			**philos;
	int				*all_philos_created;
	int				*is_dead;
	pthread_mutex_t	**forks;
	pthread_mutex_t	*print;
	pthread_mutex_t	*all_philos_created_mutex;
	pthread_mutex_t	*is_dead_mutex;
}	t_prog;

t_prog			new_prog(void);
int				init_prog(t_prog *prog);
pthread_mutex_t	*init_mutex(void);
pthread_mutex_t	**init_forks(int num);
t_philo			**init_philosophers(t_prog *prog);
int				parse(t_prog *prog, int argc, char **argv);
int				philo_circle(t_philo *philo);
void			free_philos(t_philo **philos);
void			free_phil(t_philo *phil);
void			free_forks(pthread_mutex_t **forks);
void			free_prog(t_prog *prog);
void			free_str(char *s);
void			destroy_mutex(pthread_mutex_t *mutex);
int				check(t_prog *prog);
int				check_if_dead(t_philo *philo);
void			*create_philosopher(void *arg);
long			get_time(void);
int				ft_sleep(int sleep_time, t_philo *phil);
int				print_status(t_philo *phil, int status);
int				wait_philos_creations(t_philo *philo);
int				wait_hungry(t_philo *philo);
int				wait_result(t_philo *philo);
int				wait_finishing(t_prog *prog);
int				take_forks(t_philo *philo);
int				unlock_forks(t_philo *philo);
int				update_time(t_philo *philo);
int				make_philos_dead(t_prog *prog);
int				ft_strlen(const char *s);
int				ft_isdigit(int c);
int				check_simbols(char *str);
int				ft_trim(char c, char const *set);
size_t			ft_strlcpy(char *dst, const char *src, size_t size);
char			*ft_substr(char const *s, unsigned int start, size_t len);
char			*ft_strtrim(char const *s1, char const *set);
void			free_prog_mutexes(t_prog *prog);
int				ft_atoi(const char *nptr);
int				make_number(char *str);
int				create_thread(t_prog *prog, t_philo *phil);
int				create_threads(t_prog *prog);
#endif
