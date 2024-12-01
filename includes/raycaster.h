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

# define M_PI	3.14159265358979323846	/* pi */
# define TILE_SIZE	32
# define RADIUS 4
# define GREEN 0x0000FF00
# define RED 0x00FF0000
# define BLUE 0x000000FF
# define WHITE 0x00FFFFFF
# define BLACK 0x00000000
# define YELLOW 0x00FFFF00
# define ORANGE 0x00FFA500
# define PURPLE 0x008A2BE2

# define FOV_ANGLE 60
# define NUM_RAYS 320

enum {
	KEY_W = 119,
	KEY_A = 97,
	KEY_S = 115,
	KEY_D = 100,
	KEY_UP = 65362,
	KEY_DOWN = 65364,
	KEY_LEFT = 65361,
	KEY_RIGHT = 65363,
	ESC = 65307
};

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
	double radius;
	double turnDirection;
	double walkDirection;
	double rotationAngle;
	double moveSpeed;
	double rotationSpeed;
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

typedef struct s_ray 
{
	double rayAngle;
	int columnId;
	int ray_lenght;
}  t_ray;

typedef struct s_param
{
    void	*mlx;
    void	*mlx_win;
	t_data data;
	t_player player;
	t_ray *rays;
	char **map;
} t_param;

void draw_line(double x, double y, double x1, double y1, t_player *p, t_data *data, int color);
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
int key_r(int keycode, void *var);
double deg2rad(double x);
#endif