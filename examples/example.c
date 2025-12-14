#define JSIMPLON_IMPLEMENTATION
#include "jsimplon.h"

int main(int argc, char **argv)
{
	if (argc < 2)
		return 1;

	JSimplON data;
	
	if (!jsimplon_parse_file(&data, argv[1]))
		printf("jsimplon error: %s\n", jsimplon_get_error());

	return 0;
}
