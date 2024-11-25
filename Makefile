SRC= ./src/main.c ./src/get_next_line.c ./src/get_next_line_utils.c ./src/utils.c ./src/player_mv.c
OBJ=$(SRC:.c=.o)
NAME=raycaster

all: $(NAME)

%.o: %.c
	$(CC) -I/usr/local/include -lmlx -lXext -lX11 -O3 -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(OBJ) -I/usr/local/include -lmlx -lXext -lX11 -o $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: clean all