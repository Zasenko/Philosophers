/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 13:29:27 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/16 10:01:51 by dzasenko         ###   ########.fr       */
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
        destroy_mutex(prog->print);
        free(prog->print);
        prog->print = NULL;
    }
    if (prog->all_philos_created_mutex)
    {
        destroy_mutex(prog->all_philos_created_mutex);
        free(prog->all_philos_created_mutex);
        prog->all_philos_created_mutex = NULL;
    }
    if (prog->all_philos_created)
    {
        free(prog->all_philos_created);
        prog->all_philos_created = NULL;
    }
    if (prog->is_dead_mutex)
    {
        destroy_mutex(prog->is_dead_mutex);
        free(prog->is_dead_mutex);
        prog->is_dead_mutex = NULL;
    }
    if (prog->is_dead)
    {
        free(prog->is_dead);
        prog->is_dead = NULL;
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
        return;
    i = 0;
    while (philos[i])
    {
        if (philos[i]->must_eat_times_mutex)
        {
            destroy_mutex(philos[i]->must_eat_times_mutex);
            free(philos[i]->must_eat_times_mutex);
            philos[i]->must_eat_times_mutex = NULL;
        }
        if (philos[i]->time_mutex)
        {
            destroy_mutex(philos[i]->time_mutex);
            free(philos[i]->time_mutex);
            philos[i]->time_mutex = NULL;
        }
        free(philos[i]);
        philos[i] = NULL;
        i++;
    }
    free(philos);
}

void destroy_mutex(pthread_mutex_t *mutex)
{
    if (!mutex)
        return;
    int result = pthread_mutex_destroy(mutex);

    if (result == EBUSY)
    {
        printf("ERROR: Mutex is locked. UNLOCKING\n");
        if (pthread_mutex_unlock(mutex) != 0)
        {
            printf("ERROR: unlock mutex\n");
            return;
        }
        result = pthread_mutex_destroy(mutex);
    }
    if (result != 0)
        printf("ERROR: destroy mutex\n");
    // else
    // printf("Mutex destroyed\n");
}
