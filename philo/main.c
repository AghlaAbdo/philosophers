/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thedon <thedon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 19:21:28 by thedon            #+#    #+#             */
/*   Updated: 2024/06/25 23:45:35 by thedon           ###   ########.fr       */
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
}

void    data_init(t_data *data, char **av)
{
	int	i;

	i = -1;
	data->philo_nb = ft_atol(av[1]);
	data->die_time = ft_atol(av[2]);
	data->eat_time = ft_atol(av[3]);
	data->slp_time = ft_atol(av[4]);
	if (av[5])
		data->meals_nb = ft_atol(av[5]);
	else
		data->meals_nb = -1;
	data->forks = ft_malloc(sizeof(t_mtx) * data->philo_nb, 0);
	data->philos = ft_malloc(sizeof(t_philo) * data->philo_nb, 0);
	while (++i < data->philo_nb)
	{
		if (pthread_mutex_init(&data->forks[i].fork, NULL))
			clean_exit("error in mutex init");
		data->forks[i].id = i;
	}
	philos_init(data);
}

// void	threads_init(t_data *data)
// {
// 	int	i;

// 	i = -1;
// 	while (++i < data->philo_nb)
// 	;
// }

int main(int ac, char **av)
{
	t_data data;

	if (ac != 5 && ac != 6)
		return (1);
	data_init(&data, av);
	// threads_init(&data);
	return (0);
}