/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 21:40:56 by sguan             #+#    #+#             */
/*   Updated: 2024/11/25 18:09:09 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_len(long nb)
{
	size_t	count;

	if (nb <= 0)
		count = 1;
	else
		count = 0;
	while (nb != 0)
	{
		count++;
		nb = nb / 10;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	size_t	len;
	long	num;
	char	*result;

	len = count_len((long)n);
	num = n;
	result = malloc((len + 1) * sizeof(char));
	if (!result)
		return (NULL);
	result[len] = '\0';
	if (num < 0)
	{
		num = -num;
		result[0] = '-';
	}
	while (len > 0 && num != 0)
	{
		result[len - 1] = num % 10 + '0';
		num /= 10;
		len--;
	}
	if (n == 0)
		result[0] = '0';
	return (result);
}
