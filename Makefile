# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sguan <sguan@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/21 16:37:10 by sguan             #+#    #+#              #
#    Updated: 2025/11/07 14:11:01 by sguan            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MLX = minilibx-linux
LIBFT_DIR = ./libft
OBJDIR = objects

SRCS = sources/main.c \
    sources/math/ray_ops.c \
    sources/math/vec_ops.c \
    sources/math/vec_ops2.c \
    sources/math/vec_ops3.c \
    sources/intersection/intersect_sphere.c \
    sources/intersection/intersect_plane.c \
    sources/intersection/intersect_cylinder.c \
    sources/intersection/intersect_cone.c \
    sources/intersection/intersect.c \
    sources/camera/camera.c \
    sources/camera/ray_generation.c \
    sources/mlx/mlx_init.c \
    sources/scene/scene.c \
    sources/scene/parse_objects.c \
    sources/scene/parse_elements.c \
    sources/scene/scene_utils.c \
    sources/scene/parse_utils.c \
    sources/scene/parse_material_attributes.c \
    sources/render/renderer.c \
    sources/materials/patterns.c \
    sources/materials/bump_mapping.c \
    sources/lighting/ambient.c \
    sources/lighting/diffuse.c \
    sources/lighting/specular.c \
    sources/lighting/shadows.c

OBJS = $(SRCS:sources/%.c=$(OBJDIR)/%.o)

NAME = miniRT
CFLAGS = -Wall -Wextra -Werror -I/usr/include -I$(MLX) -I$(LIBFT_DIR) -I./includes
LDFLAGS = -L$(MLX) -lmlx -L/usr/lib -lXext -lX11 -lm -lz -L$(LIBFT_DIR) -lft
CC = cc
RM = rm -f

all: $(MLX) $(NAME)

$(MLX):
	if [ ! -d $(MLX) ]; then git clone https://github.com/42paris/minilibx-linux.git $(MLX); fi
	make -C $(MLX)

$(NAME): $(OBJS)
	make -C $(LIBFT_DIR)
	$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)

$(OBJDIR)/%.o: sources/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -r $(OBJDIR)
	make -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

bonus: all

.PHONY: all clean fclean re bonus