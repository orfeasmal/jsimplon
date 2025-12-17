#define JSIMPLON_IMPLEMENTATION
#include "jsimplon.h"

int main(int argc, char **argv)
{
	if (argc < 2)
		return 1;

	char *error;
	JSimplON *jsimplon = jsimplon_create_from_file(&error, argv[1]);

	if (error != NULL) {
		printf("jsimplon error: %s\n", error);
		free(error);
		return 1;
	}

	return 0;
}
