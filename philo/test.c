// #include "philo.h"

// void *routine(void *philo)
// {
//     t_philo *phil = (t_philo *)philo;
//     int *result = malloc(sizeof(int));
//     if (!result)
//     {
//         printf("Failed to allocate memory for result\n");
//         return NULL;
//     }
//     *result = 1;

//     if (phil->index % 2 == 1)
//         usleep(500);

//     while (1)
//     {
//         if (check_simulation_end(phil))
//             break;

//         if (phil->index % 2 == 0)
//             take_forks(phil, phil->r_fork, phil->l_fork);
//         else
//             take_forks(phil, phil->l_fork, phil->r_fork);

//         if (eat(phil))
//             break;

//         if (phil_sleep(phil))
//             break;

//         if (think(phil))
//             break;
//     }

//     free(result);
//     return NULL;
// }

// int check_simulation_end(t_philo *phil)
// {
//     pthread_mutex_lock(&phil->my_data->lock);
//     if (phil->my_data->flag)
//     {
//         pthread_mutex_unlock(&phil->my_data->lock);
//         return 1;
//     }
//     pthread_mutex_unlock(&phil->my_data->lock);
//     return 0;
// }

// void take_forks(t_philo *phil, pthread_mutex_t *first, pthread_mutex_t *second)
// {
//     pthread_mutex_lock(first);
//     if (locked_print("has taken a fork", phil))
//     {
//         pthread_mutex_unlock(first);
//         return;
//     }
//     pthread_mutex_lock(second);
//     if (locked_print("has taken a fork", phil))
//     {
//         pthread_mutex_unlock(first);
//         pthread_mutex_unlock(second);
//         return;
//     }
// }

// int eat(t_philo *phil)
// {
//     pthread_mutex_lock(&phil->my_data->lock);
//     phil->start_time = get_current_time();
//     if (!phil->my_data->flag)
//         printf("%zu %d is eating\n", get_current_time() - phil->start, phil->index);
//     // phil->meals_eaten++;
//     pthread_mutex_unlock(&phil->my_data->lock);

//     ft_usleep(phil->time_to_eat);
//     pthread_mutex_unlock(phil->r_fork);
//     pthread_mutex_unlock(phil->l_fork);

//     return check_simulation_end(phil);
// }

// int phil_sleep(t_philo *phil)
// {
//     if (locked_print("is sleeping", phil))
//         return 1;
//     ft_usleep(phil->time_to_sleep);
//     return check_simulation_end(phil);
// }

// int think(t_philo *phil)
// {
//     return locked_print("is thinking", phil);
// }

// int main(int argc, char **argv)
// {
//     t_data data;
//     t_philo *philo;

//     if (argc < 5 || argc > 6)
//     {
//         write(2, "Invalid number of arguments\n", 28);
//         return 1;
//     }

//     if (ft_parsing(&data, argv) != 0)
//         return 1;

//     philo = malloc(data.num_philo * sizeof(t_philo));
//     if (!philo)
//     {
//         perror("Failed to allocate memory for philo");
//         return 1;
//     }

//     if (initialize_mutexes(&data) != 0)
//     {
//         free(philo);
//         return 1;
//     }

//     if (create_philosophers(&data, philo) != 0)
//     {
//         cleanup(&data, philo);
//         return 1;
//     }

//     monitor_philosophers(&data, philo);
//     join_threads(&data, philo);
//     cleanup(&data, philo);

//     return 0;
// }



#include "philo.h"

