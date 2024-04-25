/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabidar <aabidar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 13:00:32 by aabidar           #+#    #+#             */
/*   Updated: 2024/04/25 13:01:46 by aabidar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

size_t	get_timestamp(size_t start, size_t end)
{
	return (end - start);
}

void	ft_sleep(size_t time_ms)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < time_ms)
		usleep(time_ms / 10);
}

int	message(t_philo *philo, char *state, char *color)
{
	if (check_death(philo))
		return (1);
	pthread_mutex_lock(&philo->data->print_lock);
	printf("%s%zu %d %s\n%s", color, get_timestamp(philo->data->start_time,
			get_current_time()), philo->id, state, C_NC);
	pthread_mutex_unlock(&philo->data->print_lock);
	return (0);
}

int	case_one(t_philo *philo)
{
	pthread_mutex_lock(philo->r_fork);
	if (message(philo, "has taken a fork", ""))
		return (pthread_mutex_unlock(philo->r_fork), 1);
	while (1)
	{
		if (check_death(philo))
			return (pthread_mutex_unlock(philo->r_fork), 1);
	}
	pthread_mutex_unlock(philo->r_fork);
	return (0);
}
