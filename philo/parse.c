/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzasenko <dzasenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 12:19:47 by dzasenko          #+#    #+#             */
/*   Updated: 2025/01/22 13:53:43 by dzasenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int ft_strlen(char *s)
{
	int i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

static int	ft_trim(char c, char const *set)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	src_size;
	size_t	i;

	i = 0;
	src_size = ft_strlen(src);
	if (size > 0)
	{
		while (src[i] != '\0' && i < size - 1)
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (src_size);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub_s;
	size_t	s_len;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
	{
		sub_s = (char *)malloc(1 * sizeof(char));
		if (sub_s == NULL)
			return (NULL);
		sub_s[0] = '\0';
		return (sub_s);
	}
	if (len > s_len - start)
		len = s_len - start;
	sub_s = (char *)malloc((len + 1) * sizeof(char));
	if (sub_s == NULL)
		return (NULL);
	ft_strlcpy(sub_s, s + start, len + 1);
	return (sub_s);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*new_s;
	size_t	s_l;
	size_t	e_l;
	size_t	len;
	size_t	i;

	s_l = 0;
	e_l = 0;
	len = ft_strlen(s1);
	i = 0;
	while (s1[i] && ft_trim(s1[i], set) == 1)
	{
		s_l++;
		i++;
	}
	i = len - 1;
	while (s1[i] && ft_trim(s1[i], set) == 1)
	{
		e_l++;
		i--;
	}
	new_s = ft_substr(s1, s_l, len - e_l - s_l);
	if (new_s == NULL)
		return (NULL);
	return (new_s);
}

pthread_mutex_t	**init_forks(int num)
{
	pthread_mutex_t	**forks;
	int				i;

	forks = (pthread_mutex_t **)malloc(sizeof(pthread_mutex_t *) * (num + 1));
	if (!forks)
		return (NULL);
	i = 0;
	while (i <= num)
	{
		forks[i] = NULL;
		i++;
	}
	i = 0;
	while (i < num)
	{
		forks[i] = init_mutex();
		if (forks[i] == NULL)
			return (free_forks(forks), NULL);
		i++;
	}
	return (forks);
}

int	give_forks(t_prog *prog, t_philo *phil, int i)
{
	if (!prog || !phil)
		return (0);
	if (prog->number_of_philosophers == 1)
	{
		phil->fork1 = prog->forks[i];
		phil->fork2 = NULL;
	}
	else
	{
		if (phil->i % 2 == 0)
		{
			phil->fork1 = prog->forks[i - 1];
			phil->fork2 = prog->forks[i];
		}
		else
		{
			phil->fork1 = prog->forks[i];
			if (i == 0)
				phil->fork2 = prog->forks[prog->number_of_philosophers - 1];
			else
				phil->fork2 = prog->forks[i - 1];
		}
	}
	return (1);
}

t_philo	*init_philos(t_prog *prog, int i)
{
	t_philo	*phil;

	if (!prog || !prog->forks)
		return (NULL);
	phil = malloc(sizeof(t_philo));
	if (phil == NULL)
		return (printf("malloc error\n"), NULL);
	phil->i = i + 1;
	phil->must_eat_times = prog->must_eat_times;
	phil->time_to_die = prog->time_to_die;
	phil->time_to_eat = prog->time_to_eat;
	phil->time_to_sleep = prog->time_to_sleep;
	phil->number_of_philosophers = prog->number_of_philosophers;
	phil->is_dead_mutex = prog->is_dead_mutex;
	phil->is_dead = prog->is_dead;
	phil->print = prog->print;
	phil->all_philos_created = prog->all_philos_created;
	phil->all_philos_created_mutex = prog->all_philos_created_mutex;
	phil->must_eat_times_mutex = init_mutex();
	phil->time_mutex = init_mutex();
	if (!phil->time_mutex || !phil->must_eat_times_mutex || !give_forks(prog, phil, i))
		return (free_phil(phil), NULL);
	return (phil);
}

t_philo	**create_philos(t_prog *prog)
{
	t_philo	**philos;
	int		i;

	philos = malloc(sizeof(t_philo *) * (prog->number_of_philosophers + 1));
	if (!philos)
		return (printf("Malloc error\n"), NULL);
	i = 0;
	while (i <= prog->number_of_philosophers)
	{
		philos[i] = NULL;
		i++;
	}
	i = 0;
	while (i < prog->number_of_philosophers)
	{
		philos[i] = init_philos(prog, i);
		if (!philos[i])
			return (free_philos(philos), NULL);
		i++;
	}
	return (philos);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int check_simbols(char *str)
{
	int i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (ft_isdigit(str[i]))
			i++;
		else
		{
			return (0);
		}
	}
	return (1);
}

int	ft_atoi(const char *nptr)
{
	int		i;
	long		sing;
	long	result;

	i = 0;
	sing = +1;
	result = 0;
	while (nptr[i] == ' ' || (nptr[i] >= '\t' && nptr[i] <= '\r'))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sing *= -1;
		i++;
	}
	while (nptr[i] != '\0' && (nptr[i] >= '0' && nptr[i] <= '9'))
	{
		result = result * 10 + (nptr[i] - '0');
		i++;
	}
	if ((result * sing) < -2147483648)
	{
		printf("Error: Argument is too small number\n");
		return (-1);
	}
	else if ((result * sing) > 2147483647)
	{
		//error
		printf("Error: Argument is too big number\n");
		return (-1);
	}
	return ((int)(result * sing));
}

int make_number(char *str)
{
	if (!str)
		return (-1);
	char *trim_str = ft_strtrim(str, " \t");//malloc
	if (!trim_str)
		return (-1);
	if (!ft_strlen(trim_str))
	{
		printf("Error: empty argument\n");
		//free trim_str
		return (-1);
	}
	if (!check_simbols(trim_str))
	{
		printf("Error: Argument has a whong simbol\n");
		//free trim_str
		return (-1);
	}
	if (ft_strlen(trim_str) > 11)
	{
		//free trim_str
		printf("Error: Argument is not INT\n");
		return (-1);
	}
	
	int num = ft_atoi(trim_str);
	//free trim_str
	if (num < 1)
	{
		printf("Error: Argument has a whong simbol\n");

	}
	return num;
	//free trim_str
	//TODO TRIM each arg
	//check +-0123456789
	//chech INT MIN INT MAX
	//check > 0
}

int parse(t_prog *prog, int argc, char **argv)
{
	if (!prog || !argv)
		return (0);
	if (argc < 5 || argc > 6)
		return (printf("Error: Wrong arguments count\n"), 0);

	// TODO TRIM each arg
	// check +-0123456789
	// chech INT MIN INT MAX
	//check > 0
	int number_of_philosophers = atoi(argv[1]); // todo atoi
	if (number_of_philosophers < 1)
		return (printf("Wrong philosophers count. It should be > 0\n"), 0);
	prog->number_of_philosophers = number_of_philosophers;
	prog->time_to_die = atoi(argv[2]);	 // todo atoi
	prog->time_to_eat = atoi(argv[3]);	 // todo atoi
	prog->time_to_sleep = atoi(argv[4]); // todo atoi
	if (argc == 6)
	{
		prog->must_eat_times = atoi(argv[5]); // todo atoi
		if (prog->must_eat_times < 1)
			return (printf("number of times each philosopher must eat should be > 0\n"), 0);
	}
	prog->forks = init_forks(number_of_philosophers);
	if (!prog->forks)
		return (0);
	prog->philos = create_philos(prog);
	if (!prog->philos)
		return (0);
	return (1);
}
