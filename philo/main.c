/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghla <aaghla@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 19:21:28 by thedon            #+#    #+#             */
/*   Updated: 2024/07/17 13:39:06 by aaghla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	wait_rest(t_data *data)
{
	long long	running;

	while (1)
	{
		running = get_long(&data->run_mtx, &data->running);
		if (running == -1)
			return (1);
		if (running == data->philo_nb)
			break ;
	}
	return (0);
}

void	run_check(t_data *data, long long last_meal, int i)
{
	long long	lng;

	lng = get_long(&data->run_mtx, &data->running);
	if (lng == -1)
	{
		set_bool(&data->end_mtx, &data->end, 1);
		return (NULL);
	}
	while (++i < data->philo_nb)
	{
		my_usleep(2e3);
		last_meal = get_long(&data->philos[i].lstml_mx,
				&data->philos[i].last_meal);
		if (last_meal == -1)
			return (NULL);
		if (my_gettime("MIL_SEC") - last_meal > data->die_time)
		{
			if (set_bool(&data->end_mtx, &data->end, 1)
				|| print_status(data, data->philos + i, "DIE"))
				return (NULL);
			break ;
		}
	}
}

void	*monitor(void *arg)
{
	t_data		*data;
	long long	last_meal;
	int			i;

	data = (t_data *)arg;
	while (!get_bool(&data->end_mtx, &data->end)
		&& get_long(&data->run_mtx, &data->running))
	{
		i = -1;
		while (++i < data->philo_nb && get_long(&data->run_mtx, &data->running))
		{
			my_usleep(2e3);
			last_meal = get_long(&data->philos[i].lstml_mx,
					&data->philos[i].last_meal);
			if (last_meal == -1)
				return (NULL);
			if (my_gettime("MIL_SEC") - last_meal > data->die_time)
			{
				if (set_bool(&data->end_mtx, &data->end, 1)
					|| print_status(data, data->philos + i, "DIE"))
					return (NULL);
				break ;
			}
		}
	}
	return (NULL);
}

int	check_chars(char **av)
{
	int	i;
	int	j;

	i = 0;
	while (av[++i])
	{
		j = -1;
		while (av[i][++j])
		{
			if (!(av[i][j] >= '0' && av[i][j] <= '9'))
				return (1);
		}
	}
	return (0);
}

int	parse_input(char **av)
{
	long long	n;
	int			i;
	int			j;

	i = 0;
	j = -1;
	(*av)++;
	if (check_chars(av))
		return (1);
	while (av[++i])
	{
		if (ft_len(av[i]) > 10 || ft_atol(av[i]) > 2147483647)
			return (1);
	}
	return (0);
}

void	clean_stuff(t_data *data)
{
	int	i;

	i = -1;
	ft_malloc(0, 1);
	while (++i < data->philo_nb)
		pthread_mutex_destroy(&data->forks[i].fork);
	pthread_mutex_destroy(&data->end_mtx);
	pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->run_mtx);
}

int	main(int ac, char **av)
{
	t_data	data;
	int		i;

	i = -1;
	if (ac != 5 && ac != 6)
		return (1);
	if (parse_input(av))
		return (1);
	data_init(&data, av);
	threads_init(&data);
	if (pthread_join(data.monitor, NULL))
		return (1);
	if (data.end)
	{
		while (++i < data.philo_nb)
			if (pthread_detach(data.philos[i].thread))
				return (1);
	}
	else
		while (++i < data.philo_nb)
			if (pthread_join(data.philos[i].thread, NULL))
				return (1);
	clean_stuff(&data);
	return (0);
}
