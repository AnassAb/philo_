/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabidar <aabidar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 12:46:24 by aabidar           #+#    #+#             */
/*   Updated: 2024/04/25 18:41:06 by aabidar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	parse_data(t_data *data, int ac, char **av)
{
	if (ac < 5 || ac > 6)
		return (printf("Error: %s\n", USAGE), 1);
	data->philo_nbr = ft_atoi(av[1]);
	if (data->philo_nbr < 1 || data->philo_nbr > 200)
		return (printf("Error: 0<nbr_philos [%d]<=200\n", data->philo_nbr), 1);
	data->time_to_die = ft_atoi(av[2]);
	if (data->time_to_die < 60)
		return (printf("Error: time_to_die [%zd]\n", data->time_to_die), 1);
	data->time_to_eat = ft_atoi(av[3]);
	if (data->time_to_eat < 60)
		return (printf("Error: time_to_eat [%zd]\n", data->time_to_eat), 1);
	data->time_to_sleep = ft_atoi(av[4]);
	if (data->time_to_sleep < 60)
		return (printf("Error: time_to_sleep [%zd]\n", data->time_to_sleep), 1);
	if (ac == 6)
	{
		data->nbr_of_meals = ft_atoi(av[5]);
		if (data->nbr_of_meals < 0)
			return (printf("Error: nbrofmeals [%d]\n", data->nbr_of_meals), 1);
	}
	else
		data->nbr_of_meals = -1;
	return (0);
}

pthread_mutex_t	*allocate_forks(t_data *data)
{
	pthread_mutex_t	*forks;

	forks = (pthread_mutex_t *)malloc(data->philo_nbr
			* sizeof(pthread_mutex_t));
	if (!forks)
		return (printf("error: fork allocation failed\n"), NULL);
	return (forks);
}

t_philo	*allocate_philos(t_data *data)
{
	t_philo	*philos;

	philos = (t_philo *)malloc(data->philo_nbr * sizeof(t_philo));
	if (!philos)
		return (printf("error: fork allocation failed\n"), NULL);
	return (philos);
}

int	main(int ac, char **av)
{
	t_data	data;
	t_philo	*philos;

	if (parse_data(&data, ac, av))
		return (1);
	data.forks = allocate_forks(&data);
	if (!data.forks)
		return (1);
	philos = allocate_philos(&data);
	if (!philos)
		return (free(data.forks), 1);
	if (init_data(philos, &data))
		return (1);
	if (start_simulation(philos, &data))
		return (1);
	if (end_simulation(philos, &data))
		return (1);
	return (0);
}
