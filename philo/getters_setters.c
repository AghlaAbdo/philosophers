/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters_setters.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghla <aaghla@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 12:47:40 by thedon            #+#    #+#             */
/*   Updated: 2024/07/17 11:18:10 by aaghla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_bool(t_mtx *mtx, int *value)
{
	int	res;

	if (pthread_mutex_lock(mtx))
		return (-1);
	res = *value;
	if (pthread_mutex_unlock(mtx))
		return (-1);
	return (res);
}

int	set_bool(t_mtx *mtx, int *set, int value)
{
	if (pthread_mutex_lock(mtx))
		return (-1);
	*set = value;
	if (pthread_mutex_unlock(mtx))
		return (-1);
	return (0);
}

long long	get_long(t_mtx *mtx, long long *value)
{
	long long	res;

	if (pthread_mutex_lock(mtx))
		return (-1);
	res = *value;
	if (pthread_mutex_unlock(mtx))
		return (-1);
	return (res);
}

int	set_long(t_mtx *mtx, long long *set, long long value)
{
	if (pthread_mutex_lock(mtx))
		return (-1);
	*set = value;
	if (pthread_mutex_unlock(mtx))
		return (-1);
	return (0);
}

int	increase_long(t_mtx *mtx, long long *num, char *op)
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
