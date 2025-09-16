/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 14:58:49 by sguan             #+#    #+#             */
/*   Updated: 2025/09/16 19:08:23 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include <fcntl.h>
 #include <stdbool.h>
# include "../libft/libft.h"
# include "mlx.h"

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define EPSILON 1e-9
#define BUMP_EPSILON 1e-4
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
	t_vec3			p_color2;	// Second color for patterns (checkerboard, etc.)
	double			pattern_scale;	// Size of pattern elements
	bool			bump;
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

typedef struct	s_viewport
{
	double	height;
	double	width;
	double	x;
	double	y;
	t_vec3	pos_in_world;
}	t_viewport;

/*
** t_light - Light source for illumination
** 
** For bonus: supports colored lights and multiple lights
*/
typedef struct s_light
{
	t_vec3	pos;		// Light location in world space
	t_vec3	color;			// Light color (RGB values 0-1)
	double	brightness;		// Light intensity [0-1]
}	t_light;

/* ************************************************************************** */
/*                             GEOMETRIC OBJECTS                             */
/* ************************************************************************** */

/*
** Object type enumeration for polymorphic object handling
*/
typedef enum e_type
{
	OBJECT_SPHERE,
	OBJECT_PLANE,
	OBJECT_CYLINDER,
	OBJECT_CONE			// Bonus: Second-degree object
}	t_type;

/*
** t_sphere - Sphere geometric object
** Equation: |P - center|² = radius²
*/
typedef struct s_sphere
{
	t_vec3		center;		// Center point of sphere
	double		radius;		// Radius (diameter/2 from .rt file)
	t_material	material;	// Surface properties
}	t_sphere;

/*
** t_plane - Infinite plane geometric object
** Equation: (P - point) · normal = 0
*/
typedef struct s_plane
{
	t_vec3		point;		// Any point on the plane
	t_vec3		normal;		// Normalized normal vector
	t_material	material;	// Surface properties
}	t_plane;

/*
** t_cylinder - Finite cylinder geometric object
** Axis-aligned cylinder with circular cross-sections
*/
typedef struct s_cylinder
{
	t_vec3		center;		// Center of cylinder base
	t_vec3		axis;		// Normalized axis direction vector
	double		radius;		// Radius (diameter/2 from .rt file)
	double		height;		// Height of cylinder
	t_material	material;	// Surface properties
}	t_cylinder;

/*
** t_cone - Cone geometric object (BONUS)
** Mathematical definition: cone with circular base
** Apex angle determines how "wide" the cone opens
*/
typedef struct s_cone
{
	t_vec3		apex;		// Apex (pointed end) of cone
	t_vec3		axis;		// Normalized axis from tip toward base
	double		angle;		// Half-angle in radians (tan(angle) = radius/height)
	double		height;		// Height from tip to base
	t_material	material;	// Surface properties
}	t_cone;

/*
** t_object - Polymorphic object container
** Uses union for memory efficiency and type safety
*/
typedef struct s_object
{
	t_type	type;		// Which type of object this is
	union
	{
		t_sphere	sphere;
		t_plane		plane;
		t_cylinder	cylinder;
		t_cone		cone;		// Bonus object
	}	data;
	struct s_object	*next;		// Linked list for multiple objects
}	t_object;

/* ************************************************************************** */
/*                           INTERSECTION & SCENE                            */
/* ************************************************************************** */

/*
** t_hit - Intersection result structure
** Contains all information about a ray-object intersection
*/
typedef struct s_hit
{
	bool		hit;		// Did intersection occur?
	double		t;			// Distance parameter along ray
	t_vec3		point;		// 3D intersection point
	t_vec3		normal;		// Surface normal at intersection (normalized)
	t_material	*material;	// Pointer to intersected object's material
	t_object	*object;	// Pointer to intersected object
}	t_hit;

/*
** t_ambient - Global ambient lighting
** Represents light that illuminates all objects equally
*/
typedef struct s_ambient
{
	double		ratio;		// Ambient lighting ratio [0.0-1.0]
	t_vec3		color;		// Ambient light color (RGB 0-1)
}	t_ambient;

/*
** t_scene - Complete scene description
** Contains all elements needed for ray tracing
*/
typedef struct s_scene
{
	t_ambient	ambient;	// Global ambient lighting
	t_camera	camera;		// Single camera (mandatory)
	t_light		*lights;	// Array of lights (bonus: multiple)
	int			light_count;// Number of lights in scene
	t_object	*objects;	// Linked list of all objects
	t_vec3		background;	// Background color when rays miss all objects
}	t_scene;

typedef struct s_quadratic
{
    double	a;
    double	b;
    double	c;
    double	discriminant;
    double	t1;
    double	t2;
}	t_quadratic;

/* ************************************************************************** */
/*                            MINILIBX INTEGRATION                           */
/* ************************************************************************** */

