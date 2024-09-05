#include "./headers/maze.h"

/**
 * print_help - print help message
*/
void print_help(char **argv)
{
	printf("Usage: %s [map_name] [options]\n\n", argv[0]);
	printf("Available ptions:\n\n");
	printf("  help    ->  Print this help message.\n");
	printf("  textured -> Specify whether to load textures.\n\n");
	printf("  no-textures -> Disable textures\n");
}
