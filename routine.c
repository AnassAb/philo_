/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabidar <aabidar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 12:59:03 by aabidar           #+#    #+#             */
/*   Updated: 2024/04/25 13:00:00 by aabidar          ###   ########.fr       */
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
	ft_sleep(TIME_TO_EAT);
	if (NBR_OF_MEALS >= 0)
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
	ft_sleep(TIME_TO_SLEEP);
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
	if ((philo->id % 2))
		ft_sleep(60);
	if (_think(philo))
		return (NULL);
	if (NBR == 1 && case_one(philo))
		return (NULL);
	while (!is_finished(philo))
	{
		if (_eat(philo))
			break ;
		if (_sleep(philo))
			break ;
		if (_think(philo))
			break ;
		ft_sleep((TIME_TO_DIE - (get_current_time() - philo->last_meal)) / 2);
	}
	return (NULL);
}
