/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabidar <aabidar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 12:57:25 by aabidar           #+#    #+#             */
/*   Updated: 2024/04/25 13:47:35 by aabidar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->death_lock);
	if (philo->data->death == 1)
	{
		pthread_mutex_unlock(&philo->data->death_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->death_lock);
	return (0);
}

int	check_end(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->end_lock);
	if (philo->data->end == philo->data->philo_nbr)
	{
		pthread_mutex_unlock(&philo->data->end_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->end_lock);
	return (0);
}

int	is_finished(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->finished_lock);
	if (philo->finished)
	{
		pthread_mutex_unlock(&philo->data->finished_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->finished_lock);
	return (0);
}

int	is_ended(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->end_lock);
	if (philo->data->end == philo->data->philo_nbr)
	{
		pthread_mutex_unlock(&philo->data->end_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->end_lock);
	return (0);
}

int	is_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->time_lock);
	if ((get_current_time() - philo->last_meal) > philo->data->time_to_die)
	{
		pthread_mutex_lock(&philo->data->death_lock);
		philo->data->death = 1;
		pthread_mutex_unlock(&philo->data->death_lock);
		pthread_mutex_unlock(&philo->data->time_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->time_lock);
	return (0);
}