/*
** t_minirt - Main program structure
** Integrates MinilibX with ray tracing scene
*/
typedef struct s_minirt
{
	void		*mlx;			// MLX instance pointer
	void		*window;		// Window instance pointer
	void		*image;			// Image buffer pointer
	char		*img_data;		// Direct access to image pixel data
	int			width;			// Window/image width in pixels
	int			height;			// Window/image height in pixels
	int			bits_per_pixel;	// Color depth (usually 32)
	int			line_length;	// Bytes per image line
	int			endian;			// Byte order for color values
	t_scene		scene;			// Ray tracing scene data
}	t_minirt;

/* ************************************************************************** */
/*                            FUNCTION DECLARATIONS                          */
/* ************************************************************************** */

/*
** Vector mathematics operations
*/
t_vec3		vec3_create(double x, double y, double z);
t_vec3		vec3_add(t_vec3 a, t_vec3 b);
t_vec3		vec3_subtract(t_vec3 a, t_vec3 b);
t_vec3		vec3_scale(t_vec3 v, double scalar);
t_vec3		vec3_divide(t_vec3 v, double scalar);
double		vec3_dot(t_vec3 a, t_vec3 b);
t_vec3		vec3_cross(t_vec3 a, t_vec3 b);
t_vec3		vec3_mul(t_vec3 a, t_vec3 b);
double		vec3_length(t_vec3 v);
double		vec3_len_squared(t_vec3 v);
t_vec3		vec3_normalize(t_vec3 v);
double		vec3_distance(t_vec3 a, t_vec3 b);
bool		is_normalized(t_vec3 vec);


/*
** Ray operations
*/
t_ray		ray_create(t_vec3 origin, t_vec3 direction);
bool		ray_valid_t(t_ray ray, double t);
t_vec3		ray_at(t_ray ray, double t);

/*
** Object intersection functions
*/
double		calculate_hit_plane(t_ray ray, t_plane plane);
bool		in_circle(double t, t_ray ray, t_plane plane, double radius);
t_hit		intersect_sphere(t_ray ray, t_sphere sphere);
t_hit		intersect_plane(t_ray ray, t_plane plane);
t_hit		intersect_cylinder(t_ray ray, t_cylinder cylinder);
t_hit		intersect_cone(t_ray ray, t_cone cone);
t_hit		intersect_scene(t_ray ray, t_scene *scene);

/*
** Camera and ray generation
*/
void		camera_setup(t_camera *camera);
t_ray		generate_ray(t_camera *camera, double pixel_x, double pixel_y, int width, int height);

/*
** Scene parsing utilities
*/
void		error_exit(char *msg);
void		free_tokens(char **tokens);
int			count_tokens(char **tokens);
bool		parse_color(char *token, t_vec3 *color);
bool		parse_vec3(char *token, t_vec3 *vec);
void		free_scene(t_scene *scene);
t_scene		*init_scene(void);
char		*strip_comment(char *line);
bool		has_invalid_chars(char *line);
bool		validate_cone_params(t_cone *co, char **tokens, double angle_degrees);

/*
** Scene parsing
*/
int			parse_scene(char *filename, t_scene *scene);
bool		parse_line(char *line, t_scene *scene);

/*
** Scene elements parsing
*/
bool		parse_ambient(t_scene *scene, char **tokens);
bool		parse_camera(t_scene *scene, char **tokens);
bool		parse_light(t_scene *scene, char **tokens);

/*
** Object parsing
*/
bool		parse_sphere(t_scene *scene, char **tokens);
bool		parse_plane(t_scene *scene, char **tokens);
bool		parse_cylinder(t_scene *scene, char **tokens);
bool		parse_cone(t_scene *scene, char **tokens);
bool		parse_mtl_attr(t_material *material, char **tokens, int count, int index);

/*
** Scene validation
*/
bool		validate_scene(t_scene *scene);

/*
** Lighting calculations
*/
t_vec3		calculate_ambient(t_scene *scene, t_material *material);
t_vec3		calculate_diffuse(t_scene *scene, t_hit *hit);
t_vec3		calculate_specular(t_scene *scene, t_hit *hit);
t_vec3		reflect(t_vec3 light_dir, t_vec3 normal);
bool		is_in_shadow(t_scene *scene, t_hit *hit, t_vec3 light_pos);
/*
** Rendering
*/
void		put_pixel(t_minirt *minirt, int x, int y, int color);
void		render_scene(t_minirt *minirt);
int			vec3_to_rgb(t_vec3 color);
int			get_pixel_color(t_scene *scene, t_hit *hit);  // Add this if you keep the function

/*
** MinilibX integration
*/
int			init_mlx(t_minirt *minirt);
void		display_image(t_minirt *minirt);
int			close_window(t_minirt *minirt);

#endif
