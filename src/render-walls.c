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

unsigned int darken_color(unsigned int color, double factor) {
    if (factor < 0.0) 
        factor = 0.0;
    if (factor > 1.0)
        return color;
    unsigned char r = (color >> 16) & 0xFF; // Extract red
    unsigned char g = (color >> 8) & 0xFF;  // Extract green
    unsigned char b = color & 0xFF;         // Extract blue

    // Apply factor and clamp to [0, 255]
    r = (unsigned char)(r * (1.0 - factor));
    g = (unsigned char)(g * (1.0 - factor));
    b = (unsigned char)(b * (1.0 - factor));

    // Combine back into ARGB format (assuming alpha remains the same)
    return (color & 0xFF000000) | (r << 16) | (g << 8) | b;
}


void render_3d_walls(t_param *param)
{
    double distanceProjPlane;
    double wallStripHeight;
    double correctDistance;
    int shade_color;
    int color;
    double alpha;

    int i;

    i = 0;
    correctDistance = 0;
    color = RED;
    while (i < NUM_RAYS)
    {
        distanceProjPlane = ((double)WINDOW_WIDTH / 2.0) / tan(deg2rad(FOV_ANGLE / 2));
        correctDistance = param->rays[i].distance * cos(deg2rad(param->rays[i].rayAngle) - deg2rad(param->player.rotationAngle));

        alpha = 1.0 - (80.0 / correctDistance);
        // if (param->rays[i].wasHitVertical != 1)
        //     color = RED;
        // else 
        //     color = B_RED;
        shade_color = darken_color(color, alpha);
        wallStripHeight = ((double)TILE_SIZE / correctDistance) * distanceProjPlane;
        rect(&param->img3d, i * WALL_STRIP_WIDTH, (WINDOW_HEIGHT / 2) - ((int)wallStripHeight / 2), WALL_STRIP_WIDTH, (int)wallStripHeight, shade_color);
        i++;
    }
}