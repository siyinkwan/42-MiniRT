/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 14:58:49 by sguan             #+#    #+#             */
/*   Updated: 2025/11/09 17:19:37 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include <fcntl.h>
# include <stdbool.h>
# include "../libft/libft.h"
# include "mlx.h"

# ifndef PI
#  define PI 3.14159265358979323846
# endif

# define EPSILON 1e-6
# define B_EPSILON 1e-4

typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
}	t_vec3;

typedef struct s_ray
{
	t_vec3	origin;
	t_vec3	direction;
	double	t_min;
	double	t_max;
}	t_ray;

typedef enum e_pattern_type
{
	PATTERN_SOLID,
	PATTERN_CHECKER
}	t_pattern_type;

typedef struct s_material
{
	t_vec3			color;
	double			ambient;
	double			diffuse;
	double			specular;
	double			shininess;
	bool			bump;
	t_pattern_type	pattern_type;
	t_vec3			pattern_color;
	double			pattern_scale;
}	t_material;

typedef struct s_camera
{
	t_vec3	position;
	t_vec3	forward;
	t_vec3	up;
	t_vec3	right;
	double	fov;
	double	aspect_ratio;
}	t_camera;

typedef struct s_viewport
{
	double	height;
	double	width;
	double	x;
	double	y;
	t_vec3	pos_world;
}	t_viewport;

typedef struct s_pixel
{
	double	x;
	double	y;
	int		width;
	int		height;
}	t_pixel;

typedef struct s_light
{
	t_vec3	pos;
	t_vec3	color;
	double	brightness;
}	t_light;

typedef struct s_bump
{
	double	u;
	double	v;
	t_vec3	tan;
	t_vec3	bitan;
}	t_bump;

typedef enum e_type
{
	OBJECT_SPHERE,
	OBJECT_PLANE,
	OBJECT_CYLINDER,
	OBJECT_CONE
}	t_type;

typedef struct s_sphere
{
	t_vec3		center;
	double		radius;
	t_material	material;
}	t_sphere;

typedef struct s_plane
{
	t_vec3		point;
	t_vec3		normal;
	t_material	material;
}	t_plane;

typedef struct s_cylinder
{
	t_vec3		center;
	t_vec3		axis;
	double		radius;
	double		height;
	t_material	material;
}	t_cylinder;

typedef struct s_cone
{
	t_vec3		apex;
	t_vec3		axis;
	double		angle;
	double		height;
	t_material	material;
}	t_cone;

typedef struct s_object
{
	t_type				type;
	struct s_object		*next;
	union
	{
		t_sphere	sphere;
		t_plane		plane;
		t_cylinder	cylinder;
		t_cone		cone;
	}	u_data;
}	t_object;

typedef struct s_hit
{
	bool		hit;
	double		t;
	t_vec3		point;
	t_vec3		normal;
	t_material	*material;
	t_object	*object;
	t_bump		*bump;
}	t_hit;

typedef struct s_ambient
{
	double		ratio;
	t_vec3		color;
}	t_ambient;

typedef struct s_scene
{
	t_ambient	ambient;
	t_camera	camera;
	t_light		*lights;
	int			light_count;
	t_object	*objects;
	t_vec3		background;
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

typedef struct s_minirt
{
	void		*mlx;
	void		*window;
	void		*image;
	char		*img_data;
	int			width;
	int			height;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
	t_scene		scene;
}	t_minirt;

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
t_ray		ray_create(t_vec3 origin, t_vec3 direction);
bool		ray_valid_t(t_ray ray, double t);
t_vec3		ray_at(t_ray ray, double t);
double		calculate_hit_plane(t_ray ray, t_plane plane);
bool		in_circle(t_vec3 p, t_vec3 c, t_vec3 axis, double r);
t_hit		intersect_sphere(t_ray ray, t_sphere sphere);
t_hit		intersect_plane(t_ray ray, t_plane plane);
t_hit		intersect_cylinder(t_ray ray, t_cylinder cyl);
t_hit		intersect_cone(t_ray ray, t_cone cone);
t_hit		intersect_scene(t_ray ray, t_scene *scene);
void		calculate_cone_quadratic(t_ray ray, t_cone cone, t_quadratic *q);
t_vec3		calculate_cone_normal(t_cone cone, t_vec3 point, double h);
void		camera_setup(t_camera *camera);
t_ray		generate_ray(t_camera *camera, t_pixel pixel);
void		error_exit(char *msg);
void		free_tokens(char **tokens);
int			count_tokens(char **tokens);
bool		parse_color(char *token, t_vec3 *color);
bool		parse_vec3(char *token, t_vec3 *vec);
void		free_scene(t_scene *scene);
t_scene		*init_scene(void);
char		*strip_comment(char *line);
bool		has_invalid_chars(char *line);
bool		validate_cone_params(t_cone *co, char **tokens,
				double angle_degrees);
int			parse_scene(char *filename, t_scene *scene);
bool		parse_line(char *line, t_scene *scene);
bool		parse_ambient(t_scene *scene, char **tokens);
bool		parse_camera(t_scene *scene, char **tokens);
bool		parse_light(t_scene *scene, char **tokens);

bool		parse_sphere(t_scene *scene, char **tokens);
bool		parse_plane(t_scene *scene, char **tokens);
bool		parse_cylinder(t_scene *scene, char **tokens);
bool		parse_cone(t_scene *scene, char **tokens);
bool		parse_mtl_attr(t_material *material,
				char **tokens, int count, int index);

bool		validate_scene(t_scene *scene);

void		print_scene_info(t_scene *scene);

t_vec3		calculate_ambient(t_scene *scene, t_material *material);
t_vec3		calculate_diffuse(t_scene *scene, t_hit *hit);
t_vec3		calculate_specular(t_scene *scene, t_hit *hit);
t_vec3		reflect(t_vec3 light_dir, t_vec3 normal);
bool		is_in_shadow(t_scene *scene, t_hit *hit, t_vec3 light_pos);

void		put_pixel(t_minirt *minirt, int x, int y, int color);
void		render_scene(t_minirt *minirt);
int			vec3_to_rgb(t_vec3 color);
t_vec3		calculate_final_color(t_scene *scene, t_hit *hit);
t_vec3		trace_ray(t_scene *scene, t_ray ray, int depth);

int			init_mlx(t_minirt *minirt);
int			close_window(t_minirt *minirt);

double		bump_func(double u, double v);
void		compute_uv(t_hit *hit, double *u, double *v);
void		compute_tangent_space(t_hit *hit, t_vec3 *tan, t_vec3 *bitan);
t_vec3		apply_bump(t_hit *hit);

t_vec3		checkerboard_pattern(double u, double v,
				t_vec3 color1, t_vec3 color2);
t_vec3		apply_pattern(t_hit *hit);

#endif
