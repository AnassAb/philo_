/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitors.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabidar <aabidar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 13:01:58 by aabidar           #+#    #+#             */
/*   Updated: 2024/04/25 18:49:45 by aabidar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// SUPERVISOR
void	finished_eating(t_philo *philo)
{
	if (philo->data->nbr_of_meals > -1)
	{
		pthread_mutex_lock(&philo->data->meals_lock);
		if (philo->nbr_meals == philo->data->nbr_of_meals)
		{
			pthread_mutex_lock(&philo->data->finished_lock);
			philo->finished = 1;
			pthread_mutex_unlock(&philo->data->finished_lock);
		}
		pthread_mutex_unlock(&philo->data->meals_lock);
	}
}

void	print_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->print_lock);
	printf("%s%zu %d %s\n%s", C_DIED,
		get_timestamp(philo->data->start_time,
			get_current_time()), philo->id, "died", C_NC);
	pthread_mutex_unlock(&philo->data->print_lock);
}

void	*supervisor(void *data)
{
	int		i;
	int		nbr;
	t_philo	*philos;

	philos = (t_philo *)data;
	nbr = philos[0].data->philo_nbr;
	while (1)
	{
		i = 0;
		while (i < nbr)
		{
			if (philos[i].data->nbr_of_meals > -1 && is_ended(&philos[i]))
				return (NULL);
			if (is_dead(&philos[i]))
				return (print_death(&philos[i]), NULL);
			finished_eating(&philos[i]);
			i++;
		}
	}
	return (NULL);
}

// MONITOR
void	calc_end(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->finished_lock);
	if (philo->finished == 1)
	{
		pthread_mutex_lock(&philo->data->end_lock);
		philo->data->end++;
		pthread_mutex_unlock(&philo->data->end_lock);
		philo->finished = 2;
	}
	pthread_mutex_unlock(&philo->data->finished_lock);
}

void	*monitor(void *data)
{
	int		i;
	int		nbr;
	t_philo	*philos;

	philos = (t_philo *)data;
	nbr = philos[0].data->philo_nbr;
	while (1)
	{
		i = 0;
		while (i < nbr)
		{
			if (is_dead(&philos[i]) || is_ended(&philos[i]))
				return (NULL);
			calc_end(&philos[i]);
			i++;
		}
	}
	return (NULL);
}
