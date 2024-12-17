#include "../includes/raycaster.h"


int key_p(int keycode, void *var)
{
    t_param *param;
    double moveStep;
    int check_x;
    int check_y;

    param = (t_param *)var;
    if (keycode == KEY_W || keycode == KEY_UP || keycode == KEY_A)
        param->player.walkDirection = 1;
    else if (keycode == KEY_S || keycode == KEY_DOWN || keycode == KEY_D)
        param->player.walkDirection = -1;
    else if (keycode == KEY_LEFT)
        param->player.turnDirection = -1;
    else if (keycode == KEY_RIGHT)
        param->player.turnDirection = 1;
    else if (keycode == ESC)
        exit(1);
    param->player.rotationAngle += param->player.turnDirection * param->player.rotationSpeed;
    param->player.rotationAngle = normalizeAngle(param->player.rotationAngle);
    moveStep = param->player.walkDirection * param->player.moveSpeed;
    if (keycode == KEY_A || keycode == KEY_D)
    {
        check_x = param->player.x - round(cos(deg2rad(param->player.rotationAngle + 90)) * moveStep);
        check_y = param->player.y - round(sin(deg2rad(param->player.rotationAngle + 90)) * moveStep);
    }
    else
    {
        check_x = param->player.x + round(cos(deg2rad(param->player.rotationAngle)) * moveStep);
        check_y = param->player.y + round(sin(deg2rad(param->player.rotationAngle)) * moveStep);
    }
    printf("walkDirection: %f\n", param->player.walkDirection);
    printf("rotationAngle: %f\n", param->player.rotationAngle);
    if (param->map[param->player.y / TILE_SIZE][check_x / TILE_SIZE] != '1')
        param->player.x = check_x;
    if (param->map[check_y / TILE_SIZE][param->player.x / TILE_SIZE] != '1')
        param->player.y = check_y;
    return 0;
}

int key_r(int keycode, void *var)
{
    t_param *param;

    param = (t_param *)var;
    if (keycode == KEY_W || keycode == KEY_UP || keycode == KEY_A)
        param->player.walkDirection = 0;
    else if (keycode == KEY_S || keycode == KEY_DOWN || keycode == KEY_D)
        param->player.walkDirection = 0;
    else if (keycode == KEY_LEFT)
        param->player.turnDirection = 0;
    else if (keycode == KEY_RIGHT)
        param->player.turnDirection = 0;
    return 0;
}

double normalizeAngle(double angle)
{
    while (angle < 0)
        angle += 360;
    while (angle >= 360)
        angle -= 360;
    return angle;
}

double distance(int x1, int y1, int x2, int y2)
{
    return sqrt(pow(x2 - x1, 2)
                + pow(y2 - y1, 2) * 1.0);
}

