/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghla <aaghla@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 08:18:56 by aaghla            #+#    #+#             */
/*   Updated: 2024/08/13 09:31:34 by aaghla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	increase_int(t_mtx *mtx, int *num, char *op)
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
