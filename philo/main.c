/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thedon <thedon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 19:21:28 by thedon            #+#    #+#             */
/*   Updated: 2024/06/29 22:26:08 by thedon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean_exit(char *err)
{
	printf("%s\n", err);
	ft_malloc(0, 1);
}

void	philos_init(t_data *data)
{
	int	i;
	t_philo	*philo;

	i = -1;
	while (++i < data->philo_nb)
	{
		philo = data->philos + i;
		philo->data = data;
		philo->id = i + 1;
		philo->meals = 0;
		philo->full = false;
		philo->first_fork = &data->forks[i];
		philo->last_meal = my_gettime("MIL_SEC");
		philo->second_fork = &data->forks[(i + 1) % data->philo_nb];
		if ((i +1) % 2 == 0)
		{
			philo->first_fork = &data->forks[(i + 1) % data->philo_nb];
			philo->second_fork = &data->forks[i];
		}
	}
	i = -1;
	while (++i < data->philo_nb)
	{
		printf("philo id: %d\n", data->philos[i].id);
		printf("\tfirst fork id: %lld\n", data->philos[i].first_fork->id);
		printf("\tsecond fork id: %lld\n", data->philos[i].second_fork->id);
	}
}

void    data_init(t_data *data, char **av)
{
	int	i;

	i = -1;
	data->philo_nb = ft_atol(av[1]);
	data->die_time = ft_atol(av[2]);
	data->eat_time = ft_atol(av[3]) * 1000;
	data->slp_time = ft_atol(av[4]) * 1000;
	data->ready = false;
	data->end = false;
	if (av[5])
		data->meals_nb = (int)ft_atol(av[5]);
	else
		data->meals_nb = -1;
	data->forks = ft_malloc(sizeof(t_fork) * data->philo_nb, 0);
	data->philos = ft_malloc(sizeof(t_philo) * data->philo_nb, 0);
	while (++i < data->philo_nb)
	{
		if (pthread_mutex_init(&data->forks[i].fork, NULL))
			clean_exit("error in mutex init");
		data->forks[i].id = i;
	}
	if (pthread_mutex_init(&data->ready_mtx, NULL))
		clean_exit("error in mutex init");
	if (pthread_mutex_init(&data->end_mtx, NULL))
		clean_exit("error in mutex init");
	if (pthread_mutex_init(&data->print, NULL))
		clean_exit("error in mutex init");
	philos_init(data);
}

void	wait_rest(t_data *data)
{
	while (!get_bool(&data->ready_mtx, &data->ready))
		;
}

void	print_status(t_data *data, t_philo *philo, char *status)
{
	pthread_mutex_lock(&data->print);
	if (!get_bool(&data->end_mtx, &data->end))
	{
		if (!ft_strcmp(status, "FORK_1") && !get_bool(&data->end_mtx, &data->end))
			printf("%lld %d has taken a first fork: %lld\n",my_gettime("MIL_SEC") - data->simul_strt,
					philo->id, philo->first_fork->id);
		else if (!ft_strcmp(status, "FORK_2") && !get_bool(&data->end_mtx, &data->end))
			printf("%lld %d has taken second fork: %lld\n",my_gettime("MIL_SEC") - data->simul_strt,
					philo->id, philo->second_fork->id);
		else if (!ft_strcmp(status, "EAT") && !get_bool(&data->end_mtx, &data->end))
			printf("%lld %d is eating\n",my_gettime("MIL_SEC") - data->simul_strt,
					philo->id);
		else if (!ft_strcmp(status, "SLEEP") && !get_bool(&data->end_mtx, &data->end))
			printf("%lld %d is sleeping\n",my_gettime("MIL_SEC") - philo->data->simul_strt,
				philo->id);
		else if (!ft_strcmp(status, "THINK") && !get_bool(&data->end_mtx, &data->end))
			printf("%lld %d is thinking\n",my_gettime("MIL_SEC") - philo->data->simul_strt,
				philo->id);
		else if (!ft_strcmp(status, "DIE") && !get_bool(&data->end_mtx, &data->end))
			printf("%lld %d died\n", my_gettime("MIL_SEC") - data->simul_strt, philo->id);
		else
			clean_exit("check print_status() args\n");
	}
	pthread_mutex_unlock(&data->print);
}

void	my_usleep(long long usec, t_data *data)
{
	long long	start;
	long long	elapsed;
	long long	rem;

	start = my_gettime("MIC_SEC");
	while (my_gettime("MIC_SEC") - start < usec)
	{
		if (get_bool(&data->end_mtx, &data->end))
			break ;
		elapsed = my_gettime("MIC_SEC") - start;
		rem = usec - elapsed;
		if (rem > 1e3)
			usleep(rem / 2);
		else
		{
			while(my_gettime("MIC_SEC") - start < usec)
				;
		}
	}
}

void	eat(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&philo->first_fork->fork);
	print_status(data, philo, "FORK_1");
	pthread_mutex_lock(&philo->second_fork->fork);
	print_status(data, philo, "FORK_2");
	print_status(data, philo, "EAT");
	philo->meals++;
	if (data->meals_nb > 0 && philo->meals == data->meals_nb)
		philo->full = true;
	if (!data->end)
	if (!get_bool(&data->end_mtx, &data->end))
		usleep(data->eat_time);
	set_long(&philo->last_meal_mtx, &philo->last_meal, my_gettime("MIL_SEC"));
	pthread_mutex_unlock(&philo->second_fork->fork);
	pthread_mutex_unlock(&philo->first_fork->fork);
}

void	sync_philos(t_philo *philo)
{
	long long	think;
	// printf("philo nb: %lld\ndie time: %lld\neat time: %lld\nsleep time: %lld\n",
		// philo->data->philo_nb, philo->data->die_time, philo->data->eat_time, philo->data->slp_time);
	if (philo->data->philo_nb % 2 == 0)
	{
		if (philo->id % 2 == 0)
			usleep(3e4);
	}
	else
	{
		if (philo->id % 2)
		{
			think = philo->data->eat_time * 2 - philo->data->slp_time;
			if (think > 0)
			{
				usleep(think * 0.45);
			}
		}
	}
}

void	think(t_data *data, t_philo *philo)
{
	long long	think;

	print_status(data, philo, "THINK");
	if (data->philo_nb % 2 == 0)
		return ;
	think = philo->data->eat_time * 2 - philo->data->slp_time;
	if (think > 0)
		usleep(think * 0.45);
}

void	*simulation(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	wait_rest(philo->data);
	set_long(&philo->last_meal_mtx, &philo->last_meal, my_gettime("MIL_SEC"));
	if (!data->meals_nb)
		philo->full = true;
	sync_philos(philo);
	while (!get_bool(&data->end_mtx, &data->end))
	{
		if (philo->full)
			break;
		eat(philo);
		print_status(data, philo, "SLEEP");
		if (!get_bool(&data->end_mtx, &data->end))
			usleep(philo->data->slp_time);
		think(data, philo);
	}
	return (NULL);
}

void	*monitor(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	usleep(4e4);
	while (!get_bool(&data->end_mtx, &data->end))
	{
		i = -1;
		while (++i < data->philo_nb)
		{
			if (my_gettime("MIL_SEC") - get_long(&data->philos[i].last_meal_mtx, &data->philos[i].last_meal)
					>= data->die_time)
			{
				printf("philo last meal: %lld time: %lld %d died\n",my_gettime("MIL_SEC") - get_long(&data->philos[i].last_meal_mtx, &data->philos[i].last_meal), my_gettime("MIL_SEC") - data->simul_strt,
					data->philos[i].id);
				// write(1, "is dead\n", 8);
				// printf("%lld %d died\n", my_gettime("MIL_SEC") - data->simul_strt, data->philos[i].id);
				print_status(data, data->philos + i, "DIE");
				set_bool(&data->end_mtx, &data->end, true);
				break;
			}
		}
	}
	
}

void	threads_init(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->philo_nb)
	{
		if (pthread_create(&data->philos[i].thread, NULL, &simulation, &data->philos[i]))
			clean_exit("error creating thread");
	}
	data->simul_strt = my_gettime("MIL_SEC");
	data->ready = true;
	if (pthread_create(&data->monitor, NULL, &monitor, data))
		clean_exit("error creating thread");
}

int main(int ac, char **av)
{
	t_data data;
	int		i;

	i = -1;
	if (ac != 5 && ac != 6)
		return (1);
	data_init(&data, av);
	threads_init(&data);

	while (++i < data.philo_nb)
	{
		if (pthread_join(data.philos[i].thread, NULL))
			clean_exit("error joining thread");
	}
	if (pthread_join(data.monitor, NULL))
		clean_exit("error joining thread");
	return (0);
}