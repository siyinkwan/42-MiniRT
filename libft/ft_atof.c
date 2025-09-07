/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 16:03:55 by sguan             #+#    #+#             */
/*   Updated: 2025/09/06 18:46:15 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	skip_whitespace(const char *str)
{
	int	i;

	i = 0;
	while ((str[i] >= '\t' && str[i] <= '\r') || str[i] == ' ')
		i++;
	return (i);
}

static int	parse_sign(const char *str, int *pos)
{
	if (str[*pos] == '-')
		return ((*pos)++, -1);
	if (str[*pos] == '+')
		(*pos)++;
	return (1);
}

double	ft_atof(const char *nptr)
{
	int		i;
	int		sign;
	double	result;
	double	divisor;

	i = skip_whitespace(nptr);
	sign = parse_sign(nptr, &i);
	result = 0.0;
	divisor = 1.0;
	while (nptr[i] >= '0' && nptr[i] <= '9')
		result = result * 10 + (nptr[i++] - '0');
	if (nptr[i] == '.')
	{
		i++;
		while (nptr[i] >= '0' && nptr[i] <= '9')
		{
			divisor *= 10;
			result += (nptr[i++] - '0') / divisor;
		}
	}
	return (result * sign);
}
