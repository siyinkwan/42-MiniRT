/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:47:29 by sguan             #+#    #+#             */
/*   Updated: 2025/09/14 19:12:48 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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

bool	validate_cone_params(t_cone *co, char **tokens, double angle_degrees)
{
	if (!parse_vec3(tokens[1], &co->apex) || !parse_vec3(tokens[2], &co->axis)
		|| angle_degrees <= 0.0 || angle_degrees >= 90.0 || co->height <= 0.0
		|| !parse_color(tokens[5], &co->material.color)
		|| !is_normalized(co->axis))
		return (false);
	co->angle = angle_degrees * PI / 180.0;
	return (true);
}

bool	has_invalid_chars(char *line)
{
    int	i;

    i = 0;
    while (line[i])
    {
        if (!ft_isdigit(line[i]) && !ft_isalpha(line[i])
            && line[i] != ' ' && line[i] != '\t' && line[i] != ','
            && line[i] != '.' && line[i] != '-' && line[i] != '+'
			&& line[i] != '\n')
            return (true);
        i++;
    }
    return (false);
}

bool	parse_color(char *token, t_vec3 *color)
{
	char	**rgb_arr;
	int		r;
	int		g;
	int		b;

	rgb_arr = ft_split(token, ',');
	if (!rgb_arr)
		return (printf("Error\nfailed to split color\n"), false);
	if (count_tokens(rgb_arr) != 3)
	{
		free_tokens(rgb_arr);
		return (printf("Error\ninvalid color parameters\n"), false);
	}
	r = ft_atoi(rgb_arr[0]);
	g = ft_atoi(rgb_arr[1]);
	b = ft_atoi(rgb_arr[2]);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
	{
		free_tokens(rgb_arr);
		return (printf("Error\ncolor values must be 0-255\n"), false);
	}
	color->x = r / 255.0;
	color->y = g / 255.0;
	color->z = b / 255.0;
	return (free_tokens(rgb_arr), true);
}

bool	parse_vec3(char *token, t_vec3 *vec)
{
	char	**coords;

	if (!token || !vec)
		return (printf("Error\nnull vector token or pointer\n"), false);
	coords = ft_split(token, ',');
	if (!coords)
		return (printf("Error\nfailed to split vector\n"), false);
	if (count_tokens(coords) != 3)
	{
		free_tokens(coords);
		return (printf("Error\nvector requires 3 coordinates\n"), false);
	}
	vec->x = ft_atof(coords[0]);
	vec->y = ft_atof(coords[1]);
	vec->z = ft_atof(coords[2]);
	free_tokens(coords);
	return (true);
}
