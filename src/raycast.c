#include "./headers/maze.h"

/**
* ray_cast - cast rays and render walls, ceiling and floor
 * @instance: sdl instance
 * @maze_map: pointer to 2D maze map
 *
 * Return: nothing
*/
void ray_cast(SDL_Instance *instance, int *maze_map)
{
	double cameraX, dist_to_wall;
	point ray_pos, ray_dir, pos_to_next, dist_to_next;
	SDL_Point map, step;
	int hit, side, x;

	for (x = 0; x < SCREEN_WIDTH; x++)
	{
		cameraX = 2 * x / (double)(SCREEN_WIDTH) - 1;
		ray_pos.x = pos.x, ray_pos.y = pos.y;
		ray_dir.x = dir.x + plane.x * cameraX;
		ray_dir.y = dir.y + plane.y * cameraX;
		map.x = (int)(ray_pos.x), map.y = (int)(ray_pos.y);

		dist_to_next.x = sqrt(
				1 + (pow(ray_dir.y, 2)) / pow(ray_dir.x, 2));
		dist_to_next.y = sqrt(
				1 + (pow(ray_dir.x, 2)) / pow(ray_dir.y, 2));

		if (ray_dir.x < 0)
		{
			step.x = -1;
			pos_to_next.x = (ray_pos.x - map.x) * dist_to_next.x;
		}
		else
		{
			step.x = 1;
			pos_to_next.x = (map.x + 1.0 - ray_pos.x) *
				dist_to_next.x;
		}

		if (ray_dir.y < 0)
		{
			step.y = -1;
			pos_to_next.y = (ray_pos.y - map.y) *
				dist_to_next.y;
		}
		else
		{
			step.y = 1;
			pos_to_next.y = (map.y + 1.0 - ray_pos.y) *
				dist_to_next.y;
		}

		hit = 0;
		while (hit == 0)
		{
			if (pos_to_next.x < pos_to_next.y)
			{
				pos_to_next.x += dist_to_next.x;
				map.x += step.x;
				side = 0;
			}
			else
			{
				pos_to_next.y += dist_to_next.y;
				map.y += step.y;
				side = 1;
			}

			/* check if rays hits a wall */
			if (*((int *)maze_map + map.x * MAP_WIDTH + map.y) > 0)
				hit = 1;
		}

		/* calculate distance to the wall in camera direction */
		if (side == 0)
			dist_to_wall = (map.x - ray_pos.x +
					(1 - step.x) / 2) / ray_dir.x;
		else
			dist_to_wall = (map.y - ray_pos.y +
					(1 - step.y) / 2) / ray_dir.y;

		/* render walls */
		render_walls(instance, maze_map, map, ray_pos, ray_dir,
				dist_to_wall, x, side);
	}
	/* refresh render with the update buffer */
	update_renderer(instance);
}
