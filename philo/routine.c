/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabidar <aabidar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 12:59:03 by aabidar           #+#    #+#             */
/*   Updated: 2024/04/25 18:42:38 by aabidar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->r_fork);
	if (message(philo, "has taken a fork", ""))
		return (pthread_mutex_unlock(philo->r_fork), 1);
	pthread_mutex_lock(philo->l_fork);
	if (message(philo, "has taken a fork", ""))
	{
		pthread_mutex_unlock(philo->r_fork);
		return (pthread_mutex_unlock(philo->l_fork), 1);
	}
	return (0);
}

int	_eat(t_philo *philo)
{
	if (take_forks(philo))
		return (1);
	if (message(philo, "is eating", C_EAT))
	{
		pthread_mutex_unlock(philo->r_fork);
		return (pthread_mutex_unlock(philo->l_fork), 1);
	}
	pthread_mutex_lock(&philo->data->time_lock);
	philo->last_meal = get_current_time();
	pthread_mutex_unlock(&philo->data->time_lock);
	ft_sleep(philo->data->time_to_eat);
	if (philo->data->nbr_of_meals > -1)
	{
		pthread_mutex_lock(&philo->data->meals_lock);
		philo->nbr_meals++;
		pthread_mutex_unlock(&philo->data->meals_lock);
		if (check_end(philo))
		{
			pthread_mutex_unlock(philo->r_fork);
			return (pthread_mutex_unlock(philo->l_fork), 1);
		}
	}
	pthread_mutex_unlock(philo->r_fork);
	return (pthread_mutex_unlock(philo->l_fork), 0);
}

int	_sleep(t_philo *philo)
{
	if (message(philo, "is sleeping", C_SLEEP))
		return (1);
	ft_sleep(philo->data->time_to_sleep);
	return (0);
}

int	_think(t_philo *philo)
{
	if (message(philo, "is thinking", C_THINK))
		return (1);
	return (0);
}

void	*routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (_think(philo))
		return (NULL);
	if (philo->data->philo_nbr == 1 && case_one(philo))
		return (NULL);
	while (1)
	{
		if (philo->data->nbr_of_meals > -1 && is_finished(philo))
			break ;
		if (_eat(philo))
			break ;
		if (_sleep(philo))
			break ;
		if (_think(philo))
			break ;
		ft_sleep((philo->data->time_to_die - (get_current_time()
					- philo->last_meal)) / 2);
	}
	return (NULL);
}
