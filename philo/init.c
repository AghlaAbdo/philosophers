/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghla <aaghla@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 18:54:49 by thedon            #+#    #+#             */
/*   Updated: 2024/08/13 10:36:58 by aaghla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	philos_init(t_data *data, t_philo *philo, int i)
{
	while (++i < data->philo_nb)
	{
		philo = data->philos + i;
		philo->data = data;
		philo->id = i + 1;
		philo->meals = 0;
		philo->full = 0;
		philo->is_dead = 0;
		philo->last_meal = my_gettime("MIL_SEC");
		philo->first_fork = &data->forks[i];
		philo->second_fork = &data->forks[(i + 1) % data->philo_nb];
		if ((i +1) % 2 == 0)
		{
			philo->first_fork = &data->forks[(i + 1) % data->philo_nb];
			philo->second_fork = &data->forks[i];
		}
		if (pthread_mutex_init(&philo->lstml_mx, NULL)
			|| pthread_mutex_init(&philo->dead_mtx, NULL)
			|| pthread_mutex_init(&data->forks[i].fork, NULL)
			|| pthread_mutex_init(&philo->full_mtx, NULL))
			return (1);
		data->forks[i].id = i;
		if (!data->meals_nb)
			set_int(&philo->full_mtx, &philo->full, 1);
	}
	return (0);
}

int	data_init(t_data *data, char **av)
{
	data->philo_nb = ft_atol(av[1]);
	data->die_time = ft_atol(av[2]);
	data->eat_time = ft_atol(av[3]) * 1000;
	data->slp_time = ft_atol(av[4]) * 1000;
	data->end = 0;
	data->running = 0;
	data->meals_nb = -1;
	data->status = 0;
	data->full = 0;
	if (av[5])
		data->meals_nb = (int)ft_atol(av[5]);
	if (pthread_mutex_init(&data->end_mtx, NULL)
		|| pthread_mutex_init(&data->print, NULL)
		|| pthread_mutex_init(&data->run_mtx, NULL)
		|| pthread_mutex_init(&data->full_mtx, NULL)
		|| pthread_mutex_init(&data->stat_mtx, NULL)
		|| pthread_mutex_init(&data->strt_mtx, NULL))
		return (1);
	return (philos_init(data, NULL, -1));
}

int	threads_init(t_data *data)
{
	int	i;

	i = -1;
	data->simul_strt = -1;
	while (++i < data->philo_nb)
	{
	// set_long(&data->strt_mtx, &data->simul_strt, my_gettime("MIL_SEC"));
	// 	set_long(&data->philos[i].lstml_mx, &data->philos[i].last_meal, my_gettime("MIL_SEC"));
		if (pthread_create(&data->philos[i].thread, NULL,
				&simulation, &data->philos[i]))
			return (1);
	}
	// i = -1;
	long long meal = my_gettime("MIL_SEC");
	while (++i < data->philo_nb)
		data->philos[i].last_meal = meal;
	set_long(&data->strt_mtx, &data->simul_strt, meal);
		
	// data->simul_strt = my_gettime("MIL_SEC");
	// usleep(100000);
	wait_rest(data);
	if (pthread_create(&data->monitor1, NULL, &monitor1, data))
		return (1);
	// if (pthread_create(&data->monitor2, NULL, &monitor2, data))
	// 	return (1);
	return (0);
}

int	simul_init(void *arg, t_philo *philo, t_data *data)
{
	// set_long(&philo->lstml_mx, &philo->last_meal, my_gettime("MIL_SEC"));
	if (increase_long(&data->run_mtx, &data->running, "++") == -1
		|| wait_rest(data) || sync_philos(philo))
		return (1);
	// usleep(200);
	return (0);
}
