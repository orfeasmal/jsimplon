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

	Jsimplon_Array *root_array = jsimplon_value_get_array(root_value);
	size_t root_array_count = jsimplon_array_get_count(root_array);

	// There is also jsimplon_value_get_type(value);

	for (uint32_t i = 0; i < root_array_count; ++i) {
		Jsimplon_Value *value = jsimplon_array_get_value_at_index(root_array, i);
		Jsimplon_Object *object = jsimplon_value_get_object(value);

		printf("%s\n",  jsimplon_object_member_get_str(object, "Name"));
		printf("%s\n",  jsimplon_object_member_get_str(object, "Sex"));
		printf("%lf\n", jsimplon_object_member_get_number(object, "Age"));
		printf("%s\n",  jsimplon_object_member_get_str(object, "Ethnicity"));

		bool alive = jsimplon_object_member_get_bool(object, "Alive");
		if (alive)
			printf("true\n");
		else
			printf("false\n");

		Jsimplon_Array *bros = jsimplon_object_member_get_array(object, "Bros");
		size_t bros_array_count = jsimplon_array_get_count(bros);
		for (uint32_t j = 0; j < bros_array_count; ++j) {
			Jsimplon_Value *value = jsimplon_array_get_value_at_index(bros, j);
			printf("%s\n", jsimplon_value_get_str(value));
		}

		/*
			Alternatively,

			Jsimplon_Value value = jsimplon_object_member_get_value(object, "Key");
			double thing = jsimplon_value_get_number(value);
		*/
	}

	jsimplon_tree_destroy(root_value);

	const char *name = "Bro2";
	const char *sex = "Male";
	double age = 13.1;
	const char *ethnicity = "Colombian";
	const char *bros[] = {"John Doe", "Bro1", "Bro3"};
	bool alive = true;

	root_value = jsimplon_tree_root_create();

	Jsimplon_Object *object = jsimplon_value_set_object(root_value);
	jsimplon_object_add_member_str(object, "Name", name);
	jsimplon_object_add_member_str(object, "Sex", sex);
	jsimplon_object_add_member_number(object, "Age", age);
	jsimplon_object_add_member_str(object, "Ethnicity", ethnicity);
	jsimplon_object_add_member_bool(object, "Alive", alive);

	Jsimplon_Array *bros_array = jsimplon_object_add_member_array(object, "Bros");
	for (uint32_t i = 0; i < 3; ++i) {
		Jsimplon_Value *temp = jsimplon_array_push_value(bros_array);
		jsimplon_value_set_str(temp, bros[i]);
	}

	jsimplon_tree_to_file(&error, root_value, "Bro2.json");
	if (error != NULL) {
		fprintf(stderr, "jsimplon error: %s\n", error);
		free(error);
		jsimplon_tree_destroy(root_value);
		return 1;
	}

	jsimplon_tree_destroy(root_value);

	return 0;
}
