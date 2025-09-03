/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 17:40:30 by sguan             #+#    #+#             */
/*   Updated: 2025/09/03 18:25:14 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	init_mlx(t_minirt *minirt)
{
    minirt->mlx = mlx_init();
    if (!minirt->mlx)
        return (0);
    minirt->window = mlx_new_window(minirt->mlx, minirt->width, minirt->height, "MiniRT");
    if (!minirt->window)
        return (0);
    minirt->image = mlx_new_image(minirt->mlx, minirt->width, minirt->height);
    if (!minirt->image)
        return (0);
    minirt->img_data = mlx_get_data_addr(minirt->image, &minirt->bits_per_pixel,
                                            &minirt->line_length, &minirt->endian);
    return (1);
}

void	display_image(t_minirt *minirt)
{
    mlx_put_image_to_window(minirt->mlx, minirt->window, minirt->image, 0, 0);
}

int	close_window(t_minirt *minirt)
{
    if (minirt->image)
        mlx_destroy_image(minirt->mlx, minirt->image);
    if (minirt->window)
        mlx_destroy_window(minirt->mlx, minirt->window);
    if (minirt->mlx)
    {
        mlx_destroy_display(minirt->mlx);
        free(minirt->mlx);
    }
    exit(0);
    return (0);
}