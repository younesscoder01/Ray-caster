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


// void render_3d_walls(t_param *param)
// {
//     double distanceProjPlane;
//     double wallStripHeight;
//     double correctDistance;
//     int shade_color;
//     int color;
//     double alpha;

//     int i;

//     i = 0;
//     correctDistance = 0;
//     color = RED;
//     while (i < NUM_RAYS)
//     {
//         distanceProjPlane = ((double)WINDOW_WIDTH / 2.0) / tan(deg2rad(FOV_ANGLE / 2));
//         correctDistance = param->rays[i].distance * cos(deg2rad(param->rays[i].rayAngle) - deg2rad(param->player.rotationAngle));

//         alpha = 1.0 - (80.0 / correctDistance);
//         // if (param->rays[i].wasHitVertical != 1)
//         //     color = RED;
//         // else 
//         //     color = B_RED;
//         shade_color = darken_color(color, alpha);
//         wallStripHeight = ((double)TILE_SIZE / correctDistance) * distanceProjPlane;
//         rect(&param->img3d, i * WALL_STRIP_WIDTH, (WINDOW_HEIGHT / 2) - ((int)wallStripHeight / 2), WALL_STRIP_WIDTH, (int)wallStripHeight, shade_color);
//         i++;
//     }
// }


unsigned int	get_texture_pixel(t_img_info *img, int x, int y)
{
	char	*pxl;

	pxl = 0;
	if (x >= 0 && x < img->img_width && y >= 0 && y < img->img_height)
	{
		pxl = img->addr + (y * img->line_length + x * (img->bits_per_pixel
					/ 8));
	}
	if (pxl == 0)
		return (6);
	return (*(unsigned int *)pxl);
}

void render_3d_walls(t_param *param)
{
    double distanceProjPlane;
    double wallStripHeight;
    double correctDistance;
    int texture_x;
    int texture_y;
    int color;
    t_img_info img;

    int i = 0;
    int w = 64;
    int h = 64;
    img.img = mlx_xpm_file_to_image(param->mlx, "./BR1.xpm", &w, &h);
    if (img.img == NULL)
    {
        printf("Error:\n");
        exit(99);
    }
    img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
    img.img_height = 64;
    img.img_width = 64;
    while (i < NUM_RAYS)
    {
        // Determine texture color based on hit type
        if (param->rays[i].wasHitVertical)
            texture_x = (int)(param->rays[i].wallHitY) % TILE_SIZE;
        else
            texture_x = (int)(param->rays[i].wallHitX) % TILE_SIZE;

        // Scale texture_x to match texture width
        texture_x = (texture_x * TEXTURE_SIZE) / TILE_SIZE;

        // Correct distance to avoid fisheye distortion
        distanceProjPlane = ((double)WINDOW_WIDTH / 2.0) / tan(deg2rad(FOV_ANGLE / 2));
        correctDistance = param->rays[i].distance * cos(deg2rad(param->rays[i].rayAngle) - deg2rad(param->player.rotationAngle));

        // Compute wall height
        wallStripHeight = ((double)TILE_SIZE / correctDistance) * distanceProjPlane;
        int y_top = (WINDOW_HEIGHT / 2) - ((int)wallStripHeight / 2);
        int y_low = y_top + wallStripHeight;

        // Loop through each vertical pixel to apply texture mapping
        for (int y = y_top; y < y_low; y++)
        {
            int d = y - y_top;
            texture_y = (d * TEXTURE_SIZE) / wallStripHeight;

            // Fetch color from texture
            color = get_texture_pixel(&img, texture_x, texture_y);
            ft_put_pixel(&param->img3d, i * WALL_STRIP_WIDTH, y, color);
        }
        i++;
    }
}