void castAllrays(t_param *param)
{
    int i;
    double x1;
    double y1;
    double xintercept;
    double yintercept;
    double xstep;
    double ystep;
    int Hhitx = 0;
    int Hhity = 0;
    int Vhitx = 0;
    int Vhity = 0;
    int foundHwallhit;
    int foundVwallhit;
    double Hdistance = 0;
    double Vdistance = 0;
    int px;
    int py;
    double rayAngle;

    rayAngle = param->player.rotationAngle - (FOV_ANGLE / 2);
    i = 0;
    int count = 0;
    while (i < NUM_RAYS)
    {
        foundHwallhit = 0;
        foundVwallhit = 0;
        Hdistance = 0;
        Hdistance = 0;
        px = 0;
        py = 0;
        // printf("before rayAngle: %f\n", rayAngle);
        rayAngle = normalizeAngle(rayAngle);
        param->rays[i].rayAngle = rayAngle;
        param->rays[i].isRayFacingDown = rayAngle > 0 && rayAngle < 180;
        param->rays[i].isRayFacingUp = (rayAngle > 180 && rayAngle < 360) ;// || (rayAngle == 0 || rayAngle == 360);
        param->rays[i].isRayFacingRight = rayAngle < 90 || rayAngle > 270;
        param->rays[i].isRayFacingLeft = (rayAngle > 90 && rayAngle < 270);
        param->rays[i].columnId = i;


        //horizontal interception
        yintercept = floor(param->player.y / TILE_SIZE) * TILE_SIZE;
        if (param->rays[i].isRayFacingDown)
            yintercept += TILE_SIZE;
        xintercept = param->player.x + ((yintercept - param->player.y) / tan(deg2rad(rayAngle)));

        //xstep and ystep
        ystep = TILE_SIZE;
        if (param->rays[i].isRayFacingUp)
            ystep *= -1;
        xstep = TILE_SIZE / tan(deg2rad(rayAngle));
        if (param->rays[i].isRayFacingLeft && xstep > 0)
            xstep *= -1;
        if (param->rays[i].isRayFacingRight && xstep < 0)
            xstep *= -1;
        if (param->rays[i].isRayFacingUp)
            py = 1;
        while (xintercept >= 0 && xintercept < param->data.window_width \
                && yintercept >= 0 && yintercept < param->data.window_height )
        {
            if (param->map[((int)yintercept / TILE_SIZE) - py][(int)xintercept / TILE_SIZE] == '1')
            {
                foundHwallhit = 1;
                Hhitx = xintercept;
                Hhity = yintercept;
                break;
            }
            yintercept += ystep;
            xintercept += xstep;
        }

        // vertical interception
        xintercept = floor(param->player.x / TILE_SIZE) * TILE_SIZE;
        if (param->rays[i].isRayFacingRight)
            xintercept += TILE_SIZE;
        yintercept = param->player.y + (xintercept - param->player.x) * tan(deg2rad(rayAngle));
        // xstep and ystep
        xstep = TILE_SIZE;
        if (param->rays[i].isRayFacingLeft)
            xstep *= -1;
        ystep = TILE_SIZE * tan(deg2rad(rayAngle));
        if (param->rays[i].isRayFacingUp && ystep > 0)
            ystep *= -1;
        if (param->rays[i].isRayFacingDown && ystep < 0)
            ystep *= -1;
        if (param->rays[i].isRayFacingLeft)
            px = 1;
        while (xintercept >= 0 && xintercept < param->data.window_width \
                && yintercept >= 0 && yintercept < param->data.window_height )
        {
            if (param->map[(int)yintercept / TILE_SIZE][((int)xintercept / TILE_SIZE) - px] == '1')
            {
                foundVwallhit = 1;
                Vhitx = xintercept;
                Vhity = yintercept;
                break;
            }
            yintercept += ystep;
            xintercept += xstep;
        }
        //compare the distance between the two points
        if (foundHwallhit)
            Hdistance = distance(param->player.x, param->player.y, Hhitx, Hhity);
        else 
            Hdistance = INT_MAX;
        if (foundVwallhit)
            Vdistance = distance(param->player.x, param->player.y, Vhitx, Vhity);
        else 
            Vdistance = INT_MAX;
        
        if (Hdistance < Vdistance)
        {
            param->rays[i].distance = Hdistance;
            param->rays[i].wallHitX = Hhitx;
            param->rays[i].wallHitY = Hhity;
        }
        else
        {
            param->rays[i].distance = Vdistance;
            param->rays[i].wallHitX = Vhitx;
            param->rays[i].wallHitY = Vhity;
        }
        // printf("after rayAngle: %f\n", rayAngle);
        draw_line(param->player.x * MINIMAP_SCALE_FACTOR, param->player.y * MINIMAP_SCALE_FACTOR,  param->rays[i].wallHitX * MINIMAP_SCALE_FACTOR,  param->rays[i].wallHitY * MINIMAP_SCALE_FACTOR, &param->player, &param->data, PINK);
        // printf("rayAngle: %f\n", rayAngle);
        rayAngle += (double)(FOV_ANGLE) / (double)(NUM_RAYS);
        i++;
    }
    // printf("player angle: %f\n", param->player.rotationAngle);
}

int ft_get_pixel(t_img_info *img, int x, int y)
{
    return *(int *)(img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8)));
}

void copy_pixl_img2img(t_img_info *dst, t_img_info *src)
{
    int i;
    int j;
    int color;

    i = 0;
    while (i < src->window_height)
    {
        j = 0;
        while (j < src->window_width)
        {
            color = ft_get_pixel(src, j, i);
            ft_put_pixel(dst, j, i, color);
            j++;
        }
        i++;
    }
}

int render_next_frame(void *var)
{
    t_param *param;

    param = (t_param *)var;

    param->data.img = param->img3d.img;
    param->data.addr = param->img3d.addr;
    param->data.bits_per_pixel = param->img3d.bits_per_pixel;
    param->data.line_length = param->img3d.line_length;
    param->data.endian = param->img3d.endian;
    castAllrays(param);
    render_3d_walls(param);
    // copy_pixl_img2img(&param->img3d, &param->data);
    mlx_put_image_to_window(param->mlx, param->mlx_win, param->img3d.img, 0 , 0);
    // mlx_put_image_to_window(param->mlx, param->mlx_win, param->data.img, 0 , 0);
    render_wall(param->map, &param->data);
    render_floor(param->map, &param->data);
    render_p(param->map, &param->data, &param->player);
    mlx_destroy_image(param->mlx, param->img3d.img);
    param->img3d.img = mlx_new_image(param->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);  
    param->img3d.addr = mlx_get_data_addr(param->img3d.img, &param->img3d.bits_per_pixel, &param->img3d.line_length, &param->img3d.endian);
    param->img3d.window_height = WINDOW_HEIGHT;
    param->img3d.window_width = WINDOW_WIDTH;
    return 0;
}