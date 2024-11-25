#include "../inc/raycaster.h"


int key_p(int keycode, void *var)
{
    t_param *param;

    param = (t_param *)var;
    if (keycode == 119)
        param->player.y -= 3;
    else if (keycode == 115)
        param->player.y += 3;
    printf("%i\n", keycode);
}

// void move_up( t_param *param)
// {
//     if (param->player.x > 0)
//         ;
// }

int render_next_frame(void *var)
{
    t_param *param;

    param = (t_param *)var;
    render_wall(param->map, &param->data);
    render_floor(param->map, &param->data);
    render_p(param->map, &param->data, &param->player);
    mlx_put_image_to_window(param->mlx, param->mlx_win, param->data.img, 0 , 0);
}