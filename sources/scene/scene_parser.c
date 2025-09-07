/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 13:37:26 by sguan             #+#    #+#             */
/*   Updated: 2025/09/06 19:38:09 by sguan            ###   ########.fr       */
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

bool validate_scene(t_scene *scene)
{
	if (!scene)
		return (false);
	if (scene->ambient.ratio < 0)
	{
		printf("Error: Missing ambient light definition\n");
		return (false);
	}
	if (scene->camera.fov <= 0)
	{
		printf("Error: Missing camera definition\n");
		return (false);
	}
	if (!scene->objects)
	{
		printf("Error: Scene must contain at least one object\n");
		return (false);
	}
	return (true);
}

void	free_scene(t_scene *scene)
{
	
}

bool	parse_line(char *line, t_scene *scene)
{
	char	**tokens;
	bool	result;

	if (!line || line[0] == '#' || line[0] == '\0')
		return (true);
	result = false;
	tokens = ft_split(line, ' ');
	if (!tokens || !tokens[0])
		return (free_tokens(tokens), false);
    if (ft_strcmp(tokens[0], "A") == 0)
        result = parse_ambient(scene, tokens);
    else if (ft_strcmp(tokens[0], "C") == 0)
        result = parse_camera(scene, tokens);
    else if (ft_strcmp(tokens[0], "L") == 0)
        result = parse_light(scene, tokens);
    else if (ft_strcmp(tokens[0], "sp") == 0)
        result = parse_sphere(scene, tokens);
    else if (ft_strcmp(tokens[0], "pl") == 0)
        result = parse_plane(scene, tokens);
    else if (ft_strcmp(tokens[0], "cy") == 0)
        result = parse_cylinder(scene, tokens);
    else if (ft_strcmp(tokens[0], "co") == 0)
        result = parse_cone(scene, tokens);
	free_tokens(tokens);
	return (result);
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
