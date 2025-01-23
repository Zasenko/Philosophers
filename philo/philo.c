/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 13:01:05 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/23 12:12:25 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	one_philo_life(t_philo *phil)
{
	if (!phil)
		return (0);
	if (print_status(phil, 1) == -1)
		return (0);
	usleep(phil->time_to_die);
	return (1);
}

int	start_infinity_circle(t_philo *philo)
{
	int	result;

	if (!philo)
		return (-1);
	while (1)
	{
		result = wait_hungry(philo);
		if (result == -1)
			return (-1);
		else if (result == 0)
			return (0);
		result = philo_circle(philo);
		if (result == -1)
			return (-1);
		else if (result == 0)
			return (0);
	}
	return (-1);
}

int	start_first_circle(t_philo *philo)
{
	int	result;

	if (!philo)
		return (-1);
	if (philo->number_of_philosophers % 2 != 0)
	{
		if (philo->i == 1)
			usleep((philo->time_to_eat / 2) * 3 * 1000);
		if (philo->i % 2 == 0)
			usleep((philo->time_to_eat / 2) * 1000);
	}
	else
	{
		if (philo->i % 2 == 0)
			usleep((philo->time_to_eat / 2) * 1000);
	}
	result = philo_circle(philo);
	if (result == -1)
		return (-1);
	else if (result == 0)
		return (0);
	return (1);
}

int	philo_life(t_philo *philo)
{
	int	result;

	if (!philo)
		return (0);
	result = start_first_circle(philo);
	if (result == -1)
		return (0);
	else if (result == 0)
		return (1);
	result = start_infinity_circle(philo);
	if (result == -1)
		return (0);
	return (1);
}

void	*create_philosopher(void *arg)
{
	t_philo	*philo;
	int		result;

	philo = (t_philo *)arg;
	if (!philo || !philo->fork1)
		return (NULL);
	result = wait_philos_creations(philo);
	if (result == -1)
		return (NULL);
	else if (result == 0)
		return (arg);
	if (philo->number_of_philosophers == 1)
	{
		if (!one_philo_life(philo))
			return (NULL);
		return (arg);
	}
	else
	{
		if (!philo_life(philo))
			return (NULL);
		return (arg);
	}
	return (NULL);
}
