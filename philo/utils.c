/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 13:01:21 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/22 11:11:44 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	print_status(t_philo *phil, int status)
{
	long	time;
	int		death_res;

	if (!phil || !phil->print)
		return (-1);
	pthread_mutex_lock(phil->print);
	death_res = check_if_dead(phil);
	if (death_res == -1)
		return (pthread_mutex_unlock(phil->print), -1);
	else if (death_res)
		return (pthread_mutex_unlock(phil->print), 0);
	time = get_time();
	if (!time)
		return (pthread_mutex_unlock(phil->print), -1);
	if (status == 1)
		printf("%ld %d has taken a fork\n", time - phil->start_time, phil->i);
	else if (status == 2)
		printf("%ld %d is eating\n", time - phil->start_time, phil->i);
	else if (status == 3)
		printf("%ld %d is sleeping\n", time - phil->start_time, phil->i);
	else if (status == 4)
		printf("%ld %d is thinking\n", time - phil->start_time, phil->i);
	else
		return (pthread_mutex_unlock(phil->print), -1);
	return (pthread_mutex_unlock(phil->print), 1);
}
