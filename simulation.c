/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabidar <aabidar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 13:05:18 by aabidar           #+#    #+#             */
/*   Updated: 2024/04/25 13:05:59 by aabidar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// INITIALIZE DATA
int	init_mutexes(t_data *data)
{
	int	i;

	i = 0;
	while (i < NBR)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (perror("fork"), 1);
		i++;
	}
	if (pthread_mutex_init(&data->time_lock, NULL) != 0)
		return (perror("time_lock"), 1);
	if (pthread_mutex_init(&data->death_lock, NULL) != 0)
		return (perror("death_lock"), 1);
	if (pthread_mutex_init(&data->print_lock, NULL) != 0)
		return (perror("print_lock"), 1);
	if (NBR_OF_MEALS >= 0)
	{
		if (pthread_mutex_init(&data->meals_lock, NULL) != 0)
			return (perror("meals_lock"), 1);
		if (pthread_mutex_init(&data->finished_lock, NULL) != 0)
			return (perror("finished_lock"), 1);
		if (pthread_mutex_init(&data->end_lock, NULL) != 0)
			return (perror("end_lock"), 1);
	}
	return (0);
}

int	init_data(t_philo *philos, t_data *data)
{
	int	i;

	data->start_time = get_current_time();
	if (init_mutexes(data))
		return (1);
	data->death = 0;
	data->end = 0;
	i = 0;
	while (i < NBR)
	{
		philos[i].id = i + 1;
		philos[i].data = data;
		philos[i].nbr_meals = 0;
		philos[i].finished = 0;
		philos[i].last_meal = data->start_time;
		philos[i].r_fork = &data->forks[i];
		philos[i].l_fork = &data->forks[((i + 1) % NBR)];
		if (i == NBR - 1)
		{
			philos[i].l_fork = &data->forks[i];
			philos[i].r_fork = &data->forks[((i + 1) % NBR)];
		}
		i++;
	}
	return (0);
}

int	start_simulation(t_philo *philos, t_data *data)
{
	int	i;

	if (NBR_OF_MEALS >= 0)
		if (pthread_create(&data->mon_tid, NULL, monitor, philos))
			return (perror("monitor"), 1);
	if (pthread_create(&data->sup_tid, NULL, supervisor, philos))
		return (perror("supervisor"), 1);
	i = 0;
	while (i < NBR)
	{
		if (pthread_create(&philos[i].thread, NULL, routine, &philos[i]))
			return (perror("philo"), 1);
		i++;
	}
	return (0);
}

// END SIMULATION
int	destroy_mutexes(t_data *data)
{
	int	i;

	i = 0;
	while (i < NBR)
	{
		if (pthread_mutex_destroy(&data->forks[i]) != 0)
			return (perror("fork"), 1);
		i++;
	}
	if (pthread_mutex_destroy(&data->time_lock) != 0)
		return (perror("time_lock"), 1);
	if (pthread_mutex_destroy(&data->death_lock) != 0)
		return (perror("death_lock"), 1);
	if (pthread_mutex_destroy(&data->print_lock) != 0)
		return (perror("print_lock"), 1);
	if (NBR_OF_MEALS >= 0)
	{
		if (pthread_mutex_destroy(&data->meals_lock) != 0)
			return (perror("meals_lock"), 1);
		if (pthread_mutex_destroy(&data->finished_lock) != 0)
			return (perror("finished_lock"), 1);
		if (pthread_mutex_destroy(&data->end_lock) != 0)
			return (perror("end_lock"), 1);
	}
	return (0);
}

int	end_simulation(t_philo *philos, t_data *data)
{
	int	i;

	if (NBR_OF_MEALS >= 0)
		if (pthread_join(data->mon_tid, NULL))
			return (perror("monitor"), 1);
	if (pthread_join(data->sup_tid, NULL))
		return (perror("supervisor"), 1);
	i = 0;
	while (i < NBR)
	{
		if (pthread_join(philos[i].thread, NULL) != 0)
			return (perror("philo"), 1);
		i++;
	}
	if (destroy_mutexes(data))
		return (1);
	return (0);
}
