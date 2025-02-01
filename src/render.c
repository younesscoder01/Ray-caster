#include "../includes/raycaster.h"


double deg2rad(double x)
{
	return (x * (M_PI / 180.0));
}

void ft_put_pixel(t_img_info *data, int x, int y, int color)
{
    char *pxl;

    if (x >= 0 && x < data->img_width && y >= 0 && y < data->img_height)
    {
        pxl = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
        *(unsigned int *)pxl = color;
    }
}

size_t get_map_weight(char **map_name)
{
    return (strlen(map_name[0]));
}

size_t get_map_height(char **map_name)
{
    int i;
    
    i = 0;
    while (map_name[i])
        i++;
    return (i);
}

char **create_map(int h, char *file_name)
{
    int fd;
    char *line;
    int i;
    char **map;

    fd = open(file_name, O_RDONLY);
    if (fd < 0)
        return (NULL);
    map = malloc(sizeof(char *) * (h));
    line = get_next_line(fd);
    i = 0;
    while (line)
    {
        map[i] = line;
        if (map[i][strlen(map[i]) - 1] == '\n')
            map[i][strlen(map[i]) - 1] = 0;
        if (map[i][0] == 0)
            map[i] = NULL;
        i++;
        line = get_next_line(fd);
    }
    map[i] = NULL;
    return map;
}

void render_tiles(t_img_info *data, int x, int y, int color)
{
    int i[2];

    i[0] = 0;
    i[1] = 0;
    while (i[0] < TILE_SIZE * MINIMAP_SCALE_FACTOR)
    {
        i[1] = 0;
        while (i[1] < TILE_SIZE * MINIMAP_SCALE_FACTOR)
        {
            // if (i[0] == 0 || i[1] == 0 || i[0] == data->window_height || i[1] == data->window_width)
            //    ft_put_pixel(data, x + i[1], y + i[0], BLACK);
            // else
                ft_put_pixel(data, x + i[1], y + i[0], color);
            i[1]++;
        }
        i[0]++;
    }
}

void render_wall(char **map, t_img_info *data)
{
    int i[2];

    i[0] = 0;
    i[1] = 0;
    while (map[i[0]])
    {
        i[1] = 0;
        while (map[i[0]][i[1]])
        {
            if ( map[i[0]][i[1]] == '1')
                render_tiles(data, i[1] * TILE_SIZE * MINIMAP_SCALE_FACTOR, i[0] * TILE_SIZE * MINIMAP_SCALE_FACTOR, ORANGE);
            i[1]++;
        }
        i[0]++;
    }
}

void render_floor(char **map, t_img_info *data)
{
    int i[2];

    i[0] = 0;
    i[1] = 0;
    while (map[i[0]])
    {
        i[1] = 0;
        while (map[i[0]][i[1]])
        {
            if ( map[i[0]][i[1]] == '0' || map[i[0]][i[1]] == 'P')
                render_tiles(data, i[1] * TILE_SIZE * MINIMAP_SCALE_FACTOR, i[0] * TILE_SIZE * MINIMAP_SCALE_FACTOR, WHITE);
            i[1]++;
        }
        i[0]++;
    }
}

void draw_line(double x, double y, double x1, double y1, t_player *p, t_img_info *data, int color)
{
    double dx;
    double dy;
    double xinc;
    double yinc;
    double steps;
    int i = 0;

    dx = x1 - x;
    dy = y1 - y;
    if (fabs(dx) > fabs(dy))
        steps = fabs(dx);
    else 
        steps = fabs(dy);
    xinc = dx / steps;
    yinc = dy / steps;
    while (i <= steps)
    {
        ft_put_pixel(data, x, y, color);
        x += xinc;
        y += yinc;
        i++;
    }
}

void render_p(char **map, t_img_info *data, t_player *p)
{
    int d;
    double x1;
    double y1;

    d = p->radius*2;
    for (int i = 0; i < d; i++)
        for (int j = 0; j < d; j++)
            if (pow(j - p->radius, 2) + pow(i - p->radius,2) <= pow(p->radius,2))
                ft_put_pixel(data, (j+p->x-p->radius)  * MINIMAP_SCALE_FACTOR  , (i+p->y-p->radius)  * MINIMAP_SCALE_FACTOR, RED);
    x1 = p->x + cos(deg2rad(p->rotationAngle)) * 20;
    y1 = p->y + sin(deg2rad(p->rotationAngle)) * 20;
    draw_line(p->x * MINIMAP_SCALE_FACTOR, p->y * MINIMAP_SCALE_FACTOR, x1 * MINIMAP_SCALE_FACTOR, y1 * MINIMAP_SCALE_FACTOR, p, data, RED);
}

int count_file_lines(char *file_name)
{
    int fd;
    char *line;
    int i;

    fd = open(file_name, O_RDONLY);
    if (fd < 0)
        return (0);
    i = 0;
    line = get_next_line(fd);
    while (line)
    {
        i++;
        // if (line != NULL && line[0] == '\n')
        //     return i;
        line = get_next_line(fd);
    }
    printf("the number of lines is %i\n", i);
    return (i);
}

int get_player_x(char **map)
{
    for (int i = 0; map[i]; i++)
    {
        for (int j = 0; map[i][j]; j++)
        {
            if (map[i][j] == 'P')
                return j;
        }
    }
    return 1;
}

int get_player_y(char **map)
{
    for (int i = 0; map[i]; i++)
    {
        for (int j = 0; map[i][j]; j++)
        {
            if (map[i][j] == 'P')
                return i;
        }
    }
    return 1;
}

void remove_p(char **map)
{
    for (int i = 0; map[i]; i++)
    {
        for (int j = 0; map[i][j]; j++)
        {
            if (map[i][j] == 'P')
                map[i][j] = '0';
        }
    }
}