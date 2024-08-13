/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghla <aaghla@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 19:21:28 by thedon            #+#    #+#             */
/*   Updated: 2024/08/13 11:13:26 by aaghla           ###   ########.fr       */
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


// int	run_check(t_data *data, long long last_meal, long long lng, int i)
// {
// 	while (++i < data->philo_nb)
// 	{
// 		// lng = get_long(&data->run_mtx, &data->running);
// 		// if (lng == -1)
// 		// 	return (1);
// 		// else if (!lng)
// 		// 	break ;
// 		// if (my_usleep(2e3))
// 		// 	return (1);
// 		last_meal = get_long(&data->philos[i].lstml_mx,
// 				&data->philos[i].last_meal);
// 		if (last_meal == -1)
// 			return (1);
// 		if (my_gettime("MIL_SEC") - last_meal > data->die_time)
// 		{
// 			printf("dieee\n");
// 			if (print_status(data, data->philos + i, "DIE", 0)
// 				|| set_int(&data->end_mtx, &data->end, 1))
// 				return (1);
// 			break ;
// 		}
// 	}
// 	return (0);
// }

void	*monitor1(void *arg)
{
	t_data		*data;
	long long	last_meal;
	int			full;
	int			end;
	int			i;

	data = (t_data *)arg;
	while (1)
	{
		i = -1;
		end = get_int(&data->end_mtx, &data->end);
		full = get_int(&data->full_mtx, &data->full);
		// full = 1;
		if (end == -1 || full == -1)
			return (NULL);
			// return set_error(data, 2), NULL);
		if (end || full == data->philo_nb) // add || !running
			return (NULL);
		// if (run_check(data, 0, 0, -1))
		// 	return (NULL);
		while (++i < data->philo_nb)
		{
			// running = get_long(&data->run_mtx, &data->running);
			// if (running == -1)
			// 	return (NULL);
			// else if (!running)
			// 	break ;
			// if (my_usleep(500))
			// 	return (NULL);
			last_meal = get_long(&data->philos[i].lstml_mx,
					&data->philos[i].last_meal);
			if (last_meal == -1)
				return (NULL);
			if (my_gettime("MIL_SEC") - last_meal > data->die_time +1)
			{
				// printf("dieee\n");
				if (print_status(data, data->philos + i, "DIE", 0)
					|| set_int(&data->end_mtx, &data->end, 1))
					return (NULL);
				return (NULL);
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
	// long long	n;
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
	while (++i < data->philo_nb) 
		pthread_mutex_destroy(&data->forks[i].fork);
	pthread_mutex_destroy(&data->end_mtx);
	pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->run_mtx);
	pthread_mutex_destroy(&data->strt_mtx);
	pthread_mutex_destroy(&data->full_mtx);
	pthread_mutex_destroy(&data->stat_mtx);
}

void	leaks()
{
	system("leaks philo");
}

int	main(int ac, char **av)
{
	t_data	data;
	void	*monit;
	int		i;

	i = -1;
	// atexit(leaks);
	if (ac != 5 && ac != 6)
		return (1);
	if (parse_input(av))
		return (2);
	if (data_init(&data, av))
		return (3);
	if (!data.meals_nb)
		return (clean_stuff(&data), 0);
	threads_init(&data);
	pthread_join(data.monitor1, &monit);
	while (++i < data.philo_nb)
		if (pthread_detach(data.philos[i].thread))
			return (clean_stuff(&data), 5);
	clean_stuff(&data);
	return (0);
}
