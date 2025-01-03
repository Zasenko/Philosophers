/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 13:29:27 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/03 13:29:30 by dzasenko         ###   ########.fr       */
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
}

void free_forks(pthread_mutex_t **forks)
{
    int i;

    if (!forks)
        return;
    i = 0;
    while (forks[i])
    {
        printf("free fork %d\n", i);
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
        if (philos[i]->phil)
        {
            destroy_mutex(philos[i]->phil);
            free(philos[i]->phil);
        }

        if (philos[i]->fork1)
        {
            destroy_mutex(philos[i]->fork1);
            free(philos[i]->fork1);
        }

        if (philos[i]->fork2)
        {
            destroy_mutex(philos[i]->fork2);
            free(philos[i]->fork2);
        }

        if (philos[i]->print)
        {
            destroy_mutex(philos[i]->print);
            free(philos[i]->print);
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
        printf("ERROR: Mutex is locked\n");
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
