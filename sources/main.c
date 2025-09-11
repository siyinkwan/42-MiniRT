/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 20:41:41 by marvin            #+#    #+#             */
/*   Updated: 2025/09/09 18:26:53 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	handle_keypress(int keycode, t_minirt *minirt)
{
	if (keycode == 65307)
		close_window(minirt);
	return (0);
}

static int	handle_destroy(t_minirt *minirt)
{
	close_window(minirt);
	return (0);
}

static int	init_minirt(t_minirt *minirt, char *filename)
{
	t_scene	*temp_scene;

	minirt->width = 800;
	minirt->height = 600;
	temp_scene = init_scene();
	if (!temp_scene)
	{
		ft_putendl_fd("Error: Failed to initialize scene", 2);
		return (0);
	}
	if (parse_scene(filename, temp_scene) < 0)
	{
		free_scene(temp_scene);
		return (0);
	}
	minirt->scene = *temp_scene;
	free(temp_scene);
	camera_setup(&minirt->scene.camera);
	if (!init_mlx(minirt))
	{
		ft_putendl_fd("Error: Failed to initialize MLX", 2);
		return (0);
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_minirt	minirt;

	if (argc != 2)
		return (ft_putendl_fd("Usage: ./miniRT <scene.rt>", 2), 1);
	if (!init_minirt(&minirt, argv[1]))
		return (1);
	render_scene(&minirt);
	mlx_key_hook(minirt.window, handle_keypress, &minirt);
	mlx_hook(minirt.window, 17, 0, handle_destroy, &minirt);
	mlx_loop(minirt.mlx);
	return (0);
}