void	*routine(void *philo)
{
	t_philo	*phil;
	int		*result;

	phil = (t_philo *)philo;
	result = malloc(sizeof(int));
	if (!result)
	{
		printf("Failed to allocate memory for result\n");
		return (NULL);
	}
	*result = 1;
	if (phil->index % 2 == 1)
		usleep(500);
	while (1)
	{
		pthread_mutex_lock(&phil->my_data->lock);
		if (phil->my_data->flag == 1)
		{
			pthread_mutex_unlock(&phil->my_data->lock);
			return ((void *)result);
		}
		pthread_mutex_unlock(&phil->my_data->lock);
		if (phil->index % 2 == 0 && (phil->my_data->flag != 1))
		{
			pthread_mutex_lock(phil->r_fork);
			if (locked_print("has taken a right fork", phil) == 1)
			{
				pthread_mutex_unlock(phil->r_fork);
				return ((void *)result);
			}
			pthread_mutex_lock(phil->l_fork);
			if (locked_print("has taken a left fork", phil) == 1)
			{
				pthread_mutex_unlock(phil->r_fork);
				pthread_mutex_unlock(phil->l_fork);
				return ((void *)result);
			}
		}
		else if (phil->my_data->flag != 1)
		{
			pthread_mutex_lock(phil->l_fork);
			if (locked_print("has taken a left fork", phil) == 1)
			{
				pthread_mutex_unlock(phil->l_fork);
				return ((void *)result);
			}
			pthread_mutex_lock(phil->r_fork);
			if (locked_print("has taken a right fork", phil) == 1)
			{
				pthread_mutex_unlock(phil->l_fork);
				pthread_mutex_unlock(phil->r_fork);
				return ((void *)result);
			}
		}
		pthread_mutex_lock(&phil->my_data->lock);
		phil->start_time = get_current_time();
		if (phil->my_data->flag != 1)
			printf("%zu %d is eating\n", get_current_time() - phil->start,
				phil->index);
		pthread_mutex_unlock(&phil->my_data->lock);
		ft_usleep(phil->time_to_eat);
		pthread_mutex_unlock(phil->r_fork);
		pthread_mutex_unlock(phil->l_fork);
        locked_print("is sleeping", phil);
		ft_usleep(phil->time_to_sleep);
        locked_print("is thinking", phil);
	}
	return ((void *)result);
}

int	main(int argc, char **argv)
{
	t_data	data;
	t_philo	*philo;
	int		*number;

	if (argc < 5 || argc > 6)
	{
		write(2, "Invalid number of arguments\n", 28);
		return (1);
	}
	ft_parsin(&data, argv);
	philo = malloc(data.num_philo * sizeof(t_philo));
	if (!philo)
	{
		perror("Failed to allocate memory for philo");
		return (1);
	}
	data.fork = malloc(data.num_philo * sizeof(pthread_mutex_t));
	if (!data.fork)
	{
		perror("Failed to allocate memory for forks");
		free(philo);
		return (1);
	}
	for (int i = 0; i < data.num_philo; i++)
		pthread_mutex_init(&data.fork[i], NULL);
	pthread_mutex_init(&data.lock, NULL);
	data.flag = 0;
    ft_creat(&data, philo);
	usleep(400);
	while (1)
	{
		for (int j = 0; j < data.num_philo; j++)
		{
			pthread_mutex_lock(&data.lock);
			if ((get_current_time() - philo[j].start_time) >= data.time_to_die)
			{
				data.flag = 1;
				printf("%zu %d died\n", get_current_time()
					- philo[j].start_time, philo[j].index);
				pthread_mutex_unlock(&data.lock);
				break ;
			}
			pthread_mutex_unlock(&data.lock);
		}
		if (data.flag == 1)
			break ;
	}
	for (int i = 0; i < data.num_philo; i++)
	{
		if (pthread_join(philo[i].th1, (void **)&number) != 0)
		{
			write(2, "Failed joining thread\n", 22);
			free(philo);
			free(data.fork);
			break ;
		}
	}
	for (int i = 0; i < data.num_philo; i++)
	{
		if (pthread_mutex_destroy(&data.fork[i]) != 0)
		{
			pthread_mutex_destroy(&data.lock);
			write(2, "Failed destroy mutex\n", 22);
			free(philo);
			free(data.fork);
			return (1);
		}
	}
	pthread_mutex_destroy(&data.lock);
	free(philo);
	free(data.fork);
	return (0);
}
////////////////
#include "philo.h"

