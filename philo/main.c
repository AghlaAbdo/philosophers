/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghla <aaghla@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 19:21:28 by thedon            #+#    #+#             */
/*   Updated: 2024/08/12 12:38:23 by aaghla           ###   ########.fr       */
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

void	set_error(t_data *data, int err)
{
	data->status = err;
}

int	run_check(t_data *data, long long last_meal, long long lng, int i)
{
	while (++i < data->philo_nb)
	{
		// lng = get_long(&data->run_mtx, &data->running);
		// if (lng == -1)
		// 	return (1);
		// else if (!lng)
		// 	break ;
		// if (my_usleep(2e3))
		// 	return (1);
		last_meal = get_long(&data->philos[i].lstml_mx,
				&data->philos[i].last_meal);
		if (last_meal == -1)
			return (1);
		if (my_gettime("MIL_SEC") - last_meal > data->die_time)
		{
			printf("dieee\n");
			if (print_status(data, data->philos + i, "DIE", 0)
				|| set_bool(&data->end_mtx, &data->end, 1))
				return (1);
			break ;
		}
	}
	return (0);
}

// void	*monitor2(void *arg)
// {
// 	t_data		*data;
// 	long long	running;
// 	long long	last_meal;
// 	int			end;
// 	int			i;

// 	data = (t_data *)arg;
// 	i = 50;
// 	while (1)
// 	{
// 		// end = get_bool(&data->end_mtx, &data->end);
// 		// running = get_long(&data->run_mtx, &data->running);
// 		// if (end == -1 || running == -1)
// 		// 	return (NULL);
// 			// return (set_error(data, 2), NULL);
// 		// if (end) // add || !running
// 		// 	return (arg);
// 		// if (run_check(data, 0, 0, -1))
// 		// 	return (NULL);
// 		while (++i < data->philo_nb)
// 		{
// 			// lng = get_long(&data->run_mtx, &data->running);
// 			// if (lng == -1)
// 			// 	return (1);
// 			// else if (!lng)
// 			// 	break ;
// 			// if (my_usleep(2e3))
// 			// 	return (1);
// 			last_meal = get_long(&data->philos[i].lstml_mx,
// 					&data->philos[i].last_meal);
// 			if (last_meal == -1)
// 				return (NULL);
// 			if (my_gettime("MIL_SEC") - last_meal >= data->die_time)
// 			{
// 				printf("dieee\n");
// 				if (print_status(data, data->philos + i, "DIE", 0)
// 					|| set_bool(&data->end_mtx, &data->end, 1))
// 					return (NULL);
// 				return (arg);
// 			}
// 		}
// 		i = -1;
// 			// return (set_error(data, 2), NULL);
// 	}
// 	return (arg);
// }
void	*monitor1(void *arg)
{
	t_data		*data;
	long long	running;
	long long	last_meal;
	int			end;
	int			i;

	data = (t_data *)arg;
	while (1)
	{
		i = -1;
		end = get_bool(&data->end_mtx, &data->end);
		running = get_long(&data->run_mtx, &data->running);
		if (end == -1 || running == -1)
			return (NULL);
			// return set_error(data, 2), NULL);
		if (end || !running) // add || !running
			return (arg);
		// if (run_check(data, 0, 0, -1))
		// 	return (NULL);
		while (++i < data->philo_nb)
		{
			// running = get_long(&data->run_mtx, &data->running);
			// if (running == -1)
			// 	return (NULL);
			// else if (!running)
			// 	break ;
			if (my_usleep(500))
				return (NULL);
			last_meal = get_long(&data->philos[i].lstml_mx,
					&data->philos[i].last_meal);
			if (last_meal == -1)
				return (NULL);
			if (my_gettime("MIL_SEC") - last_meal > data->die_time +1)
			{
				// printf("dieee\n");
				if (print_status(data, data->philos + i, "DIE", 0)
					|| set_bool(&data->end_mtx, &data->end, 1))
					return (NULL);
				return (arg);
			}
		}
			// return (set_error(data, 2), NULL);
	}
	return (arg);
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
	while (++i < data->philo_nb) 
		pthread_mutex_destroy(&data->forks[i].fork);
	pthread_mutex_destroy(&data->end_mtx);
	pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->run_mtx);
	pthread_mutex_destroy(&data->strt_mtx);
	free(data->forks);
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
		return (1);
	data_init(&data, av);
	// printf("in print:\ndata->end add: %p\nsimul_start add: %p\ndata->meals_nb: %p\n",
	// 	&data.end, &data.simul_strt, &data.meals_nb);
	// printf("die time: %p\neat_time: %p\nslp_time: %p\nrunning: %p\n", &data.die_time,
	// 	&data.eat_time, &data.slp_time, &data.running);
	threads_init(&data);
	if (pthread_join(data.monitor1, &monit) || !monit)
		return (1);
	if (data.philos->full)
	{
		while (++i < data.philo_nb)
			if (pthread_join(data.philos[i].thread, NULL))
				return (1);
		// printf("bef free?\n");
		free(data.philos);
	}
	else
	{
		while (++i < data.philo_nb)
			if (pthread_detach(data.philos[i].thread))
				return (1);
	}
	clean_stuff(&data);
	return (data.status);
}
