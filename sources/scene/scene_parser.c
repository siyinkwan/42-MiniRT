/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 13:37:26 by sguan             #+#    #+#             */
/*   Updated: 2025/09/11 22:57:32 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_scene	*init_scene(void)
{
	t_scene	*scene;

	scene = malloc(sizeof(t_scene));
	if (!scene)
		return (NULL);
	scene->ambient.ratio = 0.0;
	scene->ambient.color = vec3_create(0.0, 0.0, 0.0);
	scene->camera.position = vec3_create(0.0, 0.0, 0.0);
	scene->camera.forward = vec3_create(0.0, 0.0, -1.0);
	scene->camera.up = vec3_create(0.0, 1.0, 0.0);
	scene->camera.right = vec3_create(1.0, 0.0, 0.0);
	scene->camera.fov = 0.0;
	scene->camera.aspect_ratio = 0.0;
	scene->lights = NULL;
	scene->light_count = 0;
	scene->objects = NULL;
	scene->background = vec3_create(0.0, 0.0, 0.0);
	return (scene);
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

static bool	dispatch_parser(char *id, t_scene *scene, char **tokens)
{
    if (ft_strcmp(id, "A") == 0)
        return (parse_ambient(scene, tokens));
    if (ft_strcmp(id, "C") == 0)
        return (parse_camera(scene, tokens));
    if (ft_strcmp(id, "L") == 0)
        return (parse_light(scene, tokens));
    if (ft_strcmp(id, "sp") == 0)
        return (parse_sphere(scene, tokens));
    if (ft_strcmp(id, "pl") == 0)
        return (parse_plane(scene, tokens));
    if (ft_strcmp(id, "cy") == 0)
        return (parse_cylinder(scene, tokens));
    if (ft_strcmp(id, "co") == 0)
        return (parse_cone(scene, tokens));
    printf("Error: Unknown identifier '%s'\n", id);
    return (false);
}

bool	parse_line(char *line, t_scene *scene)
{
	char	**tokens;
	char	*comment_pos;
	bool	result;
	int		len;

	if (!line || line[0] == '#' || line[0] == '\0')
		return (true);
	comment_pos = ft_strchr(line, '#');
	if (comment_pos)
		*comment_pos = '\0';
	len = ft_strlen(line);
	while (len > 0 && (line[len - 1] == ' ' || line[len - 1] == '\t' 
		|| line[len - 1] == '\n' || line[len - 1] == '\r'))
		line[--len] = '\0';
	if (len == 0)
		return (true);
	if (has_invalid_chars(line))
		return (printf("Error: Invalid characters in line\n"), false);
	tokens = ft_split(line, ' ');
	if (!tokens || !tokens[0])
		return (free_tokens(tokens), false);
	result = dispatch_parser(tokens[0], scene, tokens);
	return (free_tokens(tokens), result);
}

int	parse_scene(char *filename, t_scene *scene)
{
	int		fd;
	char	*line;
	int		line_num;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return(error_exit("Cannot open file"), -1);
	line_num = 0;
	line = get_next_line(fd);
	while (line)
	{
		line_num++;
		if (!parse_line(line, scene))
		{
			printf("Parse error at line %d: %s", line_num, line);
			free(line);
			close(fd);
			return (-1);
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
