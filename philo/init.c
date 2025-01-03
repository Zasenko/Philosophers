#include "philo.h"

int init_prog(t_prog *prog)
{
    if (!prog)
        return (0);
    prog->number_of_philosophers = 0;
    prog->time_to_die = 0;
    prog->must_eat_times = -1;
    prog->time_to_eat = 0;
    prog->time_to_sleep = 0;
    prog->philos = NULL;
    prog->forks = NULL;
    return (1);
}