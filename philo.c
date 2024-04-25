/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabidar <aabidar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 12:46:24 by aabidar           #+#    #+#             */
/*   Updated: 2024/04/25 12:46:27 by aabidar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#define NBR 5
#define TIME_TO_DIE 800
#define TIME_TO_EAT 200
#define TIME_TO_SLEEP 200
#define NBR_OF_MEALS 7

#define C_EAT "\033[0;32m" //EAT
#define C_DIED "\033[0;31m" //DIED
#define C_THINK "\033[0;33m" //THINK
#define C_SLEEP "\033[0;34m" //SLEEP
#define C_NC "\033[0m"

typedef struct s_data
{
    size_t start_time;
    pthread_mutex_t forks[NBR];
    pthread_mutex_t time_lock;
    pthread_mutex_t death_lock;
    pthread_mutex_t print_lock;
    pthread_mutex_t meals_lock;
    pthread_mutex_t finished_lock;
    pthread_mutex_t end_lock;
    pthread_t sup_tid;
    pthread_t mon_tid;
    int death;
    int end;
}   t_data;

typedef struct s_philo
{
    int id;
    pthread_t thread;
    pthread_mutex_t *l_fork;
    pthread_mutex_t *r_fork;
    size_t last_meal;
    int nbr_meals;
    int finished;
    t_data  *data;
}   t_philo;

// UTILS
size_t  get_current_time(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

size_t  get_timestamp(size_t start, size_t end)
{
    return(end - start);
}

void    ft_sleep(size_t time_ms)
{
    size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < time_ms)
		usleep(time_ms / 10);
}

int check_death(t_philo *philo)
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

int check_end(t_philo *philo)
{
    pthread_mutex_lock(&philo->data->end_lock);
    if (philo->data->end == NBR)
    {
        pthread_mutex_unlock(&philo->data->end_lock);
        return (1);
    }
    pthread_mutex_unlock(&philo->data->end_lock);
    return (0);
}

int    message(t_philo *philo, char *state, char *color)
{
    if (check_death(philo))
        return (1);
    pthread_mutex_lock(&philo->data->print_lock);
    printf("%s%zu %d %s\n%s", color, get_timestamp(philo->data->start_time, get_current_time()), philo->id, state, C_NC);
    pthread_mutex_unlock(&philo->data->print_lock);
    return (0);
}

// SIMULATION FUNCTIONS
    // ROUTINE 
int     take_forks(t_philo *philo)
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

int     _eat(t_philo *philo)
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

int     _sleep(t_philo *philo)
{
    if (message(philo, "is sleeping", C_SLEEP))
        return (1);
    ft_sleep(TIME_TO_SLEEP);
    return (0);
}

int     _think(t_philo *philo)
{
    if (message(philo, "is thinking", C_THINK))
        return (1);
    return (0);
}

int     case_one(t_philo *philo)
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

int     is_finished(t_philo *philo)
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

void    *routine(void *data)
{
    t_philo *philo;

    philo = (t_philo *) data;
    if ((philo->id % 2))
        ft_sleep(60);
    if (_think(philo))
            return (NULL);
    if (NBR == 1 && case_one(philo))
        return (NULL);
    while (!is_finished(philo))
    {
        if (_eat(philo))
            break;
        if (_sleep(philo))
            break;
        if (_think(philo))
            break;
        ft_sleep((TIME_TO_DIE - (get_current_time() - philo->last_meal)) / 2);
    }
    return (NULL);
}

    // SUPERVISOR
int     is_ended(t_philo *philo)
{
    pthread_mutex_lock(&philo->data->end_lock);
    if (philo->data->end == NBR)
    {
        pthread_mutex_unlock(&philo->data->end_lock);
        return (1);
    }
    pthread_mutex_unlock(&philo->data->end_lock);
    return (0);
}

int     is_dead(t_philo *philo)
{
    pthread_mutex_lock(&philo->data->time_lock);
    if ((get_current_time() - philo->last_meal) > TIME_TO_DIE)
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

void finished_eating(t_philo *philo)
{
    if (NBR_OF_MEALS >= 0)
    {
        pthread_mutex_lock(&philo->data->meals_lock);
        if (philo->nbr_meals == NBR_OF_MEALS)
        {
            pthread_mutex_lock(&philo->data->finished_lock);
            philo->finished = 1;
            pthread_mutex_unlock(&philo->data->finished_lock);
        }
        pthread_mutex_unlock(&philo->data->meals_lock);
    }
}

void    *supervisor(void *data)
{
    int i;
    t_philo *philos;

    philos = (t_philo *) data;

    while (1)
    {
        i = 0;
        while (i < NBR)
        {
            if (is_ended(&philos[i]))
                return (NULL);
            if (is_dead(&philos[i]))
            {
                pthread_mutex_lock(&philos[i].data->print_lock);
                printf("%s%zu %d %s\n%s", C_DIED, get_timestamp(philos[i].data->start_time, get_current_time()), philos[i].id, "died", C_NC);
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
void    *monitor(void *data)
{
    int i;
    t_philo *philos;

    philos = (t_philo *) data;
    while (1)
    {
        i = 0;
        while (i < NBR)
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

// INITIALIZE DATA
int     init_mutexes(t_data *data)
{
    int i = 0;

    while(i < NBR)
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

int    init_data(t_data *data)
{
    data->start_time = get_current_time();
    if (init_mutexes(data))
        return (1);
    data->death = 0;
    data->end = 0;
    return (0);
}

// INITIALIZE PHILOS & START SIMULATION
void    init_philos(t_philo *philos, t_data *data)
{
    int i;

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
}

int    start_simulation(t_philo *philos, t_data *data)
{
    int i;

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
int     destroy_mutexes(t_data *data)
{
    int i = 0;

    while(i < NBR)
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

int    end_simulation(t_philo *philos, t_data *data)
{
    int i;


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

int main(void)
{
    t_data data;
    t_philo philos[NBR];

    if (init_data(&data))
        return (1);
    init_philos(philos, &data);
    if (start_simulation(philos, &data))
        return (1);
    if (end_simulation(philos, &data))
        return (1);

    return (0);
}
