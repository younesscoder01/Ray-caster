#ifndef RAYCASTER_H
# define RAYCASTER_H
#include "get_next_line.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <mlx.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define TILE_SIZE 64


typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
    size_t window_height;
    size_t window_weight;
}				t_data;

typedef struct s_player
{
	int x;
	int y;
	int radius;
	int turnDirection;
	int walkDirection;
	int rotationAngle;
	double moveSpeed;
	int rotationSpeed;
}				t_player;

enum {
	ON_KEYDOWN = 2,
	ON_KEYUP = 3,
	ON_MOUSEDOWN = 4,
	ON_MOUSEUP = 5,
	ON_MOUSEMOVE = 6,
	ON_EXPOSE = 12,
	ON_DESTROY = 17
};
typedef struct s_param
{
    void	*mlx;
    void	*mlx_win;
	t_data data;
	t_player player;
	char **map;
} t_param;


int get_player_y(char **map);
int get_player_x(char **map);
int count_file_lines(char *file_name);
void flood_fill(char **map, int x, int y, int h, int w, t_param par);
void render_floor(char **map, t_data *data);
void render_wall(char **map, t_data *data);
void render_tiles(t_data *data, int x, int y, int color);
size_t get_map_height(char **map);
size_t get_map_weight(char **map);
char **create_map(int lines, char *file_name);
void render_p(char **map, t_data *data, t_player *p);
int key_p(int keycode, void *var);
void move_up( t_param *param);
int render_next_frame(void *var);
void remove_p(char **map);
#endif