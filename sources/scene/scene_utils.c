/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 18:29:36 by sguan             #+#    #+#             */
/*   Updated: 2025/09/07 18:02:12 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	error_exit(char *msg)
{
	printf("Error: %s\n", msg);
	exit(1);
}

void	free_tokens(char **tokens)
{
    int	i;

    if (!tokens)
        return;
    
    i = 0;
    while (tokens[i])
    {
        free(tokens[i]);
        i++;
    }
    free(tokens);
}

int	count_tokens(char **tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return (0);
	while (tokens[i])
		i++;
	return (i);
}

bool	parse_color(char *token, t_vec3 *color)
{
	char	**rgb_arr;
	int		r;
	int		g;
	int		b;

	rgb_arr = ft_split(token, ',');
	if (!rgb_arr)
		return (printf("Error: failed to split color\n"), false);
	if (count_tokens(rgb_arr) != 3)
	{
		free_tokens(rgb_arr);
		return (printf("Error: invalid color parameters\n"), false);
	}
	r = ft_atoi(rgb_arr[0]);
	g = ft_atoi(rgb_arr[1]);
	b = ft_atoi(rgb_arr[2]);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
	{
		free_tokens(rgb_arr);
		return (printf("Error: color values must be 0-255\n"), false);
	}
	color->x = r;
	color->y = g;
	color->z = b;
	return (free_tokens(rgb_arr), true);
}

bool	parse_vec3(char *token, t_vec3 *vec)
{
    char	**coords;

    if (!token || !vec)
        return (printf("Error: null vector token or pointer\n"), false);

    coords = ft_split(token, ',');
    if (!coords)
        return (printf("Error: failed to split vector\n"), false);
    
    if (count_tokens(coords) != 3)
    {
        free_tokens(coords);
        return (printf("Error: vector requires 3 coordinates\n"), false);
    }

    vec->x = ft_atof(coords[0]);
    vec->y = ft_atof(coords[1]);
    vec->z = ft_atof(coords[2]);

    free_tokens(coords);
    return (true);
}
