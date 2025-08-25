# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marvin <marvin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/21 16:37:10 by sguan             #+#    #+#              #
#    Updated: 2025/08/25 20:44:38 by marvin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MLX = minilibx-linux
LIBFT_DIR = ./libft

SRCS = sources/main.c \
	sources/math/vec_ops.c \
	sources/math/vec_ops2.c \
	sources/math/vec_ops3.c

OBJS = $(SRCS:.c=.o)
NAME = miniRT
CFLAGS = -Wall -Wextra -Werror -ggdb -lm -I/usr/include -I$(MLX) -I$(LIBFT_DIR) -I./includes
LDFLAGS = -L$(MLX) -lmlx -L/usr/lib -lXext -lX11 -lm -lz  -L $(LIBFT_DIR)
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

%.o: %.c
	$(CC) $(CFLAGS) -I$(MLX) -I/includes -O3 -c $< -o $@

clean:
	$(RM) $(OBJS)
	make -C ${LIBFT_DIR} clean

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

bonus: all
	