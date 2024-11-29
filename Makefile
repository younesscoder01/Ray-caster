SRC= ./src/main.c ./src/get_next_line.c ./src/get_next_line_utils.c ./src/utils.c ./src/player_mv.c
OBJ=$(SRC:.c=.o)
CFLAGS= -lm -I/usr/local/include -I./include/raycaster.h -lmlx -lXext -lX11 -O3
NAME=raycaster

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: clean all