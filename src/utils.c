#include "../inc/raycaster.h"


double deg2rad(double x)
{
	return (x * (M_PI / 180.0));
}

void ft_put_pixel(t_data *data, int x, int y, int color)
{
    char *pxl;

    if (x >= 0 && x < data->window_weight && y >= 0 && y < data->window_height )
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

void render_tiles(t_data *data, int x, int y, int color)
{
    int i[2];

    i[0] = 0;
    i[1] = 0;
    while (i[0] < TILE_SIZE)
    {
        i[1] = 0;
        while (i[1] < TILE_SIZE)
        {
            if (i[0] == 0 || i[1] == 0 || i[0] == data->window_height || i[1] == data->window_weight)
               ft_put_pixel(data, x + i[1], y + i[0], 0x00000000);
            else
                ft_put_pixel(data, x + i[1], y + i[0], color);
            i[1]++;
        }
        i[0]++;
    }
}

void render_wall(char **map, t_data *data)
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
                render_tiles(data, i[1] * TILE_SIZE, i[0] * TILE_SIZE, 0x00000000);
            i[1]++;
        }
        i[0]++;
    }
}

void render_floor(char **map, t_data *data)
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
                render_tiles(data, i[1] * TILE_SIZE, i[0] * TILE_SIZE, 0x00FFFFFF);
            i[1]++;
        }
        i[0]++;
    }
}

// void draw_circle(int x, int y, int r, t_data *data)
// {
   
// }

void render_p(char **map, t_data *data, t_player *p)
{
    int d;

    d = p->radius*2;
    for (int i = 0; i < d; i++)
        for (int j = 0; j < d; j++)
            if (pow(j-p->radius, 2) + pow(i-p->radius,2) <= pow(p->radius,2))
                ft_put_pixel(data, j+p->x-p->radius, i+p->y-p->radius, 0x00FF0000);
}


void flood_fill(char **map, int x, int y, int h, int w, t_param par)
{
    if (x < 0 || y < 0 || y > h || x > w || map[y][x] == '1' || map[y][x] == 'D')
        return ;
    // render_tiles(par.mlx, par.mlx_win, x* TILE_SIZE, y* TILE_SIZE, 0x000000FF);
    if (map[y][x] == 'P' || map[y][x] == '0')
        map[y][x] = 'D';
    // render_d(map, par.mlx_win, par.mlx);
    flood_fill(map, x+1, y, h, w, par);
    flood_fill(map, x-1, y, h, w, par);
    flood_fill(map, x, y+1, h, w, par);
    flood_fill(map, x, y-1, h, w, par);
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