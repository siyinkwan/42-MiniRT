/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:37:46 by sguan             #+#    #+#             */
/*   Updated: 2024/11/20 21:03:56 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	total_size;
	size_t	i;
	void	*result;

	if (size != 0 && nmemb > ((size_t) -1 / size))
		return (NULL);
	total_size = nmemb * size;
	result = malloc(total_size);
	if (!result)
		return (NULL);
	i = 0;
	while (i < total_size)
	{
		((unsigned char *)result)[i] = 0;
		i++;
	}
	return (result);
}
