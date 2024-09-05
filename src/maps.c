#include "./headers/maze.h"

/**
 * read_map - reads a file and creates a map for the maze
 * @file_name: file name to read map data
 *
 * Return: pointer to map or NULL
*/
int *read_map(char *filename)
{
	FILE *file;
	int *map;
	char *token;
	char row[MAP_WIDTH * 2];
	int a, b;

	file = fopen(filename, "r");
	if (!file)
	{
		fprintf(stderr, "Error opening map file: %s\nExiting...\n",
				filename);
		return (NULL);
	}

	map = malloc(MAP_WIDTH * MAP_HEIGHT * sizeof(int));
	if (!map)
	{
		fprintf(stderr, "Malloc failed. Exiting...\n");
		return (NULL);
	}

	a = 0;

	while (fgets(row, sizeof(row), file) != NULL)
	{
		b = 0;
		if (strlen(row) <= 1)
			continue;

		token = strtok(row, ",;\n ");
		
		while (token != NULL)
		{
			map[a * MAP_WIDTH + b] = atoi(token);
			token = strtok(NULL, ",;\n ");
			b++;
		}
		a++;
	}

	fclose(file);
	return (map);	
}
