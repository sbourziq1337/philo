#include "philo.h"

void *routine(void *philo)
{
    t_philo *phil;
    size_t start;

    phil = (t_philo *)philo;
    while (1)
    {
        pthread_mutex_lock(&phil->lock);
        if (phil->flag == 1)
        {
            printf("%zu %d died\n", get_current_time(), phil->index);
            pthread_mutex_unlock(&phil->lock); // Ensure mutex is unlocked
            return (NULL);
        }
        pthread_mutex_unlock(&phil->lock);

        if (phil->index % 2 == 0)
        {
            pthread_mutex_lock(phil->r_fork);
            printf("%zu %d has taken a right fork\n", get_current_time(), phil->index);
            pthread_mutex_lock(phil->l_fork);
            printf("%zu %d has taken a left fork\n", get_current_time(), phil->index);
        }
        else
        {
            pthread_mutex_lock(phil->l_fork);
            printf("%zu %d has taken a left fork\n", get_current_time(), phil->index);
            pthread_mutex_lock(phil->r_fork);
            printf("%zu %d has taken a right fork\n", get_current_time(), phil->index);
        }

        start = get_current_time();
        printf("%zu %d is eating\n", start, phil->index);
        ft_usleep(phil->eating);
        pthread_mutex_unlock(phil->r_fork);
        pthread_mutex_unlock(phil->l_fork);

        start = get_current_time();
        pthread_mutex_lock(&phil->lock);
        printf("%zu %d is sleeping\n", start, phil->index);
        pthread_mutex_unlock(&phil->lock);
        ft_usleep(phil->time_to_sleep);

        start = get_current_time();
        pthread_mutex_lock(&phil->lock);
        printf("%zu %d is thinking\n", start, phil->index);
        pthread_mutex_unlock(&phil->lock);

        // Avoid busy waiting by adding a small sleep between cycles
        ft_usleep(10);
    }
    return (NULL);
}