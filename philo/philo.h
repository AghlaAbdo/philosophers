/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghla <aaghla@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 19:19:14 by thedon            #+#    #+#             */
/*   Updated: 2024/08/15 11:19:34 by aaghla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <limits.h>
# include <sys/time.h>

# define MAX_PHLOS	300

typedef struct s_data	t_data;
typedef pthread_mutex_t	t_mtx;

typedef struct s_fork
{
	t_mtx		fork;
	long long	id;
}	t_fork;

typedef struct s_philo
{
	int			id;
	int			meals;
	long long	last_meal;
	t_mtx		lstml_mx;
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
	long long	simul_strt;
	long long	running;
	int			full;
	int			meals_nb;
	int			end;
	pthread_t	monitor1;
	pthread_t	monitor2;
	t_mtx		full_mtx;
	t_mtx		strt_mtx;
	t_mtx		print;
	t_mtx		end_mtx;
	t_mtx		run_mtx;
	t_fork		forks[MAX_PHLOS];
	t_philo		philos[MAX_PHLOS];
}	t_data;

int			data_init(t_data *data, char **av);
int			threads_init(t_data *data);
int			prnt_stat(t_data *data, t_philo *philo,
				char *status, long long strt);
long long	my_gettime(char *time);
int			my_usleep(long long usec);
long long	ft_atol(const char *str);
int			ft_cmp(char *s1, char *s2);
int			ft_len(char *str);
void		*simulation(void *arg);
int			simul_init(t_philo *philo, t_data *data);
void		*monitor(void *arg);
int			wait_rest(t_data *data);
int			sync_philos(t_philo *philo);
void		clean_stuff(t_data *data);

// getter and setters
int			get_int(t_mtx *mtx, int *value);
int			set_int(t_mtx *mtx, int *set, int value);
int			set_long(t_mtx *mtx, long long *set, long long value);
int			increase_int(t_mtx *mtx, int *num, char *op);
int			increase_long(t_mtx *mtx, long long *num, char *op);
long long	get_long(t_mtx *mtx, long long *value);

#endif