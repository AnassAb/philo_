/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabidar <aabidar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 13:41:05 by aabidar           #+#    #+#             */
/*   Updated: 2024/04/25 13:41:16 by aabidar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isspace(char c)
{
	if (c == ' ' || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

int	ft_isdigit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	is_invalid_number(char *s)
{
	int	len;

	len = 0;
	while (ft_isspace(*s))
		s++;
	if ((*s == '-' || *s == '+') && ft_isdigit(*(s + 1)))
		s++;
	if (!ft_isdigit(*s))
		return (1);
	while (*s == '0')
		s++;
	while (ft_isdigit(*s))
	{
		len++;
		s++;
	}
	while (ft_isspace(*s))
		s++;
	if (*s || len > 10)
		return (1);
	return (0);
}

int	ft_atoi(char *s)
{
	int		sign;
	long	res;

	sign = 1;
	res = 0;
	if (is_invalid_number(s))
		err_atoi(s);
	while (ft_isspace(*s))
		s++;
	if (*s == '-' || *s == '+')
	{
		if (*s == '-')
			sign = -1;
		s++;
	}
	while (ft_isdigit(*s))
	{
		res = res * 10 + *s - '0';
		s++;
	}
	if ((res * sign) > 2147483647 || (res * sign) < -2147483648)
		err_atoi(s);
	return (res * sign);
}
