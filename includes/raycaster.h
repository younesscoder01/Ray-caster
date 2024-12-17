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
#include <limits.h>

# define M_PI	3.14159265358979323846	/* pi */
# define GREEN 0x0000FF00
# define RED 0x00FF0000
# define BLUE 0x000000FF
# define WHITE 0x00FFFFFF
# define BLACK 0x00000000
# define YELLOW 0x00FFFF00
# define ORANGE 0x00EB5B00
# define PURPLE 0x008A2BE2
# define PINK 0x00D91656


# define TILE_SIZE	26
# define RADIUS 5
# define FOV_ANGLE 60
# define WINDOW_WIDTH 720
# define WINDOW_HEIGHT 720
# define WALL_STRIP_WIDTH 1
# define NUM_RAYS WINDOW_WIDTH / WALL_STRIP_WIDTH
# define MINIMAP_SCALE_FACTOR 0.5

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

typedef struct	s_img_info {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
    size_t window_height;
    size_t window_width;
}				t_img_info;

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
	int wallHitX;
	int wallHitY;
	double distance;
	int isRayFacingDown;
	int isRayFacingUp;
	int isRayFacingRight;
	int isRayFacingLeft;
}  t_ray;

typedef struct s_param
{
    void	*mlx;
    void	*mlx_win;
	t_img_info data;
	t_player player;
	t_ray *rays;
	t_img_info img3d;
	char **map;
} t_param;

void draw_line(double x, double y, double x1, double y1, t_player *p, t_img_info *data, int color);
int get_player_y(char **map);
int get_player_x(char **map);
int count_file_lines(char *file_name);
void render_floor(char **map, t_img_info *data);
void render_wall(char **map, t_img_info *data);
void render_tiles(t_img_info *data, int x, int y, int color);
size_t get_map_height(char **map);
size_t get_map_weight(char **map);
char **create_map(int lines, char *file_name);
void render_p(char **map, t_img_info *data, t_player *p);
int key_p(int keycode, void *var);
void move_up( t_param *param);
int render_next_frame(void *var);
void remove_p(char **map);
int key_r(int keycode, void *var);
double deg2rad(double x);
void ft_put_pixel(t_img_info *data, int x, int y, int color);
double normalizeAngle(double angle);
void render_3d_walls(t_param *param);
void rect(t_img_info *data, int x, int y, int width, int height, int color);
char	get_t(int trgb);
char	get_r(int trgb);
char	get_g(int trgb);
char	get_b(int trgb);
unsigned int darken_color(unsigned int color, double factor);
#endif