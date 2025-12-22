#define JSIMPLON_IMPLEMENTATION
#include "jsimplon.h"

int main(int argc, char **argv)
{
	if (argc < 2)
		return 1;

	char *error;
	Jsimplon_Value *root_value = jsimplon_tree_from_file(&error, argv[1]);

	if (error != NULL) {
		printf("jsimplon error: %s\n", error);
		free(error);

		return 1;
	}

	Jsimplon_Array *root_array = jsimplon_get_value_array(root_value);
	size_t root_array_count = jsimplon_get_array_count(root_array);

	// There is also jsimplon_get_value_type(value);

	for (uint32_t i = 0; i < root_array_count; ++i) {
		Jsimplon_Value *value = jsimplon_get_array_index(root_array, i);
		Jsimplon_Object *object = jsimplon_get_value_object(value);

		printf("%s\n",  jsimplon_get_object_member_str(object, "Name"));
		printf("%s\n",  jsimplon_get_object_member_str(object, "Sex"));
		printf("%lf\n", jsimplon_get_object_member_number(object, "Age"));
		printf("%s\n",  jsimplon_get_object_member_str(object, "Ethnicity"));

		bool alive = jsimplon_get_object_member_bool(object, "Alive");
		if (alive)
			printf("true\n");
		else
			printf("false\n");

		Jsimplon_Array *bros = jsimplon_get_object_member_array(object, "Bros");
		size_t bros_array_count = jsimplon_get_array_count(bros);
		for (uint32_t j = 0; j < bros_array_count; ++j) {
			Jsimplon_Value *value = jsimplon_get_array_index(bros, j);
			printf("%s\n", jsimplon_get_value_str(value));
		}

		/*
			Alternatively,

			Jsimplon_Value value = jsimplon_get_object_member_value(object, "Key");
			double thing = jsimplon_get_value_number(value);
		*/
	}

	jsimplon_tree_destroy(root_value);

	return 0;
}
