/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thedon <thedon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 20:15:22 by thedon            #+#    #+#             */
/*   Updated: 2024/07/03 20:22:41 by thedon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_data *data, t_philo *philo, char *status)
{
	pthread_mutex_lock(&data->print);
	if (!get_bool(&data->end_mtx, &data->end))
	{
		if (!ft_strcmp(status, "FORK_1") && !get_bool(&data->end_mtx, &data->end))
			printf("%lld %d has taken a first fork: %lld\n",my_gettime("MIL_SEC") - data->simul_strt,
					philo->id, philo->first_fork->id);
		else if (!ft_strcmp(status, "FORK_2") && !get_bool(&data->end_mtx, &data->end))
			printf("%lld %d has taken second fork: %lld\n",my_gettime("MIL_SEC") - data->simul_strt,
					philo->id, philo->second_fork->id);
		else if (!ft_strcmp(status, "EAT") && !get_bool(&data->end_mtx, &data->end))
			printf("%lld %d is eating\n",my_gettime("MIL_SEC") - data->simul_strt,
					philo->id);
		else if (!ft_strcmp(status, "SLEEP") && !get_bool(&data->end_mtx, &data->end))
			printf("%lld %d is sleeping\n",my_gettime("MIL_SEC") - philo->data->simul_strt,
				philo->id);
		else if (!ft_strcmp(status, "THINK") && !get_bool(&data->end_mtx, &data->end))
			printf("%lld %d is thinking\n",my_gettime("MIL_SEC") - philo->data->simul_strt,
				philo->id);
		else if (!ft_strcmp(status, "DIE") && !get_bool(&data->end_mtx, &data->end))
			printf("%lld %d died\n", my_gettime("MIL_SEC") - data->simul_strt, philo->id);
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
	print_status(data, philo, "FORK_1");
	pthread_mutex_lock(&philo->second_fork->fork);
	set_long(&philo->last_meal_mtx, &philo->last_meal, my_gettime("MIL_SEC"));
	// printf("\t%lld philo %d last meal: %lld\n",my_gettime("MIL_SEC") - data->simul_strt, philo->id, my_gettime("MIL_SEC"));
	print_status(data, philo, "FORK_2");
	print_status(data, philo, "EAT");
	// if (my_gettime("MIL_SEC") - philo->last_meal > data->die_time)
	// 	set_bool(&philo->dead_mtx, &philo->is_dead, true);
	philo->meals++;
	if (data->meals_nb > 0 && philo->meals == data->meals_nb)
		philo->full = true;
	// if (!get_bool(&data->end_mtx, &data->end))
		usleep(data->eat_time);
	// set_long(&philo->last_meal_mtx, &philo->last_meal, my_gettime("MIL_SEC"));
	pthread_mutex_unlock(&philo->second_fork->fork);
	pthread_mutex_unlock(&philo->first_fork->fork);
}

static void	think(t_data *data, t_philo *philo)
{
	long long	think;

	print_status(data, philo, "THINK");
	if (data->philo_nb % 2 == 0)
		return ;
	think = philo->data->eat_time * 2 - philo->data->slp_time;
	if (think > 0)
		usleep(think * 0.45);
}

static void	sync_philos(t_philo *philo)
{
	long long	think;
	if (philo->data->philo_nb % 2 == 0)
	{
		if (philo->id % 2 == 0)
			usleep(3e4);
	}
	else
	{
		if (philo->id % 2)
		{
			think = philo->data->eat_time * 2 - philo->data->slp_time;
			if (think > 0)
			{
				usleep(think * 0.45);
			}
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
			break;
		eat(philo);
		print_status(data, philo, "SLEEP");
		if (!get_bool(&data->end_mtx, &data->end))
			usleep(philo->data->slp_time);
		think(data, philo);
	}
	decrease_long(&data->run_mtx, &data->running);
	return (NULL);
}
