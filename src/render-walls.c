#include "../includes/raycaster.h"


void rect(t_img_info *data, int x, int y, int width, int height, int color)
{
    int i;
    int j;

    i = 0;
    while (i < height)
    {
        j = 0;
        while (j < width)
        {
            ft_put_pixel(data, x + j, y + i, color);
            j++;
        }
        i++;
    }
}

void render_3d(t_param *param)
{
    double distanceProjPlane;
    double wallStripHeight;
    int i;

    i = 0;
    while (i < NUM_RAYS)
    {
        distanceProjPlane = ((double)WINDOW_WIDTH / 2.0) / tan(deg2rad(FOV_ANGLE / 2));
        wallStripHeight = ((double)TILE_SIZE / param->rays[i].distance) * distanceProjPlane;
        rect(&param->img3d, i * WALL_STRIP_WIDTH, (WINDOW_HEIGHT / 2) - (wallStripHeight / 2), WALL_STRIP_WIDTH, wallStripHeight, RED);
        i++;
    }
}