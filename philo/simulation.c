/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghla <aaghla@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 20:15:22 by aaghla            #+#    #+#             */
/*   Updated: 2024/07/16 19:12:04 by aaghla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_data *data, t_philo *philo, char *stat)
{
	pthread_mutex_lock(&data->print);
	if (!get_bool(&data->end_mtx, &data->end))
	{
		if (!ft_cmp(stat, "FORK") && !get_bool(&data->end_mtx, &data->end))
			printf("%lld %d has taken a fork\n",
				my_gettime("MIL_SEC") - data->simul_strt, philo->id);
		else if (!ft_scmp(stat, "EAT") && !get_bool(&data->end_mtx, &data->end))
			printf("%lld %d is eating\n",
				my_gettime("MIL_SEC") - data->simul_strt, philo->id);
		else if (!ft_cmp(stat, "SLP") && !get_bool(&data->end_mtx, &data->end))
			printf("%lld %d is sleeping\n",
				my_gettime("MIL_SEC") - philo->data->simul_strt, philo->id);
		else if (!ft_cmp(stat, "THNK") && !get_bool(&data->end_mtx, &data->end))
			printf("%lld %d is thinking\n",
				my_gettime("MIL_SEC") - philo->data->simul_strt, philo->id);
		else if (!ft_cmp(stat, "DIE") && !get_bool(&data->end_mtx, &data->end))
			printf("%lld %d died\n", my_gettime("MIL_SEC") - data->simul_strt,
				philo->id);
		else
			clean_exit("check print_status() args\n");
	}
	pthread_mutex_unlock(&data->print);
}

static void	eat(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&philo->first_fork->fork);
	print_status(data, philo, "FORK");
	pthread_mutex_lock(&philo->second_fork->fork);
	set_long(&philo->last_meal_mtx, &philo->last_meal, my_gettime("MIL_SEC"));
	print_status(data, philo, "FORK");
	print_status(data, philo, "EAT");
	philo->meals++;
	if (data->meals_nb > 0 && philo->meals == data->meals_nb)
		philo->full = true;
	my_usleep(data->eat_time);
	pthread_mutex_unlock(&philo->second_fork->fork);
	pthread_mutex_unlock(&philo->first_fork->fork);
}

static void	think(t_data *data, t_philo *philo)
{
	long long	think;

	print_status(data, philo, "THNK");
	if (data->philo_nb % 2 == 0)
		return ;
	think = philo->data->eat_time * 2 - philo->data->slp_time;
	if (think > 0)
		my_usleep(think * 0.45);
}

static void	sync_philos(t_philo *philo)
{
	long long	think;

	if (philo->data->philo_nb % 2 == 0)
	{
		if (philo->id % 2 == 0)
			my_usleep(3e4);
	}
	else
	{
		if (philo->id % 2)
		{
			think = philo->data->eat_time * 2 - philo->data->slp_time;
			if (think > 0)
				my_usleep(think * 0.45);
		}
	}
}

void	*simulation(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	increase_long(&data->run_mtx, &data->running);
	wait_rest(philo->data);
	if (!data->meals_nb)
		philo->full = true;
	sync_philos(philo);
	set_long(&philo->last_meal_mtx, &philo->last_meal, my_gettime("MIL_SEC"));
	while (!get_bool(&data->end_mtx, &data->end))
	{
		if (philo->full)
			break ;
		eat(philo);
		print_status(data, philo, "SLP");
		if (!get_bool(&data->end_mtx, &data->end))
			my_usleep(philo->data->slp_time);
		think(data, philo);
	}
	decrease_long(&data->run_mtx, &data->running);
	return (NULL);
}
