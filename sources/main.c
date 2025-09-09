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
	if (keycode == 65307)  // ESC key
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
	minirt->width = 800;
	minirt->height = 600;
	
	if (!load_scene(filename, &minirt->scene))
	{
		ft_putendl_fd("Error: Failed to load scene", 2);
		return (0);
	}
	
	camera_setup(&minirt->scene.camera);
	
	if (!init_mlx(minirt))
	{
		ft_putendl_fd("Error: Failed to initialize MLX", 2);
		free_scene(&minirt->scene);
		return (0);
	}
	
	return (1);
}

int	main(int argc, char **argv)
{
	t_minirt	minirt;

	if (argc != 2)
	{
		ft_putendl_fd("Usage: ./miniRT <scene.rt>", 2);
		return (1);
	}
	
	if (!init_minirt(&minirt, argv[1]))
		return (1);
	
	render_scene(&minirt);
	display_image(&minirt);
	
	mlx_hook(minirt.window, 2, 1L<<0, handle_keypress, &minirt);
	mlx_hook(minirt.window, 17, 1L<<17, handle_destroy, &minirt);
	
	mlx_loop(minirt.mlx);
	
	return (0);
}
