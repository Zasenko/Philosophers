/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 13:29:27 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/06 14:05:50 by dzasenko         ###   ########.fr       */
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
        if (pthread_detach(philos[i]->thread) != 0)
        {
            perror("Не удалось отсоединить поток");
            return;
        }
    

        // if (philos[i]->fork1)
        // {
        //     destroy_mutex(philos[i]->fork1);
        //     free(philos[i]->fork1);
        //     philos[i]->fork1 = NULL;
        // }

        // if (philos[i]->fork2)
        // {
        //     destroy_mutex(philos[i]->fork2);
        //     free(philos[i]->fork2);
        //     philos[i]->fork2 = NULL;
        // }

        // if (philos[i]->print)
        // {
        //     printf("free print\n");
        //     destroy_mutex(philos[i]->print);
        //     free(philos[i]->print);
        //     philos[i]->print = NULL;
        // }
        printf("free philos %d\n", i);
        free(philos[i]);
        philos[i] = NULL;
        i++;
    }
    free(philos);
}

//TODO delete
void free_phil(t_philo *phil)
{

    if (!phil)
        return ;

    if (phil->phil)
    {
        destroy_mutex(phil->phil);
        free(phil->phil);
    }
    if (phil->fork1)
    {
        destroy_mutex(phil->fork1);
        free(phil->fork1);
        phil->fork1 = NULL;
    }
    if (phil->fork2)
    {
        destroy_mutex(phil->fork2);
        free(phil->fork2);
        phil->fork2 = NULL;
    }
    if (phil->print)
    {
        printf("free print\n");
        destroy_mutex(phil->print);
        free(phil->print);
        phil->print = NULL;
    }
    printf("free philos %d\n", phil->i);
    free(phil);
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
