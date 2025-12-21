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

	/*
	Ignore this

	JSimplON_Array *root_array = jsimplon_get_array(jsimplon);
	size_t root_array_size = jsimplon_get_array_size(root_array);

	for (uint32_t i = 0; i < root_array_size; ++i) {
		JSimplON_Object object = jsimplon_get_array_index(root_array, i);

		printf("%s\n",  jsimplon_get_object_member_str(object, "Name"));
		printf("%s\n",  jsimplon_get_object_member_str(object, "Sex"));
		printf("%lf\n", jsimplon_get_object_member_number(object, "Age"));
		printf("%s\n",  jsimplon_get_object_member_str(object, "Ethnicity"));

		JSimplON_Array *bros = jsimplon_get_object_member_array(object, "Bros");
		size_t bros_array_size = jsimplon_get_array_size(bros);
		for (uint32_t j = 0; j < bros_array_size; ++i)
			printf("%s\n", jsimplon_get_array_index(bros, j);
	}
	*/

	jsimplon_destroy(jsimplon);

	return 0;
}
