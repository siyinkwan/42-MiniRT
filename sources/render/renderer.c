/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:40:21 by sguan             #+#    #+#             */
/*   Updated: 2025/10/29 14:03:52 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	put_pixel(t_minirt *minirt, int x, int y, int color)
{
	char	*pixel_addr;
	int		bytes_per_pixel;

	pixel_addr = NULL;
	bytes_per_pixel = minirt->bits_per_pixel / 8;
	if (x >= 0 && y >= 0 && x < minirt->width && y < minirt->height)
	{
		pixel_addr = minirt->img_data + y * minirt->line_length + x * bytes_per_pixel;
		*(unsigned int*)pixel_addr = color;
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
	return (r << 16 | g << 8 | b);
}

int	calculate_pixel_color(t_scene *scene, t_hit *hit)
{
	t_vec3	ambient;
	t_vec3	diffuse;
	t_vec3	specular;
	t_vec3	color;

	ambient = calculate_ambient(scene, hit->material);
	diffuse = calculate_diffuse(scene, hit);
	specular = calculate_specular(scene, hit);
	color = vec3_add(ambient, vec3_add(diffuse, specular));
	return (vec3_to_rgb(color));
}

void	render_scene(t_minirt *minirt)
{
	t_ray	ray;
	int		x;
	int		y;
	t_hit	hit;
	int		color;
	
	y = 0.0;
	while (y < minirt->height)
	{
		x = 0;
		while (x < minirt->width)
		{
			ray = generate_ray(&minirt->scene.camera, x, y, minirt->width, minirt->height);
			hit = intersect_scene(ray, &minirt->scene);
			if (hit.hit)
				color = calculate_pixel_color(&minirt->scene, &hit);
			else
				color = vec3_to_rgb(minirt->scene.background);
			put_pixel(minirt, x, y, color);
			x++;
		}
		y++;
	}
}