void	*routine(void *philo)
{
	t_philo	*phil;

	phil = (t_philo *)philo;
	if (phil->index % 2 == 1)
		usleep(500);
	while (1)
	{
		if (check_simulation_end(phil))
			break ;
		if (phil->index % 2 == 0)
		{
			if (take_forks(phil, phil->r_fork, phil->l_fork))
				break ;
		}
		else
		{
			if (take_forks(phil, phil->l_fork, phil->r_fork))
				break ;
		}
		if (eat(phil))
			break ;
		pthread_mutex_lock(&phil->my_data->lock);
		// if (phil->my_data->num_meal != -1
		// 	&& phil->meals_eaten == phil->my_data->num_meal)
		// {
		// 	phil->my_data->count_meal++;
		// 	pthread_mutex_unlock(&phil->my_data->lock);
		// 	break ;
		// }
		// pthread_mutex_unlock(&phil->my_data->lock);
		if (phil_sleep(phil))
			break ;
		if (think(phil))
			break ;
	}
	return (NULL);
}

int	check_simulation_end(t_philo *phil)
{
	pthread_mutex_lock(&phil->my_data->lock);
	if (phil->my_data->flag)
	{
		pthread_mutex_unlock(&phil->my_data->lock);
		return (1);
	}
	pthread_mutex_unlock(&phil->my_data->lock);
	return (0);
}

int	take_forks(t_philo *phil, pthread_mutex_t *first, pthread_mutex_t *second)
{
	pthread_mutex_lock(first);
	if (locked_print("has taken a fork", phil))
	{
		pthread_mutex_unlock(first);
		return (1);
	}
	pthread_mutex_lock(second);
	if (locked_print("has taken a fork", phil))
	{
		pthread_mutex_unlock(first);
		pthread_mutex_unlock(second);
		return (1);
	}
	return (0);
}

int	eat(t_philo *phil)
{
	pthread_mutex_lock(&phil->my_data->lock);
	phil->start_time = get_current_time();
	if (!phil->my_data->flag)
		printf("%zu %d is eating\n", get_current_time() - phil->start,
			phil->index);
	// if (phil->my_data->num_meal != -1)
	// 	phil->meals_eaten++;
	pthread_mutex_unlock(&phil->my_data->lock);
	ft_usleep(phil->time_to_eat);
	pthread_mutex_unlock(phil->r_fork);
	pthread_mutex_unlock(phil->l_fork);
	return (check_simulation_end(phil));
}

int	phil_sleep(t_philo *phil)
{
	if (locked_print("is sleeping", phil))
		return (1);
	ft_usleep(phil->time_to_sleep);
	return (check_simulation_end(phil));
}

int	think(t_philo *phil)
{
	return (locked_print("is thinking", phil));
}

int	main(int argc, char **argv)
{
	t_data data;

	if (argc < 5 || argc > 6)
	{
		write(2, "Invalid number of arguments\n", 28);
		return (1);
	}

	if (ft_parsing(&data, argv) != 0)
		return (1);

	data.philo = malloc(data.num_philo * sizeof(t_philo));
	if (!data.philo)
	{
		perror("Failed to allocate memory for philo");
		return (1);
	}

	data.fork = malloc(data.num_philo * sizeof(pthread_mutex_t));
	if (!data.fork)
	{
		perror("Failed to allocate memory for forks");
		free(data.philo);
		return (1);
	}

	if (initialize_mutexes(&data) != 0)
	{
		free(data.philo);
		free(data.fork);
		return (1);
	}

	if (create_philosophers(&data) != 0)
	{
		// Cleanup mutexes
		for (int i = 0; i < data.num_philo; i++)
			pthread_mutex_destroy(&data.fork[i]);
		pthread_mutex_destroy(&data.lock);
		free(data.philo);
		free(data.fork);
		return (1);
	}

	monitor_philosophers(&data, data.philo);
	join_threads(&data, data.philo);

	// Cleanup
	for (int i = 0; i < data.num_philo; i++)
		pthread_mutex_destroy(&data.fork[i]);
	pthread_mutex_destroy(&data.lock);
	free(data.philo);
	free(data.fork);

	return (0);
}