/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghla <aaghla@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 20:15:22 by aaghla            #+#    #+#             */
/*   Updated: 2024/07/17 12:34:16 by aaghla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	print_status(t_data *data, t_philo *philo, char *stat)
{
	if (pthread_mutex_lock(&data->print))
		return (1);
	if (!get_bool(&data->end_mtx, &data->end))
	{
		if (!ft_cmp(stat, "FORK") && !get_bool(&data->end_mtx, &data->end))
			printf("%lld %d has taken a fork\n",
				my_gettime("MIL_SEC") - data->simul_strt, philo->id);
		else if (!ft_cmp(stat, "EAT") && !get_bool(&data->end_mtx, &data->end))
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
	}
	if (pthread_mutex_unlock(&data->print))
		return (1);
	return (0);
}

static int	eat(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	if (pthread_mutex_lock(&philo->first_fork->fork))
		return (1);
	if (print_status(data, philo, "FORK"))
		return (1);
	if (pthread_mutex_lock(&philo->second_fork->fork)
		|| set_long(&philo->lstml_mx, &philo->last_meal, my_gettime("MIL_SEC")))
		return (1);
	if (print_status(data, philo, "FORK") || print_status(data, philo, "EAT"))
		return (1);
	philo->meals++;
	if (data->meals_nb > 0 && philo->meals == data->meals_nb)
		philo->full = 1;
	if (my_usleep(data->eat_time))
		return (1);
	if (pthread_mutex_unlock(&philo->second_fork->fork))
		return (1);
	if (pthread_mutex_unlock(&philo->first_fork->fork))
		return (1);
	return (0);
}

static int	think(t_data *data, t_philo *philo)
{
	long long	think;

	if (print_status(data, philo, "THNK"))
		return (1);
	if (data->philo_nb % 2 == 0)
		return (0);
	think = philo->data->eat_time * 2 - philo->data->slp_time;
	if (think > 0)
		if (my_usleep(think * 0.45))
			return (1);
	return (0);
}

static int	sync_philos(t_philo *philo)
{
	long long	think;

	if (philo->data->philo_nb % 2 == 0)
	{
		if (philo->id % 2 == 0)
			if (my_usleep(3e4))
				return (1);
	}
	else
	{
		if (philo->id % 2)
		{
			think = philo->data->eat_time * 2 - philo->data->slp_time;
			if (think > 0)
				if (my_usleep(think * 0.45))
					return (1);
		}
	}
	return (0);
}

int	simul_init(void *arg, t_philo *philo, t_data *data)
{
	if (increase_long(&data->run_mtx, &data->running, "++") == -1
		|| wait_rest(philo->data) || sync_philos(philo)
		|| set_long(&philo->lstml_mx, &philo->last_meal,
			my_gettime("MIL_SEC")) == -1)
		return (1);
	if (!data->meals_nb)
		philo->full = 1;
	return (0);
}

void	*simulation(void *arg)
{
	t_philo	*philo;
	t_data	*data;
	int		end;

	philo = (t_philo *)arg;
	data = philo->data;
	if (simul_init(arg, philo, data))
		return (NULL);
	while (1)
	{
		end = get_bool(&data->end_mtx, &data->end);
		if (end || philo->full)
			break ;
		if (eat(philo) || print_status(data, philo, "SLP") || end == -1)
			return (NULL);
		if (my_usleep(philo->data->slp_time))
			return (NULL);
		if (think(data, philo))
			return (NULL);
	}
	if (increase_long(&data->run_mtx, &data->running, "--") == -1)
		return (NULL);
	return (arg);
}
