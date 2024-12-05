#include "../includes/raycaster.h"


void p_setup(t_player *p, char **map)
{
    p->radius = RADIUS;
    p->turnDirection = 0;
    p->walkDirection = 0;
    p->rotationAngle = 0;
    p->moveSpeed = 6.0;
    p->rotationSpeed = 7;
    p->x = (get_player_x(map) * TILE_SIZE) + TILE_SIZE / 2;
    p->y = (get_player_y(map) * TILE_SIZE) + TILE_SIZE/ 2;
}

int close_win(void *var)
{
    t_param *param;

    param = (t_param *)var;
    mlx_destroy_window(param->mlx, param->mlx_win);
    exit(1);
}

int	main(int argc, char **argv)
{
	void	*mlx;
    void	*mlx_win;
    char **map;
    t_param var;
    t_data img;
    t_player p;
    t_ray rays[NUM_RAYS];

    if (argc != 2)
        return (printf("Error\n"), 1);
    if (access(argv[1], F_OK | R_OK) == -1)
        return (perror("Error"), 1);
    map = create_map(count_file_lines(argv[1]), argv[1]);
    //print the map
    printf("the map prototype\n");
    for (int i = 0; map[i]; i++)
    {
        printf("%s\n", map[i]);
    }
    img.window_height = get_map_height(map) * TILE_SIZE;
    img.window_weight = get_map_weight(map) * TILE_SIZE;
    if (img.window_weight > 1920 || img.window_height > 1080)
        return (printf("Error: the weight or the height of your map is big\n"), 1);
    mlx = mlx_init();
    mlx_win = mlx_new_window(mlx, img.window_weight, img.window_height, "raycaster");
    img.img = mlx_new_image(mlx, img.window_weight, img.window_height);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
    render_wall(map, &img);
    render_floor(map, &img);
    p_setup(&p, map);
    render_p(map, &img, &p);
    mlx_put_image_to_window(mlx, mlx_win, img.img, 0 , 0);
    var.mlx = mlx;
    var.mlx_win = mlx_win;
    var.data = img;
    var.player = p;
    var.map = map;
    var.rays = rays;
    remove_p(map);
    mlx_hook(mlx_win, ON_DESTROY, 0, close_win, &var);
    mlx_hook(mlx_win, ON_KEYDOWN, 1L << 0, key_p, &var);
    mlx_hook(mlx_win, ON_KEYUP, 1L << 1, key_r, &var);
    mlx_loop_hook(mlx, render_next_frame, &var);
    mlx_loop(mlx);
    return 0;
}