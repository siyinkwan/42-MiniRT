/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 14:58:49 by sguan             #+#    #+#             */
/*   Updated: 2025/08/22 15:45:18 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include <fcntl.h>
# include "../libft/libft.h"
# include "../minilibx-linux/mlx.h"

/* ************************************************************************** */
/*                              MATHEMATICAL TYPES                           */
/* ************************************************************************** */

/*
** t_vec3 - Our fundamental 3D vector structure
** Used for: positions, directions, colors (RGB), normals
** 
** Design decisions:
** - double for precision (important for ray intersections)
** - Individual fields (x,y,z) for readability vs array for performance
** - Same structure for geometry and colors (different semantic meaning)
*/
typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
}	t_vec3;

/*
** t_ray - Represents a ray in 3D space
** Ray equation: P(t) = origin + t * direction
** 
** Design decisions:
** - direction should always be normalized (unit vector)
** - t_min prevents self-intersection issues (floating point precision)
** - t_max limits ray distance (performance and realism)
*/
typedef struct s_ray
{
	t_vec3	origin;		// Starting point of the ray
	t_vec3	direction;	// Normalized direction vector
	double	t_min;		// Minimum valid parameter (avoid self-intersection)
	double	t_max;		// Maximum valid parameter (limit distance)
}	t_ray;

/*
** Pattern types for bonus features
** Each pattern is calculated differently but uses same material properties
*/
typedef enum e_pattern_type
{
	PATTERN_SOLID,			// Single color
	PATTERN_CHECKERBOARD,	// Alternating squares
	PATTERN_STRIPES,		// Parallel stripes (bonus)
	PATTERN_GRADIENT		// Color gradient (bonus)
}	t_pattern_type;

/*
** t_material - Surface properties for Phong lighting model
** 
** Phong lighting = Ambient + Diffuse + Specular
** - Ambient: Base color when no direct light hits
** - Diffuse: Matte reflection (depends on surface normal and light direction)
** - Specular: Shiny reflection (depends on viewing angle)
*/
typedef struct s_material
{
	t_vec3			color;			// Base color (RGB values 0-1)
	double			ambient;		// Ambient reflection coefficient [0-1]
	double			diffuse;		// Diffuse reflection coefficient [0-1]
	double			specular;		// Specular reflection coefficient [0-1]
	double			shininess;		// Specular exponent (higher = sharper highlights)
	t_pattern_type	pattern_type;	// Which pattern to apply
	t_vec3			pattern_color2;	// Second color for patterns (checkerboard, etc.)
	double			pattern_scale;	// Size of pattern elements
}	t_material;

/*
** t_camera - Defines viewpoint and projection
** 
** Camera coordinate system:
** - position: where camera is located
** - forward: direction camera is looking (normalized)
** - up: which way is "up" for the camera (normalized)
** - right: calculated from forward × up (normalized)
*/
typedef struct s_camera
{
	t_vec3	position;		// Camera location in world space
	t_vec3	forward;		// Looking direction (normalized)
	t_vec3	up;				// Up direction (normalized)
	t_vec3	right;			// Right direction (calculated)
	double	fov;			// Field of view in degrees [0-180]
	double	aspect_ratio;	// Width/height ratio
}	t_camera;

/*
** t_light - Light source for illumination
** 
** For bonus: supports colored lights and multiple lights
*/
typedef struct s_light
{
	t_vec3	position;		// Light location in world space
	t_vec3	color;			// Light color (RGB values 0-1)
	double	brightness;		// Light intensity [0-1]
}	t_light;

#endif
