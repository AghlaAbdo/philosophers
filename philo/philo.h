/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghla <aaghla@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 19:19:14 by thedon            #+#    #+#             */
/*   Updated: 2024/07/17 13:21:59 by aaghla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <limits.h>
// # include <stdbool.h>
# include <sys/time.h>

typedef struct s_data	t_data;
typedef pthread_mutex_t	t_mtx;

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
	int			full;
	int			is_dead;
	long long	last_meal;
	t_mtx		lstml_mx;
	t_mtx		dead_mtx;
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
	int			meals_nb;
	int			end;
	pthread_t	monitor;
	t_mtx		print;
	t_mtx		end_mtx;
	t_mtx		run_mtx;
	t_fork		*forks;
	t_philo		*philos;
}	t_data;

int			data_init(t_data *data, char **av);
int			threads_init(t_data *data);
int			print_status(t_data *data, t_philo *philo, char *status);
long long	my_gettime(char *time);
int			my_usleep(long long usec);
long long	ft_atol(const char *str);
void		*ft_malloc(size_t size, int mode);
int			ft_cmp(char *s1, char *s2);
int			ft_len(char *str);
// void		clean_exit(char *err);
void		*simulation(void *arg);
void		*monitor(void *arg);
int			wait_rest(t_data *data);

// getter and setters
int			get_bool(t_mtx *mtx, int *value);
int			set_bool(t_mtx *mtx, int *set, int value);
long long	get_long(t_mtx *mtx, long long *value);
int			set_long(t_mtx *mtx, long long *set, long long value);
int			increase_long(t_mtx *mtx, long long *num, char *op);

#endif