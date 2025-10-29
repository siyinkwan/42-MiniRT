/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:40:21 by sguan             #+#    #+#             */
/*   Updated: 2025/10/29 15:06:51 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	put_pixel(t_minirt *minirt, int x, int y, int color)
{
	char	*pixel_addr;
	int		bytes_per_pixel;

	bytes_per_pixel = minirt->bits_per_pixel / 8;
	if (x >= 0 && y >= 0 && x < minirt->width && y < minirt->height)
	{
		pixel_addr = minirt->img_data + (y * minirt->line_length)
			+ (x * bytes_per_pixel);
		*(unsigned int *)pixel_addr = color;
	}
}

int	vec3_to_rgb(t_vec3 color)
{
	int	r;
	int	g;
	int	b;

	r = (int)(fmin(1.0, fmax(0.0, color.x)) * 255);
	g = (int)(fmin(1.0, fmax(0.0, color.y)) * 255);
	b = (int)(fmin(1.0, fmax(0.0, color.z)) * 255);
	return ((r << 16) | (g << 8) | b);
}

t_vec3	calculate_final_color(t_scene *scene, t_hit *hit)
{
	t_vec3	surface_color;
	t_vec3	ambient;
	t_vec3	diffuse;
	t_vec3	specular;
	t_vec3	temp;

	if (hit->material->bump)
		hit->normal = apply_bump(hit);
	surface_color = apply_pattern(hit);
	temp = hit->material->color;
	hit->material->color = surface_color;
	ambient = calculate_ambient(scene, hit->material);
	diffuse = calculate_diffuse(scene, hit);
	specular = calculate_specular(scene, hit);
	hit->material->color = temp;
	return (vec3_add(ambient, vec3_add(diffuse, specular)));
}

t_vec3	trace_ray(t_scene *scene, t_ray ray, int depth)
{
	t_hit	hit;

	(void)depth;
	hit = intersect_scene(ray, scene);
	if (!hit.hit)
		return (scene->background);
	return (calculate_final_color(scene, &hit));
}

void	render_scene(t_minirt *minirt)
{
	t_ray	ray;
	int		x;
	int		y;

	y = 0;
	while (y < minirt->height)
	{
		x = 0;
		while (x < minirt->width)
		{
			ray = generate_ray(&minirt->scene.camera, x, y,
					minirt->width, minirt->height);
			put_pixel(minirt, x, y,
				vec3_to_rgb(trace_ray(&minirt->scene, ray, 0)));
			x++;
		}
		y++;
	}
}
