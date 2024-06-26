/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thedon <thedon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 19:19:14 by thedon            #+#    #+#             */
/*   Updated: 2024/06/26 22:07:54 by thedon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <limits.h>
# include <stdbool.h>

typedef struct s_data t_data;
typedef pthread_mutex_t t_mtx;

typedef struct s_addr
{
	void			*addr;
	struct s_addr	*next;
}	t_addr;

typedef struct s_fork
{
	t_mtx		fork;
	long long	id;
}	t_fork;

typedef struct s_philo
{
	int			id;
	int			meals;
	bool		full;
	pthread_t	thread;
	t_data		*data;
	t_fork		first_fork;
	t_fork		second_fork;
	
}	t_philo;

typedef struct s_data
{
	long long	philo_nb;
	long long	die_time;
	long long	eat_time;
	long long	slp_time;
	long long	meals_nb;
	long long	ready_thrds;
	long long	simul_strt;
	t_mtx		ready;
	t_fork		*forks;
	t_philo		*philos;
}	t_data;

long long	ft_atol(const char *str);
void		*ft_malloc(size_t size, int mode);

#endif