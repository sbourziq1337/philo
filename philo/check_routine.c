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

int initialize_mutexes(t_data *data)
{
    for (int i = 0; i < data->num_philo; i++)
    {
        if (pthread_mutex_init(&data->fork[i], NULL) != 0)
        {
            perror("Failed to initialize fork mutex");
            return 1;
        }
        data->philo[i].time_to_eat = data->time_to_eat;
        data->philo[i].time_to_sleep = data->time_to_sleep;
        data->philo[i].time_to_die = data->time_to_die;
        data->philo[i].r_fork = &data->fork[i];
        data->philo[i].l_fork = &data->fork[(i + 1) % data->num_philo];
        data->philo[i].index = i + 1;
        data->philo[i].my_data = data;
        data->philo[i].start_time = get_current_time();
        data->philo[i].start = get_current_time();
		
    }
    if (pthread_mutex_init(&data->lock, NULL) != 0)
    {
        perror("Failed to initialize lock mutex");
        return 1;
    }
    return 0;
}
void monitor_philosophers(t_data *data, t_philo *philo)
{
    usleep(400);
	while (1)
	{
		for (int i = 0; i < data->num_philo; i++)
		{
			pthread_mutex_lock(&data->lock);
			if (get_current_time() - philo[i].start_time > data->time_to_die)
			{
				printf("%zu %d died\n", get_current_time() - philo[i].start,
					philo[i].index);
				data->flag = 1;
				pthread_mutex_unlock(&data->lock);
				return ;
			}
			if(data->num_meal == data->count_meal)
			{
				data->flag = 1;
				pthread_mutex_unlock(&data->lock);
				return ;
			}
			pthread_mutex_unlock(&data->lock);
		}
	}
}