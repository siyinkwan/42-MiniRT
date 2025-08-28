# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marvin <marvin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/21 16:37:10 by sguan             #+#    #+#              #
#    Updated: 2025/08/28 12:05:49 by marvin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MLX = minilibx-linux
LIBFT_DIR = ./libft
OBJDIR = objects

SRCS = sources/main.c \
	sources/math/vec_ops.c \
	sources/math/vec_ops2.c \
	sources/math/vec_ops3.c \
	sources/includes/intersect_sphere.c \
	sources/includes/intersect_plane.c \
	sources/includes/intersect_cylinder.c

OBJS = $(SRCS:sources/%.c=$(OBJDIR)/%.o)

NAME = miniRT
CFLAGS = -Wall -Wextra -Werror -ggdb -I/usr/include -I$(MLX) -I$(LIBFT_DIR) -I./includes
LDFLAGS = -L$(MLX) -lmlx -L/usr/lib -lXext -lX11 -lm -lz -L$(LIBFT_DIR) -lft
CC = cc
RM = rm -f

all: $(MLX) $(NAME)

test_vec3: sources/math/vec3_test.c
	$(CC) $(CFLAGS) sources/math/vec3_test.c sources/math/vec_ops2.c sources/math/vec_ops3.c $(LIBFT_DIR)/libft.a -o test_vec3 -lm

run_vec3_test: test_vec3
	./test_vec3

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
	$(RM) test_vec3
	make -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

bonus: all

.PHONY: all clean fclean re bonus test_vec3 run_vec3_test
