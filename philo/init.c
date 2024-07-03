/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thedon <thedon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 18:54:49 by thedon            #+#    #+#             */
/*   Updated: 2024/07/03 20:22:30 by thedon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philos_init(t_data *data, t_philo *philo, int i)
{
	while (++i < data->philo_nb)
	{
		philo = data->philos + i;
		philo->data = data;
		philo->id = i + 1;
		philo->meals = 0;
		philo->full = false;
		philo->is_dead = false;
		philo->first_fork = &data->forks[i];
		philo->last_meal = my_gettime("MIL_SEC");
		philo->second_fork = &data->forks[(i + 1) % data->philo_nb];
		if ((i +1) % 2 == 0)
		{
			philo->first_fork = &data->forks[(i + 1) % data->philo_nb];
			philo->second_fork = &data->forks[i];
		}
		if (pthread_mutex_init(&philo->last_meal_mtx, NULL)
			|| pthread_mutex_init(&philo->dead_mtx, NULL)
			|| pthread_mutex_init(&data->forks[i].fork, NULL))
			clean_exit("error in mutex init");
		data->forks[i].id = i;
	}
	i = -1;
	while (++i < data->philo_nb)
	{
		printf("philo id: %d\n", data->philos[i].id);
		printf("\tfirst fork id: %lld\n", data->philos[i].first_fork->id);
		printf("\tsecond fork id: %lld\n", data->philos[i].second_fork->id);
	}
}

void	data_init(t_data *data, char **av)
{
	data->philo_nb = ft_atol(av[1]);
	data->die_time = ft_atol(av[2]);
	data->eat_time = ft_atol(av[3]) * 1000;
	data->slp_time = ft_atol(av[4]) * 1000;
	data->end = false;
	data->running = 0;
	data->meals_nb = -1;
	if (av[5])
		data->meals_nb = (int)ft_atol(av[5]);
	printf("philo nb: %lld\ndie time: %lld\neat time: %lld\nslp time: %lld\n", data->philo_nb,
		data->die_time, data->eat_time, data->slp_time);
	data->forks = ft_malloc(sizeof(t_fork) * data->philo_nb, 0);
	data->philos = ft_malloc(sizeof(t_philo) * data->philo_nb, 0);
	if (pthread_mutex_init(&data->end_mtx, NULL)
		|| pthread_mutex_init(&data->print, NULL)
		|| pthread_mutex_init(&data->run_mtx, NULL))
		clean_exit("error in mutex init");
	philos_init(data, NULL, -1);
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
	wait_rest(data);
	data->simul_strt = my_gettime("MIL_SEC");
	if (pthread_create(&data->monitor, NULL, &monitor, data))
		clean_exit("error creating thread");
}