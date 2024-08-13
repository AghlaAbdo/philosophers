/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghla <aaghla@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 20:03:29 by thedon            #+#    #+#             */
/*   Updated: 2024/08/13 10:56:10 by aaghla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_len(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_cmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

long long	ft_atol(const char *str)
{
	int		i;
	int		sign;
	long	n;

	i = 0;
	sign = 1;
	n = 0;
	while (str[i] && (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)))
		i++;
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
	{
		sign = -sign;
		i++;
	}
	while (str[i] && str[i] >= 48 && str[i] <= 57)
	{
		n = n * 10 + str[i++] - 48;
		if (n < 0 && sign == 1)
			return (-1);
		else if (n < 0 && sign == -1)
			return (0);
	}
	return (n * sign);
}

long long	my_gettime(char *time)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (-1);
	if (!ft_cmp(time, "SEC"))
		return (tv.tv_sec + (tv.tv_usec / 1e6));
	else if (!ft_cmp(time, "MIL_SEC"))
		return ((tv.tv_sec * 1e3) + (tv.tv_usec / 1e3));
	else if (!ft_cmp(time, "MIC_SEC"))
		return ((tv.tv_sec * 1e6) + tv.tv_usec);
	else
		return (-1);
}

int	my_usleep(long long usec)
{
	long long	start;
	long long	curr;

	start = my_gettime("MIC_SEC");
	if (start == -1)
		return (1);
	while (1)
	{
		curr = my_gettime("MIC_SEC");
		if (curr == -1)
			return (printf("curr ic -2?\n"), 1);
		if (curr - start >= usec)
			break ;
		usleep(100);
	}
	return (0);
}
