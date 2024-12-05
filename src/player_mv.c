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
    param->player.rotationAngle = normalizeAngle(param->player.rotationAngle);
    moveStep = param->player.walkDirection * param->player.moveSpeed;
    check_x = param->player.x + round(cos(deg2rad(param->player.rotationAngle)) * moveStep);
    check_y = param->player.y + round(sin(deg2rad(param->player.rotationAngle)) * moveStep);
    if (param->map[param->player.y / TILE_SIZE][check_x / TILE_SIZE] != '1')
        param->player.x = check_x;
    if (param->map[check_y / TILE_SIZE][param->player.x / TILE_SIZE] != '1')
        param->player.y = check_y;
    printf("x: %i\ny: %i\n", param->player.x, param->player.y);
    return 0;
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
        while (xintercept >= 0 && xintercept < param->data.window_weight \
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
        while (xintercept >= 0 && xintercept < param->data.window_weight \
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
        draw_line(param->player.x, param->player.y,  param->rays[i].wallHitX,  param->rays[i].wallHitY, &param->player, &param->data, PINK);
        // printf("rayAngle: %f\n", rayAngle);
        rayAngle += (double)(FOV_ANGLE) / (double)(NUM_RAYS);
        i++;
    }
    printf("player angle: %f\n", param->player.rotationAngle);
}


int render_next_frame(void *var)
{
    t_param *param;

    param = (t_param *)var;

    render_wall(param->map, &param->data);
    render_floor(param->map, &param->data);
    castAllrays(param);
    render_p(param->map, &param->data, &param->player);
    mlx_put_image_to_window(param->mlx, param->mlx_win, param->data.img, 0 , 0);
    return 0;
}