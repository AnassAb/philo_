/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabidar <aabidar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 13:06:21 by aabidar           #+#    #+#             */
/*   Updated: 2024/04/25 14:05:32 by aabidar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>
# include <unistd.h>

# define C_EAT "\033[0;32m"   // EAT
# define C_DIED "\033[0;31m"  // DIED
# define C_THINK "\033[0;33m" // THINK
# define C_SLEEP "\033[0;34m" // SLEEP
# define C_NC "\033[0m"

# define USAGE "./philo nbr_of_philos time_to[die eat sleep] [nbr_of_meals]"

typedef struct s_data
{
	int				philo_nbr;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nbr_of_meals;
	size_t			start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	time_lock;
	pthread_mutex_t	death_lock;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	meals_lock;
	pthread_mutex_t	finished_lock;
	pthread_mutex_t	end_lock;
	pthread_t		sup_tid;
	pthread_t		mon_tid;
	int				death;
	int				end;
}					t_data;

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	size_t			last_meal;
	int				nbr_meals;
	int				finished;
	t_data			*data;
}					t_philo;

// UTILS
size_t				get_current_time(void);
size_t				get_timestamp(size_t start, size_t end);
void				ft_sleep(size_t time_ms);
int					message(t_philo *philo, char *state, char *color);
int					case_one(t_philo *philo);
int					ft_atoi(char *s);

// CHECKERS
int					check_death(t_philo *philo);
int					check_end(t_philo *philo);
int					is_finished(t_philo *philo);
int					is_ended(t_philo *philo);
int					is_dead(t_philo *philo);

// MONITORS
void				finished_eating(t_philo *philo);
void				*supervisor(void *data);
void				*monitor(void *data);

// ROUTINE
int					take_forks(t_philo *philo);
int					_eat(t_philo *philo);
int					_sleep(t_philo *philo);
int					_think(t_philo *philo);
void				*routine(void *data);

// SIMULATION
int					init_mutexes(t_data *data);
int					init_data(t_philo *philos, t_data *data);
int					start_simulation(t_philo *philos, t_data *data);
int					destroy_mutexes(t_data *data);
int					end_simulation(t_philo *philos, t_data *data);

#endif