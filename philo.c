/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabidar <aabidar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 12:46:24 by aabidar           #+#    #+#             */
/*   Updated: 2024/04/25 13:07:31 by aabidar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(void)
{
	t_data	data;
	t_philo	philos[NBR];

	if (init_data(philos, &data))
		return (1);
	if (start_simulation(philos, &data))
		return (1);
	if (end_simulation(philos, &data))
		return (1);
	return (0);
}
