#include  "philo.h"

void free_prog(t_prog *prog)
{
    if (!prog)
        return;

    free_forks(prog->forks);
    prog->forks = NULL;
    free_philos(prog->philos);
    prog->philos = NULL;
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
        if (pthread_mutex_destroy(forks[i]) != 0)
            printf("pthread_mutex_destroy ERROR\n");
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
        if (pthread_mutex_destroy(philos[i]->phil) != 0)
            printf("pthread_mutex_destroy philo %d ERROR\n", i);
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

    if (result == 16)
    {
        printf("ERROR: Mutex is locked\n");
        if (pthread_mutex_unlock(mutex) != 0)
        {
            printf("ERROR: unlock mutex: %s\n");
            return ;
        }
        result = pthread_mutex_destroy(mutex);
    }
    if (result != 0)
        printf("ERROR: destroy mutex\n");
    else
        printf("Mutex destroyed\n");
}
