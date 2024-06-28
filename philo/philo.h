/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thedon <thedon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 19:19:14 by thedon            #+#    #+#             */
/*   Updated: 2024/06/28 20:02:51 by thedon           ###   ########.fr       */
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
# include <sys/time.h>

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
	long long	last_meal;
	t_mtx		last_meal_mtx;
	pthread_t	thread;
	t_data		*data;
	t_fork		*first_fork;
	t_fork		*second_fork;
	
}	t_philo;

typedef struct s_data
{
	long long	philo_nb;
	long long	die_time;
	long long	eat_time;
	long long	slp_time;
	int			meals_nb;
	long long	simul_strt;
	bool		ready;
	bool		end;
	pthread_t	monitor;
	t_mtx		print;
	t_mtx		end_mtx;
	t_mtx		ready_mtx;
	t_fork		*forks;
	t_philo		*philos;
}	t_data;

long long	my_gettime(char *time);
long long	ft_atol(const char *str);
void		*ft_malloc(size_t size, int mode);
int			ft_strcmp(char *s1, char *s2);
void		clean_exit(char *err);

// getter and setters
bool		get_bool(t_mtx *mtx, bool *value);
void		set_bool(t_mtx *mtx, bool *set, bool value);
long long	get_long(t_mtx *mtx, long long *value);
void		set_long(t_mtx *mtx, long long *set, long long value);

#endif