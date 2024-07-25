#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo
{
	int				eating;
	size_t			start_time;
	size_t			start;
    size_t          start_time_sleep;
	int				flag;
    pthread_mutex_t lock;
	size_t			count_time;
	size_t			end_time;
	int				time_to_sleep;
	int				time_to_die;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	int				index;
	pthread_t		th1;
}					t_philo;

typedef struct s_data
{
	int				num_philo;
	int				num_frok;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_meal;
	pthread_mutex_t	*fork;
}					t_data;

#endif