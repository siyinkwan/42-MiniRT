/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 13:37:26 by sguan             #+#    #+#             */
/*   Updated: 2025/09/05 20:28:23 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

bool	parse_line(char *line, t_scene *scene)
{
	char	**tokens;
	bool	result;

	if (!line || line[0] == "#" || line[0] == '\0')
		return (true);
	result = false;
	tokens = ft_split(line, ' ');
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
	free(tokens);
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
