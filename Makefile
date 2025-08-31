# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sguan <sguan@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/21 16:37:10 by sguan             #+#    #+#              #
#    Updated: 2025/08/31 21:46:18 by sguan            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MLX = minilibx-linux
LIBFT_DIR = ./libft
OBJDIR = objects

# 移除test_intersections.c，它不应该在主程序中
SRCS = sources/main.c \
	sources/math/ray_ops.c \
    sources/math/vec_ops.c \
    sources/math/vec_ops2.c \
    sources/math/vec_ops3.c \
    sources/intersection/intersect_sphere.c \
    sources/intersection/intersect_plane.c \
    sources/intersection/intersect_cylinder.c \
    sources/intersection/intersect_cone.c \
    sources/intersect_scene.c

OBJS = $(SRCS:sources/%.c=$(OBJDIR)/%.o)

NAME = miniRT
CFLAGS = -Wall -Wextra -Werror -ggdb -I/usr/include -I$(MLX) -I$(LIBFT_DIR) -I./includes
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

# 简化的测试目标
test: 
	@echo "Building intersection tests..."
	@make -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) test_intersections.c \
		sources/math/ray_ops.c \
		sources/math/vec_ops.c \
		sources/math/vec_ops2.c \
		sources/math/vec_ops3.c \
		sources/intersection/intersect_sphere.c \
		sources/intersection/intersect_plane.c \
		sources/intersection/intersect_cylinder.c \
		sources/intersection/intersect_cone.c \
		sources/intersection/intersect_scene.c \
		-L$(LIBFT_DIR) -lft -lm -o test_intersections
	@echo "Running tests..."
	./test_intersections
	@echo "Cleaning up..."
	$(RM) test_intersections

test_clean:
	$(RM) test_intersections

.PHONY: all clean fclean re bonus test test_clean
