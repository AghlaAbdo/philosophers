/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghla <aaghla@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 18:54:49 by thedon            #+#    #+#             */
/*   Updated: 2024/07/17 13:22:16 by aaghla           ###   ########.fr       */
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
		philo->first_fork = &data->forks[i];
		philo->last_meal = my_gettime("MIL_SEC");
		philo->second_fork = &data->forks[(i + 1) % data->philo_nb];
		if ((i +1) % 2 == 0)
		{
			philo->first_fork = &data->forks[(i + 1) % data->philo_nb];
			philo->second_fork = &data->forks[i];
		}
		if (pthread_mutex_init(&philo->lstml_mx, NULL)
			|| pthread_mutex_init(&philo->dead_mtx, NULL)
			|| pthread_mutex_init(&data->forks[i].fork, NULL))
			return (1);
		data->forks[i].id = i;
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
	if (av[5])
		data->meals_nb = (int)ft_atol(av[5]);
	data->forks = ft_malloc(sizeof(t_fork) * data->philo_nb, 0);
	data->philos = ft_malloc(sizeof(t_philo) * data->philo_nb, 0);
	if (pthread_mutex_init(&data->end_mtx, NULL)
		|| pthread_mutex_init(&data->print, NULL)
		|| pthread_mutex_init(&data->run_mtx, NULL))
		return (1);
	return (philos_init(data, NULL, -1));
}

int	threads_init(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->philo_nb)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
				&simulation, &data->philos[i]))
			return (1);
	}
	data->simul_strt = my_gettime("MIL_SEC");
	wait_rest(data);
	if (pthread_create(&data->monitor, NULL, &monitor, data))
		return (1);
	return (0);
}
