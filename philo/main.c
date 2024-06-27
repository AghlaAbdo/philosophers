/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thedon <thedon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 19:21:28 by thedon            #+#    #+#             */
/*   Updated: 2024/06/27 22:23:05 by thedon           ###   ########.fr       */
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
		philo->first_fork = &data->forks[i];
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
	data->die_time = ft_atol(av[2]) * 1000;
	data->eat_time = ft_atol(av[3]) * 1000;
	data->slp_time = ft_atol(av[4]) * 1000;
	data->ready_thrds = 0;
	if (av[5])
		data->meals_nb = ft_atol(av[5]);
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
	i = -1;
	while (++i < data->philo_nb)
	{
		printf("i = %d fork id: %lld\n", i, data->forks[i].id);
	}
	if (pthread_mutex_init(&data->ready, NULL))
		clean_exit("error in mutex init");
	philos_init(data);
}

void	wait_rest(t_data *data)
{
	int	i;
	int	phil;

	i = -1;
	pthread_mutex_lock(&data->ready);
	data->ready_thrds++;
	phil = data->ready_thrds;
	pthread_mutex_unlock(&data->ready);
	// printf("phil: %d\nready threads: %lld\nphilo nb: %lld\n", phil, data->ready_thrds, data->philo_nb);
	while (phil != data->philo_nb)
	{
		pthread_mutex_lock(&data->ready);
		phil = data->ready_thrds;
		pthread_mutex_unlock(&data->ready);
	}
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->first_fork->fork);
	printf("philo %d has taken a first fork: %lld\n", philo->id, philo->first_fork->id);
	pthread_mutex_lock(&philo->second_fork->fork);
	printf("philo %d has taken second fork: %lld\n", philo->id, philo->second_fork->id);
	printf("philo %d is eating\n", philo->id);
	usleep(philo->data->eat_time);
	pthread_mutex_unlock(&philo->second_fork->fork);
	pthread_mutex_unlock(&philo->first_fork->fork);
}

void	*simulation(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	// printf("philo %d is there\n", philo->id);
	wait_rest(philo->data);
	
	while (1)
	{
		eat(philo);
		printf("philo %d is sleeping\n", philo->id);
		usleep(philo->data->slp_time);
		printf("philo %d is thingking\n", philo->id);
	}
	
	return (NULL);
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
	return (0);
}