/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters_setters.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thedon <thedon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 12:47:40 by thedon            #+#    #+#             */
/*   Updated: 2024/07/02 22:19:45 by thedon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	get_bool(t_mtx *mtx, bool *value)
{
	bool	res;
	
	pthread_mutex_lock(mtx);
	res = *value;
	pthread_mutex_unlock(mtx);
	return (res);
}

void	set_bool(t_mtx *mtx, bool *set, bool value)
{
	pthread_mutex_lock(mtx);
	*set = value;
	pthread_mutex_unlock(mtx);
}

long long	get_long(t_mtx *mtx, long long *value)
{
	long long	res;

	pthread_mutex_lock(mtx);
	res = *value;
	pthread_mutex_unlock(mtx);
	return (res);
}

void	set_long(t_mtx *mtx, long long *set, long long value)
{
	pthread_mutex_lock(mtx);
	*set = value;
	pthread_mutex_unlock(mtx);
}

void	increase_long(t_mtx *mtx, long long *increase)
{
	pthread_mutex_lock(mtx);
	(*increase)++;
	pthread_mutex_unlock(mtx);
}

void	decrease_long(t_mtx *mtx, long long *decrease)
{
	pthread_mutex_lock(mtx);
	(*decrease)--;
	pthread_mutex_unlock(mtx);
	
}