/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thedon <thedon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 19:21:28 by thedon            #+#    #+#             */
/*   Updated: 2024/07/03 20:20:44 by thedon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean_exit(char *err)
{
	printf("%s\n", err);
	ft_malloc(0, 1);
	exit(82);
}

void	wait_rest(t_data *data)
{
	while (get_long(&data->run_mtx, &data->running) != data->philo_nb)
		;
}

void	my_usleep(long long usec, t_data *data)
{
	long long	start;
	long long	elapsed;
	long long	rem;

	start = my_gettime("MIC_SEC");
	while (my_gettime("MIC_SEC") - start < usec)
	{
		if (get_bool(&data->end_mtx, &data->end))
			break ;
		elapsed = my_gettime("MIC_SEC") - start;
		rem = usec - elapsed;
		if (rem > 1e3)
			usleep(rem / 2);
		else
		{
			while(my_gettime("MIC_SEC") - start < usec)
				;
		}
	}
}

void	*monitor(void *arg)
{
	t_data	*data;
	t_philo	*philo;
	int		i;
	long long	last_meal;

	data = (t_data *)arg;
	while (!get_bool(&data->end_mtx, &data->end) && get_long(&data->run_mtx, &data->running))
	{
		i = -1;
		while (++i < data->philo_nb && get_long(&data->run_mtx, &data->running))
		{
			philo = data->philos + i;
			usleep(2e3);
			last_meal = get_long(&data->philos[i].last_meal_mtx, &data->philos[i].last_meal);
			// if (get_bool(&philo->dead_mtx, &philo->is_dead))
			if (my_gettime("MIL_SEC") - last_meal > data->die_time)
			{
				// printf("philo last meal: %lld time: %lld %d died\n",my_gettime("MIL_SEC") - last_meal , my_gettime("MIL_SEC") - data->simul_strt,
					// data->philos[i].id);
				// printf("\t%lld monitor philo %d last meal: %lld\n", my_gettime("MIL_SEC") - data->simul_strt, data->philos[i].id, last_meal);
				// write(1, "is dead\n", 8);
				// printf("%lld %d died\n", my_gettime("MIL_SEC") - data->simul_strt, data->philos[i].id);
				print_status(data, data->philos + i, "DIE");
				set_bool(&data->end_mtx, &data->end, true);
				break;
			}
		}
	}
	
}

int main(int ac, char **av)
{
	t_data data;
	int		i;

	i = -1;
	if (ac != 5 && ac != 6)
		return (1);
	data_init(&data, av);
	threads_init(&data);

	while (++i < data.philo_nb)
	{
		if (pthread_join(data.philos[i].thread, NULL))
			clean_exit("error joining thread");
	}
	if (pthread_join(data.monitor, NULL))
		clean_exit("error joining thread");
	return (0);
}