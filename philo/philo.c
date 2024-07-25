#include "philo.h"

int	ft_atoi(char *av)
{
	int	number;
	int	i;

	number = 0;
	i = 0;
	if (!av)
		return (-1);
	while ((av[i] >= 9 && av[i] <= 13) || (av[i] == 32))
		i++;
	if (av[i] == '+')
		i++;
	if (av[i] == '-')
	{
		write(2, "we need just positive number\n", 30);
		return (-1);
	}
	while (av[i] != '\0')
	{
		if (av[i] >= '0' && av[i] <= '9')
		{
			number = number * 10 + (av[i] - '0');
			i++;
		}
		else
		{
			write(2, "Errore char\n", 12);
			return (-1);
		}
	}
	return (number);
}
size_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < milliseconds)
		usleep(500);
	return (0);
}

void *routine(void *philo)
{
    t_philo *phil;

    phil = (t_philo *)philo;
    phil->start = get_current_time();
    while (1)
    {
        pthread_mutex_lock(&phil->lock);
        if (phil->flag == 1)
        {
            printf("%zu %d die\n", get_current_time() - phil->start, phil->index);
            pthread_mutex_unlock(&phil->lock);
            return (NULL);
        }
        pthread_mutex_unlock(&phil->lock);

        if (phil->index % 2 == 0)
        {
            pthread_mutex_lock(phil->r_fork);
            printf("%zu %d has taken a right fork\n", get_current_time() - phil->start, phil->index);
            pthread_mutex_lock(phil->l_fork);
            printf("%zu %d has taken a left fork\n", get_current_time() - phil->start, phil->index);
        }
        else
        {
            pthread_mutex_lock(phil->l_fork);
            printf("%zu %d has taken a left fork\n", get_current_time() - phil->start, phil->index);
            pthread_mutex_lock(phil->r_fork);
            printf("%zu %d has taken a right fork\n", get_current_time() - phil->start, phil->index);
        }

        phil->start_time = get_current_time();
        printf("%zu %d is eating\n", get_current_time() - phil->start, phil->index);
        ft_usleep(phil->eating);
        pthread_mutex_unlock(phil->r_fork);
        pthread_mutex_unlock(phil->l_fork);

        pthread_mutex_lock(&phil->lock);
        printf("%zu %d is sleeping\n", get_current_time() - phil->start, phil->index);
        pthread_mutex_unlock(&phil->lock);
        ft_usleep(phil->time_to_sleep);

        pthread_mutex_lock(&phil->lock);
        printf("%zu %d is thinking\n", get_current_time() - phil->start, phil->index);
        pthread_mutex_unlock(&phil->lock);
    }
}

void	ft_parsin(t_data *data, char **av)
{
	data->num_philo = ft_atoi(av[1]);
	data->num_frok = data->num_philo;
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (av[5])
		data->num_meal = ft_atoi(av[5]);
	else
		data->num_meal = -1;
}

int	main(int argc, char **argv)
{
	int i;
	t_data data;
	t_philo *philo;
	int number;

	i = 0;
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

	while (i < data.num_philo)
	{
		pthread_mutex_init(&data.fork[i], NULL);
		i++;
	}
	pthread_mutex_init(&philo->lock, NULL);
	i = 0;
	while (i < data.num_philo)
	{
		philo[i].eating = data.time_to_eat;
		philo[i].time_to_sleep = data.time_to_sleep;
		philo[i].time_to_die = data.time_to_die;
		philo[i].r_fork = &data.fork[i];
		philo[i].l_fork = &data.fork[(i + 1) % data.num_philo];
		philo[i].index = i + 1;
		i++;
	}
	i = 0;
	philo->start_time = get_current_time();
	while (i < data.num_philo)
	{
		if (pthread_create(&philo[i].th1, NULL, &routine, &philo[i]) != 0)
		{
			write(2, "failed creating thread\n", 23);
			free(philo);
			free(data.fork);
			return (1);
		}
		i++;
	}
	usleep(400);
	while (1)
	{
		for (int j = 0; j < data.num_philo; j++)
		{
			pthread_mutex_lock(&philo->lock);
			if ((get_current_time() - philo->start_time) >= data.time_to_die)
			{
				philo->flag = 1;
				pthread_mutex_unlock(&philo->lock);
				break ;
			}
			pthread_mutex_unlock(&philo->lock);
			j++;
		}
		if (philo->flag == 1)
			break ;
	}
	i = 0;
	while (i < data.num_philo)
	{
		if (pthread_join(philo[i].th1, (void **)&number) != 0)
		{
			write(2, "failed joining thread\n", 22);
			free(philo);
			free(data.fork);
			return (1);
		}
		if (number == 0)
			return (0);
		i++;
	}

	free(philo);
	free(data.fork);
	return (0);
}