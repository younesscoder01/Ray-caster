#include "../includes/raycaster.h"


int key_p(int keycode, void *var)
{
    t_param *param;
    double moveStep;
    int check_x;
    int check_y;

    param = (t_param *)var;
    if (keycode == KEY_W || keycode == KEY_UP)
        param->player.walkDirection = 1;
    else if (keycode == KEY_S || keycode == KEY_DOWN)
        param->player.walkDirection = -1;
    else if (keycode == KEY_A || keycode == KEY_LEFT)
        param->player.turnDirection = -1;
    else if (keycode == KEY_D || keycode == KEY_RIGHT)
        param->player.turnDirection = 1;
    else if (keycode == ESC)
        exit(1);
    param->player.rotationAngle += param->player.turnDirection * param->player.rotationSpeed;
    moveStep = param->player.walkDirection * param->player.moveSpeed;
    check_x = round(cos(deg2rad(param->player.rotationAngle)) * moveStep);
    check_y = round(sin(deg2rad(param->player.rotationAngle)) * moveStep);
    if (param->map[param->player.y / TILE_SIZE][(param->player.x + check_x) / TILE_SIZE] != '1')
        param->player.x += check_x;
    if (param->map[(param->player.y + check_y) / TILE_SIZE][param->player.x / TILE_SIZE] != '1')
        param->player.y += check_y;
    printf("x: %i\ny: %i\n", param->player.x, param->player.y);
}

int key_r(int keycode, void *var)
{
    t_param *param;

    param = (t_param *)var;
    if (keycode == KEY_W || keycode == KEY_UP)
        param->player.walkDirection = 0;
    else if (keycode == KEY_S || keycode == KEY_DOWN)
        param->player.walkDirection = 0;
    else if (keycode == KEY_A || keycode == KEY_LEFT)
        param->player.turnDirection = 0;
    else if (keycode == KEY_D || keycode == KEY_RIGHT)
        param->player.turnDirection = 0;
}

int render_next_frame(void *var)
{
    t_param *param;

    param = (t_param *)var;

    render_wall(param->map, &param->data);
    render_floor(param->map, &param->data);
    render_p(param->map, &param->data, &param->player);
    mlx_put_image_to_window(param->mlx, param->mlx_win, param->data.img, 0 , 0);
}