/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 13:29:27 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/08 11:05:03 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "errno.h"

void free_prog(t_prog *prog)
{
    if (!prog)
        return ;
    free_forks(prog->forks);
    prog->forks = NULL;
    free_philos(prog->philos);
    prog->philos = NULL;
    if (prog->print)
    {
        printf("destroy_mutex prog->print\n");
        destroy_mutex(prog->print);
        free(prog->print);
        prog->print = NULL;
    }
}

void free_forks(pthread_mutex_t **forks)
{
    int i;

    if (!forks)
        return;
    i = 0;
    while (forks[i])
    {
        printf("destroy_mutex fork %d\n", i);
        destroy_mutex(forks[i]);
        free(forks[i]);
        forks[i] = NULL;
        i++;
    }
    free(forks);
}

void free_philos(t_philo **philos)
{
    int i;

    if (!philos)
        return ;
    i = 0;
    while (philos[i])
    {
        if (philos[i]->must_eat_times_mutex)
        {
            printf("destroy_mutex phil %d -> must_eat_times_mutex\n", philos[i]->i);
            destroy_mutex(philos[i]->must_eat_times_mutex);
            free(philos[i]->must_eat_times_mutex);
            philos[i]->must_eat_times_mutex = NULL;
        }
        if (philos[i]->time_mutex)
        {
            printf("destroy_mutex phil %d -> time_mutex\n", philos[i]->i);
            destroy_mutex(philos[i]->time_mutex);
            free(philos[i]->time_mutex);
            philos[i]->time_mutex = NULL;
        }
        if (philos[i]->is_dead_mutex)
        {
            printf("destroy_mutex phil %d -> is_dead_mutex\n", philos[i]->i);
            destroy_mutex(philos[i]->is_dead_mutex);
            free(philos[i]->is_dead_mutex);
            philos[i]->is_dead_mutex = NULL;
        }
        printf("free philos %d\n", i);
        free(philos[i]);
        philos[i] = NULL;
        i++;
    }
    free(philos);
}

void destroy_mutex(pthread_mutex_t *mutex)
{
    if (!mutex)
        return ;
    int result = pthread_mutex_destroy(mutex);

    if (result == EBUSY)
    {
        printf("ERROR: Mutex is locked. UNLOCKING\n");
        if (pthread_mutex_unlock(mutex) != 0)
        {
            printf("ERROR: unlock mutex\n");
            return ;
        }
        result = pthread_mutex_destroy(mutex);
    }
    if (result != 0)
        printf("ERROR: destroy mutex\n");
    else
        printf("Mutex destroyed\n");
}
