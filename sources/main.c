/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 20:41:41 by marvin            #+#    #+#             */
/*   Updated: 2025/09/03 17:40:21 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	key_press(int keycode, t_minirt *minirt)
{
    if (keycode == 65307)
        close_window(minirt);
    return (0);
}

void	create_test_scene(t_scene *scene)
{
    scene->camera.position = vec3_create(0, 0, 0);
    scene->camera.forward = vec3_create(0, 0, -1);
    scene->camera.fov = 70.0;
    scene->camera.aspect_ratio = 800.0 / 600.0;
    scene->objects = malloc(sizeof(t_object));
    scene->objects[0].type = OBJECT_SPHERE;
    scene->objects[0].data.sphere.center = vec3_create(0, 0, -5);
    scene->objects[0].data.sphere.radius = 1.0;
    scene->objects[0].next = NULL;
    scene->ambient.ratio = 0.2;
    scene->ambient.color = vec3_create(0.1, 0.1, 0.1);
    scene->background = vec3_create(0, 0, 0);
    scene->light_count = 0;
    scene->lights = NULL;
}

int	main(void)
{
    t_minirt	minirt;
    minirt.width = 800;
    minirt.height = 600;
    create_test_scene(&minirt.scene);
    camera_setup(&minirt.scene.camera);
    if (!init_mlx(&minirt))
    {
        printf("Error: Failed to initialize MLX\n");
        return (1);
    }
    printf("Rendering scene...\n");
    render_scene(&minirt);
    display_image(&minirt);
    mlx_key_hook(minirt.window, key_press, &minirt);
    mlx_hook(minirt.window, 17, 0, close_window, &minirt);
    printf("Rendering complete! Press ESC to exit.\n");
    mlx_loop(minirt.mlx);
    return (0);
}