/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_objects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 16:01:03 by sguan             #+#    #+#             */
/*   Updated: 2025/10/29 17:25:39 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

bool	parse_sphere(t_scene *scene, char **tokens)
{
	t_vec3		center;
	double		diameter;
	t_vec3		color;
	t_object	*new_obj;
	int			count;

	count = count_tokens(tokens);
	if (count < 4)
		return (printf("Error\nSphere requires at least 3 parameters\n"), false);
	diameter = ft_atof(tokens[2]);
	if (!parse_vec3(tokens[1], &center) || diameter <= 0.0
		|| !parse_color(tokens[3], &color))
		return (printf("Error\nInvalid sphere parameter\n"), false);
	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		return (printf("Error\nMemory allocation failed\n"), false);
	new_obj->type = OBJECT_SPHERE;
	new_obj->data.sphere.center = center;
	new_obj->data.sphere.radius = diameter / 2.0;
	new_obj->data.sphere.material.color = color;
	if (!parse_mtl_attr(&new_obj->data.sphere.material, tokens, count, 4))
		return (free(new_obj), printf("Error\nInvalid material options\n"), false);
	new_obj->next = scene->objects;
	scene->objects = new_obj;
	return (true);
}

bool	parse_plane(t_scene *scene, char **tokens)
{
	t_vec3		point;
	t_vec3		normal;
	t_vec3		color;
	t_object	*new_obj;
	int			count;

	count = count_tokens(tokens);
	if (count < 4)
		return (printf("Error\nPlane requires at least 3 parameters\n"), false);
	if (!parse_vec3(tokens[1], &point) || !parse_vec3(tokens[2], &normal)
		|| !parse_color(tokens[3], &color) || !is_normalized(normal))
		return (printf("Error\nInvalid plane parameter\n"), false);
	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		return (printf("Error\nMemory allocation failed\n"), false);
	new_obj->type = OBJECT_PLANE;
	new_obj->data.plane.point = point;
	//new_obj->data.plane.normal = normal;
	new_obj->data.plane.normal = vec3_normalize(normal);
	new_obj->data.plane.material.color = color;
	if (!parse_mtl_attr(&new_obj->data.plane.material, tokens, count, 4))
		return (free(new_obj), printf("Error\nInvalid material options\n"), false);
	new_obj->next = scene->objects;
	scene->objects = new_obj;
	return (true);
}

bool	parse_cylinder(t_scene *scene, char **tokens)
{
	t_cylinder	cyl;
	t_object	*new_obj;
	int			count;

	count = count_tokens(tokens);
	if (count_tokens(tokens) < 6)
		return (printf("Error\nNot enough parameters\n"), false);
	cyl.radius = ft_atof(tokens[3]) / 2.0;
	cyl.height = ft_atof(tokens[4]);
	if (!parse_vec3(tokens[1], &cyl.center) || !parse_vec3(tokens[2], &cyl.axis)
		|| cyl.radius <= 0.0 || cyl.height <= 0.0 || !parse_color(tokens[5],
			&cyl.material.color) || !is_normalized(cyl.axis))
		return (printf("Error\nInvalid cylinder parameter\n"), false);
	new_obj = malloc(sizeof(t_object));
	new_obj->type = OBJECT_CYLINDER;
	new_obj->data.cylinder.center = cyl.center;
	//new_obj->data.cylinder.axis = cyl.axis;
	new_obj->data.cylinder.axis = vec3_normalize(cyl.axis);
	new_obj->data.cylinder.radius = cyl.radius;
	new_obj->data.cylinder.height = cyl.height;
	new_obj->data.cylinder.material.color = cyl.material.color;
	if (!parse_mtl_attr(&new_obj->data.cylinder.material, tokens, count, 6))
		return (free(new_obj), printf("Error\nInvalid material options\n"), false);
	new_obj->next = scene->objects;
	scene->objects = new_obj;
	return (true);
}

bool	parse_cone(t_scene *scene, char **tokens)
{
	t_cone		co;
	t_object	*new_obj;
	double		angle_degrees;
	int			count;

	count = count_tokens(tokens);
	if (count < 6)
		return (printf("Error\nCone requires at least 5 parameters\n"), false);
	angle_degrees = ft_atof(tokens[3]);
	co.height = ft_atof(tokens[4]);
	if (!validate_cone_params(&co, tokens, angle_degrees))
		return (printf("Error\nInvalid cone parameter\n"), false);
	//
	co.axis = vec3_normalize(co.axis);
	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		return (printf("Error\nMemory allocation failed\n"), false);
	new_obj->type = OBJECT_CONE;
	new_obj->data.cone = co;
	if (!parse_mtl_attr(&new_obj->data.cone.material, tokens, count, 6))
		return (free(new_obj), printf("Error\nInvalid material options\n"), false);
	new_obj->next = scene->objects;
	scene->objects = new_obj;
	return (true);
}
