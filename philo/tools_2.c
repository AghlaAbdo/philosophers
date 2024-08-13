/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghla <aaghla@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 08:18:56 by aaghla            #+#    #+#             */
/*   Updated: 2024/08/13 11:42:31 by aaghla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	increase_int(t_mtx *mtx, int *num, char *op)
{
	if (pthread_mutex_lock(mtx))
		return (-1);
	if (!ft_cmp(op, "++"))
		(*num)++;
	else
		(*num)--;
	if (pthread_mutex_unlock(mtx))
		return (-1);
	return (0);
}

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

void	clean_stuff(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->philo_nb)
	{
		pthread_mutex_destroy(&data->forks[i].fork);
		pthread_mutex_destroy(&data->philos[i].lstml_mx);
	}
	pthread_mutex_destroy(&data->end_mtx);
	pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->run_mtx);
	pthread_mutex_destroy(&data->strt_mtx);
	pthread_mutex_destroy(&data->full_mtx);
	pthread_mutex_destroy(&data->stat_mtx);
}
