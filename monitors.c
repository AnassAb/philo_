/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitors.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabidar <aabidar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 13:01:58 by aabidar           #+#    #+#             */
/*   Updated: 2024/04/25 13:49:26 by aabidar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// SUPERVISOR
void	finished_eating(t_philo *philo)
{
	if (philo->data->nbr_of_meals >= 0)
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

void	*supervisor(void *data)
{
	int		i;
	t_philo	*philos;

	philos = (t_philo *)data;
	while (1)
	{
		i = 0;
		while (i < philos[i].data->philo_nbr)
		{
			if (is_ended(&philos[i]))
				return (NULL);
			if (is_dead(&philos[i]))
			{
				pthread_mutex_lock(&philos[i].data->print_lock);
				printf("%s%zu %d %s\n%s", C_DIED,
					get_timestamp(philos[i].data->start_time,
						get_current_time()), philos[i].id, "died", C_NC);
				pthread_mutex_unlock(&philos[i].data->print_lock);
				return (NULL);
			}
			finished_eating(&philos[i]);
			i++;
		}
	}
	return (NULL);
}

// MONITOR
void	*monitor(void *data)
{
	int		i;
	t_philo	*philos;

	philos = (t_philo *)data;
	while (1)
	{
		i = 0;
		while (i < philos[i].data->philo_nbr)
		{
			if (is_dead(&philos[i]) || is_ended(&philos[i]))
				return (NULL);
			pthread_mutex_lock(&philos[i].data->finished_lock);
			if (philos[i].finished == 1)
			{
				pthread_mutex_lock(&philos[i].data->end_lock);
				philos[i].data->end++;
				pthread_mutex_unlock(&philos[i].data->end_lock);
				philos[i].finished = 2;
			}
			pthread_mutex_unlock(&philos[i].data->finished_lock);
			i++;
		}
	}
	return (NULL);
}
