/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_objects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 16:01:03 by sguan             #+#    #+#             */
/*   Updated: 2025/09/09 18:25:20 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

bool	parse_mtl_attr(t_material *material, char **tokens,
			int count, int index)
{
	material->ambient = 0.1;
	material->diffuse = 0.9;
	material->specular = 0.9;
	material->shininess = 200.0;
	material->pattern_type = PATTERN_SOLID;
	material->p_color2 = vec3_create(255, 255, 255);
	material->pattern_scale = 1.0;
	material->bump = false;
	if (index < count)
		material->ambient = ft_atof(tokens[index++]);
	if (index < count)
		material->diffuse = ft_atof(tokens[index++]);
	if (index < count)
		material->specular = ft_atof(tokens[index++]);
	if (index < count)
		material->shininess = ft_atof(tokens[index++]);
	if (index < count)
		material->pattern_type = (t_pattern_type)ft_atoi(tokens[index++]);
	if (index < count && !parse_color(tokens[index++], &material->p_color2))
		return (false);
	if (index < count)
		material->pattern_scale = ft_atof(tokens[index++]);
	if (index < count)
		material->bump = ft_atoi(tokens[index]);
	return (true);
}

bool	parse_sphere(t_scene *scene, char **tokens)
{
	t_vec3		center;
	double		diameter;
	t_vec3		color;
	t_object	*new_obj;
	int			count;

	count = count_tokens(tokens);
	if (count < 4)
		return (printf("Error: Sphere requires at least 3 parameters\n"), false);
	diameter = ft_atof(tokens[2]);
	if (!parse_vec3(tokens[1], &center) || diameter <= 0.0
		|| !parse_color(tokens[3], &color))
		return (printf("Error: Invalid sphere parameter\n"), false);
	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		return (printf("Error: Memory allocation failed\n"), false);
	new_obj->type = OBJECT_SPHERE;
	new_obj->data.sphere.center = center;
	new_obj->data.sphere.radius = diameter / 2.0;
	new_obj->data.sphere.material.color = color;
	if (!parse_mtl_attr(&new_obj->data.sphere.material, tokens, count, 4))
		return (printf("Error: Invalid material options\n"), false);
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
		return (printf("Error: Plane requires at least 3 parameters\n"), false);
	if (!parse_vec3(tokens[1], &point) || !parse_vec3(tokens[2], &normal)
		|| !parse_color(tokens[3], &color) || !is_normalized(normal))
		return (printf("Error: Invalid plane parameter\n"), false);
	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		return (printf("Error: Memory allocation failed\n"), false);
	new_obj->type = OBJECT_PLANE;
	new_obj->data.plane.point = point;
	new_obj->data.plane.normal = normal;
	new_obj->data.plane.material.color = color;
	if (!parse_mtl_attr(&new_obj->data.plane.material, tokens, count, 4))
		return (printf("Error: Invalid material options\n"), false);
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
		return (printf("Error: Not enough parameters\n"), false);
	cyl.radius = ft_atof(tokens[3]) / 2.0;
	cyl.height = ft_atof(tokens[4]);
	if (!parse_vec3(tokens[1], &cyl.center) || !parse_vec3(tokens[2], &cyl.axis)
		|| cyl.radius <= 0.0 || cyl.height <= 0.0 || !parse_color(tokens[5],
			&cyl.material.color) || !is_normalized(cyl.axis))
		return (printf("Error: Invalid cylinder parameter\n"), false);
	new_obj = malloc(sizeof(t_object));
	new_obj->type = OBJECT_CYLINDER;
	new_obj->data.cylinder.center = cyl.center;
	new_obj->data.cylinder.axis = cyl.axis;
	new_obj->data.cylinder.radius = cyl.radius;
	new_obj->data.cylinder.height = cyl.height;
	new_obj->data.cylinder.material.color = cyl.material.color;
	if (!parse_mtl_attr(&new_obj->data.cylinder.material, tokens, count, 6))
		return (printf("Error: Invalid material options\n"), false);
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
		return (printf("Error: Cone requires at least 5 parameters\n"), false);
	angle_degrees = ft_atof(tokens[3]);
	co.height = ft_atof(tokens[4]);
	if (!validate_cone_params(&co, tokens, angle_degrees))
		return (printf("Error: Invalid cone parameter\n"), false);
	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		return (printf("Error: Memory allocation failed\n"), false);
	new_obj->type = OBJECT_CONE;
	new_obj->data.cone = co;
	if (!parse_mtl_attr(&new_obj->data.cone.material, tokens, count, 6))
		return (printf("Error: Invalid material options\n"), false);
	new_obj->next = scene->objects;
	scene->objects = new_obj;
	return (true);
}
