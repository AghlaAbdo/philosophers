/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghla <aaghla@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 20:15:22 by aaghla            #+#    #+#             */
/*   Updated: 2024/08/12 12:33:45 by aaghla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// long long	get_start(t_data *data)
// {
// 	while (data->simul_strt == -1)
// 		;

// }

int print_status(t_data *data, t_philo *philo, char *stat, long long strt)
{
	if (pthread_mutex_lock(&data->print))
		return (1);
	if (!get_bool(&data->end_mtx, &data->end))
	{
		strt = get_long(&data->strt_mtx, &data->simul_strt);
		if (!ft_cmp(stat, "FORK"))
			printf("%lld %d has taken a fork\n",
				   my_gettime("MIL_SEC") - strt, philo->id);
		else if (!ft_cmp(stat, "EAT"))
			printf("%lld %d is eating\n",
				   my_gettime("MIL_SEC") - strt, philo->id);
		else if (!ft_cmp(stat, "SLP"))
			printf("%lld %d is sleeping\n",
				   my_gettime("MIL_SEC") - strt, philo->id);
		else if (!ft_cmp(stat, "THNK"))
			printf("%lld %d is thinking\n",
				   my_gettime("MIL_SEC") - strt, philo->id);
		else if (!ft_cmp(stat, "DIE"))
			printf("%lld %d died\n", my_gettime("MIL_SEC") - strt,
				   philo->id);
	}
	if (pthread_mutex_unlock(&data->print))
		return (1);
	return (0);
}

static int eat(t_philo *philo)
{
	t_data *data;

	data = philo->data;
	if (pthread_mutex_lock(&philo->first_fork->fork))
		return (1);
	if (print_status(data, philo, "FORK", 0))
		return (1);
	if (pthread_mutex_lock(&philo->second_fork->fork) || set_long(&philo->lstml_mx, &philo->last_meal, my_gettime("MIL_SEC")))
		return (1);
	if (print_status(data, philo, "FORK", 0) || print_status(data, philo, "EAT", 0))
		return (1);
	philo->meals++;
	if (data->meals_nb > 0 && philo->meals == data->meals_nb)
		set_bool(&philo->full_mtx, &philo->full, 1);
	if (my_usleep(data->eat_time))
		return (1);
	if (pthread_mutex_unlock(&philo->second_fork->fork))
		return (1);
	if (pthread_mutex_unlock(&philo->first_fork->fork))
		return (1);
	return (0);
}

static int think(t_data *data, t_philo *philo)
{
	long long think;

	if (print_status(data, philo, "THNK", 0))
		return (1);
	if (data->philo_nb % 2 == 0)
		return (0);
	think = philo->data->eat_time * 2 - philo->data->slp_time;
	if (think > 0)
		if (my_usleep(think * 0.45))
			return (1);
	return (0);
}

int sync_philos(t_philo *philo)
{
	long long think;

	// if (philo->data->philo_nb % 2 == 0)
	// {
		if (philo->id % 2 == 0)
			if (my_usleep(1000))
				return (1);
	// }
	// else
	// {
	// 	if (philo->id % 2)
	// 	{
	// 		think = philo->data->eat_time * 2 - philo->data->slp_time;
	// 		if (think > 0)
	// 			if (my_usleep(think * 0.45))
	// 				return (1);
	// 	}
	// }
	return (0);
}

void *simulation(void *arg)
{
	t_philo *philo;
	t_data *data;
	int end;

	philo = (t_philo *)arg;
	data = philo->data;
	if (simul_init(arg, philo, data))
		return (NULL);
	// return (set_error(data, 3), NULL);
	while (1)
	{
		end = get_bool(&data->end_mtx, &data->end);
		if (end || get_bool(&philo->full_mtx, &philo->full))
			break;
		// printf("philo->full add: %p\n", &philo->full);
		if (eat(philo) || print_status(data, philo, "SLP", 0) || end == -1)
			return (NULL);
		// return (set_error(data, 4), NULL);
		// if (get_bool(&philo->full_mtx, &philo->full))
		// 	break;
		// printf("slp_time add: %p\n", &philo->data->slp_time);
		if (my_usleep(philo->data->slp_time))
			return (NULL);
		// return (set_error(data, 5), NULL);
		if (think(data, philo))
			return (NULL);
		// return (set_error(data, 6), NULL);
	}
	usleep(100 * philo->id);
	if (increase_long(&data->run_mtx, &data->running, "--") == -1)
		return (NULL);
	// return (set_error(data, 6), NULL);
	return (NULL);
}
