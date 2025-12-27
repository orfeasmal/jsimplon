/*
	Copyright (C) 2025 Orfeas Malliamanis <orfeas@orfeasmal.com>

	This program is free software: you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free Software
	Foundation, either version 3 of the License, or (at your option) any later
	version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY
	WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
	PARTICULAR PURPOSE. See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along with
	this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef JSIMPLON_H_
#define JSIMPLON_H_

#ifndef JSIMPLON_DEF
#define JSIMPLON_DEF static inline
#endif // JSIMPLON_DEF

typedef struct jsimplon_value Jsimplon_Value;
typedef struct jsimplon_object Jsimplon_Object;
typedef struct jsimplon_member Jsimplon_Member;
typedef struct jsimplon_array Jsimplon_Array;

typedef enum {
	JSIMPLON_VALUE_UNINITIALISED,
	JSIMPLON_VALUE_OBJECT,
	JSIMPLON_VALUE_ARRAY,
	JSIMPLON_VALUE_STRING,
	JSIMPLON_VALUE_NUMBER,
	JSIMPLON_VALUE_BOOL,
	JSIMPLON_VALUE_NULL
} Jsimplon_ValueType;

/* API Functions */

// error is opt-out so you can pass in NULL if you don't care about the message
// it also needs to be freed
JSIMPLON_DEF Jsimplon_Value *jsimplon_tree_root_create(void);
JSIMPLON_DEF Jsimplon_Value *jsimplon_tree_from_str(char **error, const char *src);
JSIMPLON_DEF Jsimplon_Value *jsimplon_tree_from_file(char **error, const char *file_name);
JSIMPLON_DEF char *          jsimplon_tree_to_str(char **error, const Jsimplon_Value *root_value);
JSIMPLON_DEF int             jsimplon_tree_to_file(char **error, const Jsimplon_Value *root_value, const char *file_name);
JSIMPLON_DEF int             jsimplon_tree_destroy(Jsimplon_Value *root_value);

#ifndef JSIMPLON_SUCCESS
#define JSIMPLON_SUCCESS 0
#endif // JSIMPLON_SUCCESS

#ifndef JSIMPLON_FAILURE
#define JSIMPLON_FAILURE 1
#endif // JSIMPLON_FAILURE

/* Setters */

JSIMPLON_DEF int              jsimplon_value_set_str(Jsimplon_Value *value, const char *str);
JSIMPLON_DEF int              jsimplon_value_set_number(Jsimplon_Value *value, double number);
JSIMPLON_DEF int              jsimplon_value_set_bool(Jsimplon_Value *value, bool bool_value);
JSIMPLON_DEF int              jsimplon_value_set_null(Jsimplon_Value *value);
JSIMPLON_DEF Jsimplon_Object *jsimplon_value_set_object(Jsimplon_Value *value);
JSIMPLON_DEF Jsimplon_Array * jsimplon_value_set_array(Jsimplon_Value *value);

JSIMPLON_DEF Jsimplon_Member *jsimplon_object_add_member(Jsimplon_Object *object);
JSIMPLON_DEF Jsimplon_Value * jsimplon_object_add_member_value(Jsimplon_Object *object, const char *key);
JSIMPLON_DEF int              jsimplon_object_add_member_str(Jsimplon_Object *object, const char *key, const char *str);
JSIMPLON_DEF int              jsimplon_object_add_member_number(Jsimplon_Object *object, const char *key, double number);
JSIMPLON_DEF int              jsimplon_object_add_member_bool(Jsimplon_Object *object, const char *key, bool bool_value);
JSIMPLON_DEF int              jsimplon_object_add_member_null(Jsimplon_Object *object, const char *key);
JSIMPLON_DEF Jsimplon_Object *jsimplon_object_add_member_object(Jsimplon_Object *object, const char *key);
JSIMPLON_DEF Jsimplon_Array * jsimplon_object_add_member_array(Jsimplon_Object *object, const char *key);
JSIMPLON_DEF int              jsimplon_object_remove_member(Jsimplon_Object *object, const char *key);

JSIMPLON_DEF int              jsimplon_member_set_key(Jsimplon_Member *member, const char *new_key);
JSIMPLON_DEF Jsimplon_Value * jsimplon_member_set_value(Jsimplon_Member *member); // Kind of useless
JSIMPLON_DEF int              jsimplon_member_set_str(Jsimplon_Member *member, const char *str);
JSIMPLON_DEF int              jsimplon_member_set_number(Jsimplon_Member *member, double number);
JSIMPLON_DEF int              jsimplon_member_set_bool(Jsimplon_Member *member, bool bool_value);
JSIMPLON_DEF int              jsimplon_member_set_null(Jsimplon_Member *member);
JSIMPLON_DEF Jsimplon_Object *jsimplon_member_set_object(Jsimplon_Member *member);
JSIMPLON_DEF Jsimplon_Array * jsimplon_member_set_array(Jsimplon_Member *member);

JSIMPLON_DEF Jsimplon_Value * jsimplon_array_push_value(Jsimplon_Array *array);
JSIMPLON_DEF int              jsimplon_array_push_str(Jsimplon_Array *array, const char *str);
JSIMPLON_DEF int              jsimplon_array_push_number(Jsimplon_Array *array, double number);
JSIMPLON_DEF int              jsimplon_array_push_bool(Jsimplon_Array *array, bool bool_value);
JSIMPLON_DEF int              jsimplon_array_push_null(Jsimplon_Array *array);
JSIMPLON_DEF Jsimplon_Object *jsimplon_array_push_object(Jsimplon_Array *array);
JSIMPLON_DEF Jsimplon_Array * jsimplon_array_push_array(Jsimplon_Array *array);
JSIMPLON_DEF Jsimplon_Value * jsimplon_array_insert_value_at_index(Jsimplon_Array *array, size_t index);
JSIMPLON_DEF int              jsimplon_array_remove_value_at_index(Jsimplon_Array *array, size_t index);

/* Getters */

// If a getter that returns a pointer fails, it will return NULL
// If a getter that doesn't return a pointer fails, I don't know

JSIMPLON_DEF Jsimplon_ValueType jsimplon_value_get_type(Jsimplon_Value *value);
JSIMPLON_DEF Jsimplon_Object *  jsimplon_value_get_object(Jsimplon_Value *value);
JSIMPLON_DEF Jsimplon_Array *   jsimplon_value_get_array(Jsimplon_Value *value);
JSIMPLON_DEF const char *       jsimplon_value_get_str(Jsimplon_Value *value);
JSIMPLON_DEF double             jsimplon_value_get_number(Jsimplon_Value *value); // returns infinity if failed
JSIMPLON_DEF int                jsimplon_value_get_bool(Jsimplon_Value *value); // returns -1 if failed

JSIMPLON_DEF Jsimplon_Member *  jsimplon_object_get_member(Jsimplon_Object *object, const char *key);
JSIMPLON_DEF size_t             jsimplon_object_member_get_count(Jsimplon_Object *object);
JSIMPLON_DEF Jsimplon_Member *  jsimplon_object_member_get_at_index(Jsimplon_Object *object, size_t index);
JSIMPLON_DEF Jsimplon_ValueType jsimplon_object_member_get_type(Jsimplon_Object *object, const char *key);
JSIMPLON_DEF Jsimplon_Value *   jsimplon_object_member_get_value(Jsimplon_Object *object, const char *key);
JSIMPLON_DEF const char *       jsimplon_object_member_get_str(Jsimplon_Object *object, const char *key);
JSIMPLON_DEF double             jsimplon_object_member_get_number(Jsimplon_Object *object, const char *key); // returns infinity if failed
JSIMPLON_DEF int                jsimplon_object_member_get_bool(Jsimplon_Object *object, const char *key); // returns -1 if failed
JSIMPLON_DEF Jsimplon_Object *  jsimplon_object_member_get_object(Jsimplon_Object *object, const char *key);
JSIMPLON_DEF Jsimplon_Array *   jsimplon_object_member_get_array(Jsimplon_Object *object, const char *key);

JSIMPLON_DEF const char *       jsimplon_member_get_key(Jsimplon_Member *member);
JSIMPLON_DEF Jsimplon_Value *   jsimplon_member_get_value(Jsimplon_Member *member);
JSIMPLON_DEF const char *       jsimplon_member_get_str(Jsimplon_Member *member);
JSIMPLON_DEF double             jsimplon_member_get_number(Jsimplon_Member *member); // returns infinity if failed
JSIMPLON_DEF int                jsimplon_member_get_bool(Jsimplon_Member *member); // returns -1 if failed
JSIMPLON_DEF Jsimplon_Object *  jsimplon_member_get_object(Jsimplon_Member *member);
JSIMPLON_DEF Jsimplon_Array *   jsimplon_member_get_array(Jsimplon_Member *member);

JSIMPLON_DEF size_t             jsimplon_array_get_count(Jsimplon_Array *array);
JSIMPLON_DEF Jsimplon_Value *   jsimplon_array_get_value_at_index(Jsimplon_Array *array, size_t index);

#ifdef JSIMPLON_IMPLEMENTATION

#include <errno.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define JSIMPLON_DEF_INTERNAL static

typedef enum {
	JSIMPLON_TOKEN_END,
	JSIMPLON_TOKEN_STRING_LITERAL,
	JSIMPLON_TOKEN_NUMBER_LITERAL,
	JSIMPLON_TOKEN_TRUE,
	JSIMPLON_TOKEN_FALSE,
	JSIMPLON_TOKEN_NULL,
	JSIMPLON_TOKEN_LBRACE,
	JSIMPLON_TOKEN_RBRACE,
	JSIMPLON_TOKEN_LBRACKET,
	JSIMPLON_TOKEN_RBRACKET,
	JSIMPLON_TOKEN_COMMA,
	JSIMPLON_TOKEN_COLON
} Jsimplon_TokenType;

typedef struct {
	char *value; // NULL for things like { } , : and so on
	Jsimplon_TokenType type;
	uint32_t line, column;
} Jsimplon_Token;

#define JSIMPLON_STRING_LITERAL_MAX_LENGTH ((1 << 15) - 1)
#define JSIMPLON_NUMBER_LITERAL_MAX_LENGTH 21

typedef struct {
	const char *src;
	char *lexeme;
	size_t index;
	size_t begin_of_line;
	uint32_t line;

	char **error;
	size_t *error_size;
	uint32_t error_count;
} Jsimplon_Lexer;

typedef struct {
	Jsimplon_Lexer lexer;
	Jsimplon_Token token;
	uint32_t error_count;
	bool is_at_beginning;
} Jsimplon_Parser;

typedef struct {
	char **error;
	size_t *error_size;
	char *str;
	size_t str_size;
	uint32_t error_count;
} Jsimplon_Serialiser;

typedef struct jsimplon_object {
	Jsimplon_Member *members;
	size_t members_count;
	size_t members_size;
} Jsimplon_Object;

typedef struct jsimplon_array {
	Jsimplon_Value *values;
	size_t values_count;
	size_t values_size;
} Jsimplon_Array;

typedef struct jsimplon_value {
	union {
		char *          string_value;
		double          number_value;
		bool            bool_value;
		void *          null_value;
		Jsimplon_Object object_value;
		Jsimplon_Array  array_value;
	};

	Jsimplon_ValueType type;
} Jsimplon_Value;

typedef struct jsimplon_member {
	char *key;
	Jsimplon_Value value;
} Jsimplon_Member;

/* Parser functions */
JSIMPLON_DEF_INTERNAL Jsimplon_Value  jsimplon_parser_parse_value(Jsimplon_Parser *parser);
JSIMPLON_DEF_INTERNAL Jsimplon_Object jsimplon_parser_parse_object(Jsimplon_Parser *parser);
JSIMPLON_DEF_INTERNAL Jsimplon_Member jsimplon_parser_parse_member(Jsimplon_Parser *parser);
JSIMPLON_DEF_INTERNAL Jsimplon_Array  jsimplon_parser_parse_array(Jsimplon_Parser *parser);

/* Lexer functions */
JSIMPLON_DEF_INTERNAL Jsimplon_Token jsimplon_lexer_next_token(Jsimplon_Lexer *lexer);
JSIMPLON_DEF_INTERNAL const char *   jsimplon_token_to_str(Jsimplon_Token token);

/* Serialisation functions */
JSIMPLON_DEF_INTERNAL void jsimplon_value_to_str(Jsimplon_Serialiser *serialiser, const Jsimplon_Value *value);
JSIMPLON_DEF_INTERNAL void jsimplon_object_to_str(Jsimplon_Serialiser *serialiser, const Jsimplon_Object *object);
JSIMPLON_DEF_INTERNAL void jsimplon_array_to_str(Jsimplon_Serialiser *serialiser, const Jsimplon_Array *array);

/* Cleaning */
JSIMPLON_DEF_INTERNAL void jsimplon_value_destroy(Jsimplon_Value *value);
JSIMPLON_DEF_INTERNAL void jsimplon_object_destroy(Jsimplon_Object *object);
JSIMPLON_DEF_INTERNAL void jsimplon_member_destroy(Jsimplon_Member *member);
JSIMPLON_DEF_INTERNAL void jsimplon_array_destroy(Jsimplon_Array *array);

/* Utility functions */
JSIMPLON_DEF_INTERNAL void  jsimplon_append_str(char **str, size_t *str_size, const char *fmt, ...);
JSIMPLON_DEF_INTERNAL char *jsimplon_file_read(char **error, size_t *error_size, const char *file_name); // Returns NULL if failed
JSIMPLON_DEF_INTERNAL int   jsimplon_file_write(char **error, size_t *error_size, const char *file_name, const char *src); // Returns 0 if success anything else if failed

JSIMPLON_DEF Jsimplon_Value *jsimplon_tree_from_str(char **error, const char *src)
{
	Jsimplon_Value *tree = jsimplon_tree_root_create();

	size_t error_size = 0;
	if (error != NULL) {
		error_size = 1;
		*error = calloc(error_size, (sizeof *(*error)));
	}

	bool success = true;

	Jsimplon_Parser parser = {
		.lexer = {
			.src        = src,
			.error      = error,
			.error_size = &error_size,
			.lexeme     = malloc(JSIMPLON_STRING_LITERAL_MAX_LENGTH + 1),
			.line       = 1
		},
		.is_at_beginning = true
	};

	parser.token = jsimplon_lexer_next_token(&parser.lexer);
	*tree = jsimplon_parser_parse_value(&parser);

	if (parser.lexer.error_count > 0) {
		success = false;

		jsimplon_append_str(
			error, &error_size,
			"lexer generated %u error(s)\n",
			parser.lexer.error_count
		);
	}

	if (parser.error_count > 0) {
		success = false;

		jsimplon_append_str(
			error, &error_size,
			"parser generated %u error(s)\n",
			parser.error_count
		);
	}

	free(parser.lexer.lexeme);

	if (!success) {
		jsimplon_tree_destroy(tree);
		return NULL;
	}

	free(*error);
	*error = NULL;

	return tree;
}

JSIMPLON_DEF Jsimplon_Value *jsimplon_tree_from_file(char **error, const char *file_name)
{
	size_t error_size;
	if (error != NULL) {
		error_size = 128;
		*error = calloc(error_size, sizeof *(*error));
	}

	char *src = jsimplon_file_read(error, &error_size, file_name);

	if (src == NULL)
		return NULL;

	if (error != NULL && *error != NULL)
		free(*error);

	Jsimplon_Value *tree = jsimplon_tree_from_str(error, src);
	free(src);

	return tree;
}

JSIMPLON_DEF char *jsimplon_tree_to_str(char **error, const Jsimplon_Value *root_value)
{
	size_t error_size;
	if (error != NULL) {
		error_size = 128;
		*error = calloc(error_size, sizeof *(*error));
	}

	bool success = true;

	Jsimplon_Serialiser serialiser = {
		.error = error,
		.error_size = &error_size,
		.str_size = 128
	};
	serialiser.str = calloc(serialiser.str_size, sizeof *serialiser.str);

	jsimplon_value_to_str(&serialiser, root_value);

	if (serialiser.error_count > 0) {
		success = false;

		jsimplon_append_str(
			error, &error_size,
			"serialiser generated %u error(s)\n",
			serialiser.error_count
		);
	}

	if (!success) {
		free(serialiser.str);
		return NULL;
	}

	free(*error);
	*error = NULL;

	return serialiser.str;
}

JSIMPLON_DEF int jsimplon_tree_to_file(char **error, const Jsimplon_Value *root_value, const char *file_name)
{
	char *str = jsimplon_tree_to_str(error, root_value);

	size_t error_size;

	if (str == NULL)
		return JSIMPLON_FAILURE;

	if (error != NULL) {
		error_size = 128;
		*error = calloc(error_size, sizeof *(*error));
	}

	int status = jsimplon_file_write(error, &error_size, file_name, str);
	free(str);

	if (status == JSIMPLON_SUCCESS) {
		free(*error);
		*error = NULL;
	}

	return status;
}

JSIMPLON_DEF int jsimplon_tree_destroy(Jsimplon_Value *tree)
{
	if (tree == NULL)
		return JSIMPLON_FAILURE;

	jsimplon_value_destroy(tree);
	free(tree);

	return JSIMPLON_SUCCESS;
}

JSIMPLON_DEF Jsimplon_Value *jsimplon_tree_root_create(void)
{
	return calloc(1, sizeof(Jsimplon_Value));
}

JSIMPLON_DEF int jsimplon_value_set_str(Jsimplon_Value *value, const char *str)
{
	if (value == NULL || str == NULL)
		return JSIMPLON_FAILURE;

	jsimplon_value_destroy(value);
	value->type = JSIMPLON_VALUE_STRING;
	value->string_value = malloc(strlen(str) + 1);
	strcpy(value->string_value, str);

	return JSIMPLON_SUCCESS;
}

JSIMPLON_DEF int jsimplon_value_set_number(Jsimplon_Value *value, double number)
{
	if (value == NULL)
		return JSIMPLON_FAILURE;

	jsimplon_value_destroy(value);
	value->type = JSIMPLON_VALUE_NUMBER;
	value->number_value = number;

	return JSIMPLON_SUCCESS;
}

JSIMPLON_DEF int jsimplon_value_set_bool(Jsimplon_Value *value, bool bool_value)
{
	if (value == NULL)
		return JSIMPLON_FAILURE;

	jsimplon_value_destroy(value);
	value->type = JSIMPLON_VALUE_BOOL;
	value->bool_value = bool_value;

	return JSIMPLON_SUCCESS;
}

JSIMPLON_DEF int jsimplon_value_set_null(Jsimplon_Value *value)
{
	if (value == NULL)
		return JSIMPLON_FAILURE;

	jsimplon_value_destroy(value);
	value->type = JSIMPLON_VALUE_NULL;
	value->null_value = NULL;

	return JSIMPLON_SUCCESS;
}

JSIMPLON_DEF Jsimplon_Object *jsimplon_value_set_object(Jsimplon_Value *value)
{
	if (value == NULL)
		return NULL;

	jsimplon_value_destroy(value);
	value->type = JSIMPLON_VALUE_OBJECT;
	value->object_value = (Jsimplon_Object) { 0 };

	return &value->object_value;
}

JSIMPLON_DEF Jsimplon_Array *jsimplon_value_set_array(Jsimplon_Value *value)
{
	if (value == NULL)
		return NULL;

	jsimplon_value_destroy(value);
	value->type = JSIMPLON_VALUE_ARRAY;
	value->array_value = (Jsimplon_Array) { 0 };

	return &value->array_value;
}

JSIMPLON_DEF Jsimplon_Member *jsimplon_object_add_member(Jsimplon_Object *object)
{
	if (object == NULL)
		return NULL;

	object->members = realloc(object->members, (object->members_count + 1) * (sizeof *object->members));
	Jsimplon_Member *member = &object->members[object->members_count++];
	*member = (Jsimplon_Member) { 0 };

	return member;
}

JSIMPLON_DEF Jsimplon_Value *jsimplon_object_add_member_value(Jsimplon_Object *object, const char *key)
{
	if (object == NULL || key == NULL)
		return NULL;

	Jsimplon_Member *member = jsimplon_object_add_member(object);
	jsimplon_member_set_key(member, key);

	return &member->value;
}

JSIMPLON_DEF int jsimplon_object_add_member_str(Jsimplon_Object *object, const char *key, const char *str)
{
	if (object == NULL || key == NULL || str == NULL)
		return JSIMPLON_FAILURE;

	return jsimplon_value_set_str(jsimplon_object_add_member_value(object, key), str);
}

JSIMPLON_DEF int jsimplon_object_add_member_number(Jsimplon_Object *object, const char *key, double number)
{
	if (object == NULL || key == NULL)
		return JSIMPLON_FAILURE;

	return jsimplon_value_set_number(jsimplon_object_add_member_value(object, key), number);
}

JSIMPLON_DEF int jsimplon_object_add_member_bool(Jsimplon_Object *object, const char *key, bool bool_value)
{
	if (object == NULL || key == NULL)
		return JSIMPLON_FAILURE;

	return jsimplon_value_set_bool(jsimplon_object_add_member_value(object, key), bool_value);
}

JSIMPLON_DEF int jsimplon_object_add_member_null(Jsimplon_Object *object, const char *key)
{
	if (object == NULL || key == NULL)
		return JSIMPLON_FAILURE;

	return jsimplon_value_set_null(jsimplon_object_add_member_value(object, key));
}

JSIMPLON_DEF Jsimplon_Object *jsimplon_object_add_member_object(Jsimplon_Object *object, const char *key)
{
	if (object == NULL || key == NULL)
		return NULL;

	return jsimplon_value_set_object(jsimplon_object_add_member_value(object, key));
}

JSIMPLON_DEF Jsimplon_Array *jsimplon_object_add_member_array(Jsimplon_Object *object, const char *key)
{
	if (object == NULL || key == NULL)
		return NULL;

	return jsimplon_value_set_array(jsimplon_object_add_member_value(object, key));
}

JSIMPLON_DEF int jsimplon_object_remove_member(Jsimplon_Object *object, const char *key)
{
	if (object == NULL || key == NULL)
		return JSIMPLON_FAILURE;

	for (uint32_t i = 0; i < object->members_count; ++i) {
		Jsimplon_Member *member = &object->members[i];

		if (strcmp(member->key, key) == 0) {
			free(member->key);
			jsimplon_value_destroy(&member->value);

			if (i < object->members_count - 1)
				memmove(&object->members[i], &object->members[i + 1], (object->members_count - i - 1) * (sizeof *object->members));

			--object->members_count;

			return JSIMPLON_SUCCESS;
		}
	}

	return JSIMPLON_FAILURE;
}

JSIMPLON_DEF int jsimplon_member_set_key(Jsimplon_Member *member, const char *new_key)
{
	if (member == NULL || new_key == NULL)
		return JSIMPLON_FAILURE;

	member->key = malloc(strlen(new_key) + 1);
	strcpy(member->key, new_key);

	return JSIMPLON_SUCCESS;
}

JSIMPLON_DEF Jsimplon_Value *jsimplon_member_set_value(Jsimplon_Member *member)
{
	if (member == NULL)
		return NULL;

	return &member->value;
}

JSIMPLON_DEF int jsimplon_member_set_str(Jsimplon_Member *member, const char *str)
{
	if (member == NULL)
		return JSIMPLON_FAILURE;

	return jsimplon_value_set_str(&member->value, str);
}

JSIMPLON_DEF int jsimplon_member_set_number(Jsimplon_Member *member, double number)
{
	if (member == NULL)
		return JSIMPLON_FAILURE;

	return jsimplon_value_set_number(&member->value, number);
}

JSIMPLON_DEF int jsimplon_member_set_bool(Jsimplon_Member *member, bool bool_value)
{
	if (member == NULL)
		return JSIMPLON_FAILURE;

	return jsimplon_value_set_bool(&member->value, bool_value);
}

JSIMPLON_DEF int jsimplon_member_set_null(Jsimplon_Member *member)
{
	if (member == NULL)
		return JSIMPLON_FAILURE;

	return jsimplon_value_set_null(&member->value);
}

JSIMPLON_DEF Jsimplon_Object *jsimplon_member_set_object(Jsimplon_Member *member)
{
	if (member == NULL)
		return NULL;

	return jsimplon_value_set_object(&member->value);
}

JSIMPLON_DEF Jsimplon_Array *jsimplon_member_set_array(Jsimplon_Member *member)
{
	if (member == NULL)
		return NULL;

	return jsimplon_value_set_array(&member->value);
}

JSIMPLON_DEF Jsimplon_Value *jsimplon_array_push_value(Jsimplon_Array *array)
{
	if (array == NULL)
		return NULL;

	array->values = realloc(array->values, (array->values_count + 1) * (sizeof *array->values));
	Jsimplon_Value *value = &array->values[array->values_count++];
	*value = (Jsimplon_Value) { 0 };

	return value;
}

JSIMPLON_DEF int jsimplon_array_push_str(Jsimplon_Array *array, const char *str)
{
	return jsimplon_value_set_str(jsimplon_array_push_value(array), str);
}

JSIMPLON_DEF int jsimplon_array_push_number(Jsimplon_Array *array, double number)
{
	return jsimplon_value_set_number(jsimplon_array_push_value(array), number);
}

JSIMPLON_DEF int jsimplon_array_push_bool(Jsimplon_Array *array, bool bool_value)
{
	return jsimplon_value_set_bool(jsimplon_array_push_value(array), bool_value);
}

JSIMPLON_DEF int jsimplon_array_push_null(Jsimplon_Array *array)
{
	return jsimplon_value_set_null(jsimplon_array_push_value(array));
}

JSIMPLON_DEF Jsimplon_Object *jsimplon_array_push_object(Jsimplon_Array *array)
{
	return jsimplon_value_set_object(jsimplon_array_push_value(array));
}

JSIMPLON_DEF Jsimplon_Array *jsimplon_array_push_array(Jsimplon_Array *array)
{
	return jsimplon_value_set_array(jsimplon_array_push_value(array));
}

JSIMPLON_DEF Jsimplon_Value *jsimplon_array_insert_value_at_index(Jsimplon_Array *array, size_t index)
{
	if (array == NULL)
		return NULL;

	array->values = realloc(array->values, (array->values_count + 1) * (sizeof *array->values)); 

	if (index >= array->values_count)
		return jsimplon_array_push_value(array);

	memmove(&array->values[index + 1], &array->values[index], (array->values_count - index) * (sizeof *array->values));

	Jsimplon_Value *value = &array->values[array->values_count++];
	*value = (Jsimplon_Value) { 0 };

	return value;
}

JSIMPLON_DEF int jsimplon_array_remove_value_at_index(Jsimplon_Array *array, size_t index)
{
	if (array == NULL || index >= array->values_count - 1)
		return JSIMPLON_FAILURE;

	if (index < array->values_count - 1)
		memmove(&array->values[index], &array->values[index + 1], (array->values_count - index - 1) * (sizeof *array->values));

	--array->values_count;

	return JSIMPLON_SUCCESS;
}

JSIMPLON_DEF Jsimplon_ValueType jsimplon_value_get_type(Jsimplon_Value *value)
{
	return value->type;
}

JSIMPLON_DEF Jsimplon_Object *jsimplon_value_get_object(Jsimplon_Value *value)
{
	if (value == NULL)
		return NULL;

	return &value->object_value;
}

JSIMPLON_DEF Jsimplon_Array *jsimplon_value_get_array(Jsimplon_Value *value)
{
	if (value == NULL)
		return NULL;

	return &value->array_value;
}

JSIMPLON_DEF const char *jsimplon_value_get_str(Jsimplon_Value *value)
{
	if (value == NULL)
		return NULL;

	return value->string_value;
}

JSIMPLON_DEF double jsimplon_value_get_number(Jsimplon_Value *value)
{
	if (value == NULL)
		return INFINITY;

	return value->number_value;
}

JSIMPLON_DEF int jsimplon_value_get_bool(Jsimplon_Value *value)
{
	if (value == NULL)
		return -1;

	return value->bool_value;
}

JSIMPLON_DEF Jsimplon_Member *jsimplon_object_get_member(Jsimplon_Object *object, const char *key)
{
	if (object == NULL || key == NULL)
		return NULL;

	for (size_t i = 0; i < object->members_count; ++i) {
		Jsimplon_Member *member = &object->members[i];

		if (strcmp(member->key, key) == 0)
			return member;
	}

	return NULL;
}

JSIMPLON_DEF size_t jsimplon_object_member_get_count(Jsimplon_Object *object)
{
	if (object == NULL)
		return 0;

	return object->members_count;
}

JSIMPLON_DEF Jsimplon_Member *jsimplon_object_member_get_at_index(Jsimplon_Object *object, size_t index)
{
	if (object == NULL)
		return NULL;

	if (index >= object->members_count)
		return NULL;

	return &object->members[index];
}

JSIMPLON_DEF Jsimplon_ValueType jsimplon_object_member_get_type(Jsimplon_Object *object, const char *key)
{
	return jsimplon_value_get_type(jsimplon_object_member_get_value(object, key));
}

JSIMPLON_DEF Jsimplon_Value *jsimplon_object_member_get_value(Jsimplon_Object *object, const char *key)
{
	return &jsimplon_object_get_member(object, key)->value;
}

JSIMPLON_DEF Jsimplon_Object *jsimplon_object_member_get_object(Jsimplon_Object *object, const char *key)
{
	return jsimplon_value_get_object(jsimplon_object_member_get_value(object, key));
}

JSIMPLON_DEF Jsimplon_Array *jsimplon_object_member_get_array(Jsimplon_Object *object, const char *key)
{
	return jsimplon_value_get_array(jsimplon_object_member_get_value(object, key));
}

JSIMPLON_DEF const char *jsimplon_object_member_get_str(Jsimplon_Object *object, const char *key)
{
	return jsimplon_value_get_str(jsimplon_object_member_get_value(object, key));
}

JSIMPLON_DEF double jsimplon_object_member_get_number(Jsimplon_Object *object, const char *key)
{
	return jsimplon_value_get_number(jsimplon_object_member_get_value(object, key));
}

JSIMPLON_DEF int jsimplon_object_member_get_bool(Jsimplon_Object *object, const char *key)
{
	return jsimplon_value_get_bool(jsimplon_object_member_get_value(object, key));
}

JSIMPLON_DEF const char *jsimplon_member_get_key(Jsimplon_Member *member)
{
	if (member == NULL)
		return NULL;

	return member->key;
}

JSIMPLON_DEF Jsimplon_Value *jsimplon_member_get_value(Jsimplon_Member *member)
{
	if (member == NULL)
		return NULL;

	return &member->value;
}

JSIMPLON_DEF const char *jsimplon_member_get_str(Jsimplon_Member *member)
{
	if (member == NULL)
		return NULL;

	return jsimplon_value_get_str(&member->value);
}

JSIMPLON_DEF double jsimplon_member_get_number(Jsimplon_Member *member)
{
	if (member == NULL)
		return INFINITY;

	return jsimplon_value_get_number(&member->value);
}

JSIMPLON_DEF int jsimplon_member_get_bool(Jsimplon_Member *member)
{
	if (member == NULL)
		return -1;

	return jsimplon_value_get_bool(&member->value);
}

JSIMPLON_DEF Jsimplon_Object *jsimplon_member_get_object(Jsimplon_Member *member)
{
	if (member == NULL)
		return NULL;

	return jsimplon_value_get_object(&member->value);
}

JSIMPLON_DEF Jsimplon_Array *jsimplon_member_get_array(Jsimplon_Member *member)
{
	if (member == NULL)
		return NULL;

	return jsimplon_value_get_array(&member->value);
}

JSIMPLON_DEF size_t jsimplon_array_get_count(Jsimplon_Array *array)
{
	if (array == NULL)
		return 0;

	return array->values_count;
}

JSIMPLON_DEF Jsimplon_Value *jsimplon_array_get_value_at_index(Jsimplon_Array *array, size_t index)
{
	if (array == NULL)
		return NULL;

	if (index >= array->values_count)
		return NULL;

	return &array->values[index];
}

JSIMPLON_DEF_INTERNAL Jsimplon_Value jsimplon_parser_parse_value(Jsimplon_Parser *parser)
{
	Jsimplon_Value value = { 0 };

	if (parser->is_at_beginning) {
		parser->is_at_beginning = false;

		if (parser->token.type != JSIMPLON_TOKEN_LBRACE && parser->token.type != JSIMPLON_TOKEN_LBRACKET) {
			jsimplon_append_str(
				parser->lexer.error, parser->lexer.error_size,
				"parser error: %u:%u: expected '{' or '[', got '%s'\n",
				parser->token.line, parser->token.column,
				jsimplon_token_to_str(parser->token)
			);
			++parser->error_count;

			return value;
		}
	}

	switch (parser->token.type) {
		case JSIMPLON_TOKEN_END:
			break;
		case JSIMPLON_TOKEN_STRING_LITERAL:
			value.type = JSIMPLON_VALUE_STRING;
			value.string_value = parser->token.value;
			break;
		case JSIMPLON_TOKEN_NUMBER_LITERAL:
			value.type = JSIMPLON_VALUE_NUMBER;
			value.number_value = strtod(parser->token.value, NULL);
			free(parser->token.value);
			break;
		case JSIMPLON_TOKEN_TRUE:
			value.type = JSIMPLON_VALUE_BOOL;
			value.bool_value = true;
			break;
		case JSIMPLON_TOKEN_FALSE:
			value.type = JSIMPLON_VALUE_BOOL;
			value.bool_value = false;
			break;
		case JSIMPLON_TOKEN_NULL:
			value.type = JSIMPLON_VALUE_NULL;
			value.null_value = NULL;
			break;
		case JSIMPLON_TOKEN_LBRACE:
			value.type = JSIMPLON_VALUE_OBJECT;
			value.object_value = jsimplon_parser_parse_object(parser);
			break;
		case JSIMPLON_TOKEN_LBRACKET:
			value.type = JSIMPLON_VALUE_ARRAY;
			value.array_value = jsimplon_parser_parse_array(parser);
			break;
		default:
			jsimplon_append_str(
				parser->lexer.error, parser->lexer.error_size,
				"parser error: %u:%u: unexpected token: '%s'\n",
				parser->token.line, parser->token.column,
				jsimplon_token_to_str(parser->token)
			);
			++parser->error_count;
			break;
	}

	return value;
}

JSIMPLON_DEF_INTERNAL Jsimplon_Object jsimplon_parser_parse_object(Jsimplon_Parser *parser)
{
	Jsimplon_Object object = { 0 };
	bool expecting_comma = false;

	while (true) {
		parser->token = jsimplon_lexer_next_token(&parser->lexer);

		if (parser->token.type == JSIMPLON_TOKEN_RBRACE)
			break;

		if (expecting_comma) {
			expecting_comma = false;

			if (parser->token.type != JSIMPLON_TOKEN_COMMA) {
				jsimplon_append_str(
					parser->lexer.error, parser->lexer.error_size,
					"parser error: %u:%u: expected ',', got '%s'\n",
					parser->token.line, parser->token.column,
					jsimplon_token_to_str(parser->token)
				);
				++parser->error_count;
			}

			continue;
		}

		if (parser->token.type != JSIMPLON_TOKEN_STRING_LITERAL) {
			jsimplon_append_str(
				parser->lexer.error, parser->lexer.error_size,
				"parser error: %u:%u: expected string literal, got '%s'\n",
				parser->token.line, parser->token.column,
				jsimplon_token_to_str(parser->token)
			);
			++parser->error_count;

			continue;
		}

		object.members = realloc(object.members, (object.members_count + 1) * (sizeof *object.members));
		object.members[object.members_count++] = jsimplon_parser_parse_member(parser);

		expecting_comma = true;
	}

	return object;
}

JSIMPLON_DEF_INTERNAL Jsimplon_Member jsimplon_parser_parse_member(Jsimplon_Parser *parser)
{
	Jsimplon_Member member = { 0 };

	member.key = parser->token.value;
	
	parser->token = jsimplon_lexer_next_token(&parser->lexer);

	if (parser->token.type != JSIMPLON_TOKEN_COLON) {
		jsimplon_append_str(
			parser->lexer.error, parser->lexer.error_size,
			"parser error: %u:%u: expected ':', got '%s'\n",
			parser->token.line, parser->token.column,
			jsimplon_token_to_str(parser->token)
		);
		++parser->error_count;
	}
	else {
		parser->token = jsimplon_lexer_next_token(&parser->lexer);
		member.value = jsimplon_parser_parse_value(parser);
	}


	return member;
}

JSIMPLON_DEF_INTERNAL Jsimplon_Array jsimplon_parser_parse_array(Jsimplon_Parser *parser)
{
	Jsimplon_Array array = { 0 };
	bool expecting_comma = false;

	while (true) {
		parser->token = jsimplon_lexer_next_token(&parser->lexer);

		if (parser->token.type == JSIMPLON_TOKEN_RBRACKET)
			break;

		if (parser->token.type == JSIMPLON_TOKEN_END) {
			jsimplon_append_str(
				parser->lexer.error, parser->lexer.error_size,
				"parser error: %u:%u: expected JSON value, got '%s'\n",
				parser->token.line, parser->token.column,
				jsimplon_token_to_str(parser->token)
			);
			++parser->error_count;

			break;
		}

		if (expecting_comma) {
			expecting_comma = false;

			if (parser->token.type != JSIMPLON_TOKEN_COMMA) {
				jsimplon_append_str(
					parser->lexer.error, parser->lexer.error_size,
					"parser error: %u:%u: expected ',', got '%s'\n",
					parser->token.line, parser->token.column,
					jsimplon_token_to_str(parser->token)
				);
				++parser->error_count;

			}

			continue;
		}

		array.values = realloc(array.values, (array.values_count + 1) * (sizeof *array.values));
		array.values[array.values_count++] = jsimplon_parser_parse_value(parser);

		expecting_comma = true;
	}

	return array;
}

/*
 ** TODO: Refactor lexer to remove spaghetti code!!! **

JSIMPLON_DEF_INTERNAL Jsimplon_Token jsimplon_lexer_next_token(Jsimplon_Lexer *l)
{
	char c;
	size_t lexeme_index;
	Jsimplon_Token token = {
		.line = l->line,
		.column = l->index - l->begin_of_line + 1
	};

	while (isspace(c = l->src[l->index]))
		if (c == '\n') {
			l->begin_of_line = l->index;
			++l->line;
		}

		++l->index;
	}

	if (c == '\"') {
		++l->index;

		while ((c = l->src[l->index]) != '\"') {
			
		}
	}

	switch (c) {
		case '\0':
			break;
		case '{':
			token.type = JSIMPLON_TOKEN_LBRACE;
			break;
		case '}':
			token.type = JSIMPLON_TOKEN_RBRACE;
			break;
		case '[':
			token.type = JSIMPLON_TOKEN_LBRACKET;
			break;
		case ']':
			token.type = JSIMPLON_TOKEN_RBRACKET;
			break;
		case ':':
			token.type = JSIMPLON_TOKEN_COLON;
			break;
		case ',':
			token.type = JSIMPLON_TOKEN_COMMA;
			break;
		default:
			jsimplon_append_str(
				l->error, l->error_size,
				"lexer error: %u:%u: stray '%c'\n",
				token.line, token.column,
				c
			);
			++l->error_count;

			break;
	}

	++l->index;

	return token;
}
*/

JSIMPLON_DEF_INTERNAL Jsimplon_Token jsimplon_lexer_next_token(Jsimplon_Lexer *lexer)
{
	size_t lexeme_index;

	bool looking_for_string = false;
	bool escape_in_effect = false;

	bool looking_for_tfl = false; // tfl = true, false, null

	bool looking_for_number = false;
	bool dot_in_effect = false;
	bool exponent_in_effect = false;
	bool exponent_neg_in_effect = false;

	Jsimplon_Token token = {
		.line = lexer->line,
		.column = lexer->index - lexer->begin_of_line
	};

	do {
	 	char c = lexer->src[lexer->index];

		if (c == 0) {
			token.type = JSIMPLON_TOKEN_END;
			break;
		}

		++lexer->index;

		if (looking_for_string) {
			if (c == '\\') {
				if (!escape_in_effect) {
					escape_in_effect = true;
					continue;
				}
			}
			else if (c == '\"') {
				if (!escape_in_effect) {
					looking_for_string = false;

					token.type = JSIMPLON_TOKEN_STRING_LITERAL;
					token.value = malloc(strlen(lexer->lexeme) + 1);
					strcpy(token.value, lexer->lexeme);

					break;
				}
			}
			else if (c == '\n') {
				jsimplon_append_str(
					lexer->error, lexer->error_size,
					"lexer error: %u:%u: newline character inserted in the middle of string literal %s\n",
					token.line, token.column,
					lexer->lexeme
				);
				++lexer->error_count;

				break;
			}

			if (lexeme_index == JSIMPLON_STRING_LITERAL_MAX_LENGTH) {
				jsimplon_append_str(
					lexer->error, lexer->error_size,
					"lexer error: %u:%u: string literal \"%s...\" exceeded maximum length of %d\n",
					token.line, token.column,
					lexer->lexeme, JSIMPLON_STRING_LITERAL_MAX_LENGTH
				);
				++lexer->error_count;

				break;
			}

			lexer->lexeme[lexeme_index++] = c;
			escape_in_effect = false;

			continue;
		}
		else if (c == '\"') {
			looking_for_string = true;

			lexeme_index = 0;
			memset(lexer->lexeme, 0, JSIMPLON_STRING_LITERAL_MAX_LENGTH + 1);

			continue;
		}

		if (isdigit(c)) {
			if (looking_for_number) {
				if (lexeme_index == JSIMPLON_NUMBER_LITERAL_MAX_LENGTH) {
					jsimplon_append_str(
						lexer->error, lexer->error_size,
						"lexer error: %u:%u: number literal '%s...' exceeded maximum length of %d\n",
						token.line, token.column,
						lexer->lexeme,
						JSIMPLON_NUMBER_LITERAL_MAX_LENGTH
					);
					++lexer->error_count;

					break;
				}
			}
			else {
				looking_for_number = true;

				lexeme_index = 0;
				memset(lexer->lexeme, 0, JSIMPLON_NUMBER_LITERAL_MAX_LENGTH + 1);
			}

			lexer->lexeme[lexeme_index++] = c;

			continue;
		}
		else if (looking_for_number) {
			if (c == '.') {
				if (dot_in_effect) {
					jsimplon_append_str(
						lexer->error, lexer->error_size,
						"lexer error: %u:%u: number literal '%s.' has more than one decimal place\n",
						token.line, token.column,
						lexer->lexeme
					);

					break;
				}

				if (exponent_in_effect) {
					jsimplon_append_str(
						lexer->error, lexer->error_size,
						"lexer error: %u:%u: floating point exponent in number literal '%s.'\n",
						token.line, token.column,
						lexer->lexeme
					);

					break;
				}

				dot_in_effect = true;
				lexer->lexeme[lexeme_index++] = c;

				continue;
			}
			else if (c == 'e') {
				if (exponent_in_effect) {
					jsimplon_append_str(
						lexer->error, lexer->error_size,
						"lexer error: %u:%u: number literal '%se' has more than one exponent marker\n",
						token.line, token.column,
						lexer->lexeme
					);

					break;
				}

				if (lexer->lexeme[lexeme_index - 1] == '.') {
					jsimplon_append_str(
						lexer->error, lexer->error_size,
						"lexer error: %u:%u: number literal '%se' has exponent marker right after '.'\n",
						token.line, token.column,
						lexer->lexeme
					);

					break;
				}

				exponent_in_effect = true;
				lexer->lexeme[lexeme_index++] = c;

				continue;
			}

			if (exponent_in_effect && c == '-') {
				if (exponent_neg_in_effect) {
					jsimplon_append_str(
						lexer->error, lexer->error_size,
						"lexer error: %u:%u: number literal '%s-' has more than one negation sign in the exponent\n",
						token.line, token.column,
						lexer->lexeme
					);

					break;
				}

				exponent_neg_in_effect = true;
				lexer->lexeme[lexeme_index++] = c;

				continue;
			}

			looking_for_number = false;

			token.type = JSIMPLON_TOKEN_NUMBER_LITERAL;
			token.value = malloc(strlen(lexer->lexeme) + 1);
			strcpy(token.value, lexer->lexeme);

			--lexer->index;

			break;
		}
		else if (c == '-') {
			looking_for_number = true;

			lexeme_index = 0;
			memset(lexer->lexeme, 0, JSIMPLON_NUMBER_LITERAL_MAX_LENGTH + 1);

			lexer->lexeme[lexeme_index++] = c;

			continue;
		}

		if (isalpha(c)) {
			if (!looking_for_tfl) {
				looking_for_tfl = true;

				lexeme_index = 0;
				memset(lexer->lexeme, 0, JSIMPLON_STRING_LITERAL_MAX_LENGTH + 1);
			}

			lexer->lexeme[lexeme_index++] = c;

			continue;
		}
		else if (looking_for_tfl) {
			if (strcmp(lexer->lexeme, "true") == 0) {
				token.type = JSIMPLON_TOKEN_TRUE;
			}
			else if (strcmp(lexer->lexeme, "false") == 0) {
				token.type = JSIMPLON_TOKEN_NULL;
			}
			else if (strcmp(lexer->lexeme, "null") == 0) {
				token.type = JSIMPLON_TOKEN_NULL;
			}
			else {
				jsimplon_append_str(
					lexer->error, lexer->error_size,
					"lexer error: %u:%u: unknown character sequence '%s'\n",
					token.line, token.column,
					lexer->lexeme,
					JSIMPLON_NUMBER_LITERAL_MAX_LENGTH
				);
				++lexer->error_count;
			}

			--lexer->index;

			break;
		}


		if (isspace(c)) {
			if (c == '\n') {
				lexer->begin_of_line = lexer->index;
				++lexer->line;
			}

			continue;
		}

		switch (c) {
			case '{':
				token.type = JSIMPLON_TOKEN_LBRACE;
				break;
			case '}':
				token.type = JSIMPLON_TOKEN_RBRACE;
				break;
			case '[':
				token.type = JSIMPLON_TOKEN_LBRACKET;
				break;
			case ']':
				token.type = JSIMPLON_TOKEN_RBRACKET;
				break;
			case ':':
				token.type = JSIMPLON_TOKEN_COLON;
				break;
			case ',':
				token.type = JSIMPLON_TOKEN_COMMA;
				break;
			default:
				jsimplon_append_str(
					lexer->error, lexer->error_size,
					"lexer error: %u:%u: stray '%c'\n",
					token.line, token.column,
					c
				);
				++lexer->error_count;

				break;
		}
	} while (token.type == 0);

	return token;
}

JSIMPLON_DEF_INTERNAL const char *jsimplon_token_to_str(Jsimplon_Token token)
{
	switch (token.type) {
		case JSIMPLON_TOKEN_END:
			return "TOKEN_END";
		case JSIMPLON_TOKEN_STRING_LITERAL:
		case JSIMPLON_TOKEN_NUMBER_LITERAL:
			return token.value;
		case JSIMPLON_TOKEN_TRUE:
			return "true";
		case JSIMPLON_TOKEN_FALSE:
			return "false";
		case JSIMPLON_TOKEN_NULL:
			return "null";
		case JSIMPLON_TOKEN_LBRACE:
			return "{";
		case JSIMPLON_TOKEN_RBRACE:
			return "}";
		case JSIMPLON_TOKEN_LBRACKET:
			return "[";
		case JSIMPLON_TOKEN_RBRACKET:
			return "]";
		case JSIMPLON_TOKEN_COMMA:
			return ",";
		case JSIMPLON_TOKEN_COLON:
			return ":";
	}
}

JSIMPLON_DEF_INTERNAL void jsimplon_value_to_str(Jsimplon_Serialiser *s, const Jsimplon_Value *value)
{
	if (value == NULL) {
		jsimplon_append_str(
			s->error, s->error_size,
			"serialisation error: null value\n" // TODO: Make this more helpful?
		);
		++s->error_count;

		return;
	}

	switch (value->type) {
		case JSIMPLON_VALUE_UNINITIALISED:
			jsimplon_append_str(
				s->error, s->error_size,
				"serialisation error: uninitialised value\n"
			);
			++s->error_count;

			break;
		case JSIMPLON_VALUE_OBJECT:
			jsimplon_object_to_str(s, &value->object_value);
			break;
		case JSIMPLON_VALUE_ARRAY:
			jsimplon_array_to_str(s, &value->array_value);
			break;
		case JSIMPLON_VALUE_STRING:
			jsimplon_append_str(
				&s->str, &s->str_size,
				"\"%s\"", value->string_value
			);
			break;
		case JSIMPLON_VALUE_NUMBER:
			jsimplon_append_str(
				&s->str, &s->str_size,
				"%lf", value->number_value
			);
			break;
		case JSIMPLON_VALUE_BOOL:
			if (value->bool_value == true)
				jsimplon_append_str(&s->str, &s->str_size, "true");
			else
				jsimplon_append_str(&s->str, &s->str_size, "false");
			break;
		case JSIMPLON_VALUE_NULL:
			jsimplon_append_str(&s->str, &s->str_size, "null");
			break;
	}
}

JSIMPLON_DEF_INTERNAL void jsimplon_object_to_str(Jsimplon_Serialiser *s, const Jsimplon_Object *object)
{
	jsimplon_append_str(&s->str, &s->str_size, "{");

	for (uint32_t i = 0; i < object->members_count; ++i) {
		if (i > 0)
			jsimplon_append_str(&s->str, &s->str_size, ",");

		const Jsimplon_Member *member = &object->members[i];

		jsimplon_append_str(
			&s->str, &s->str_size,
			"\"%s\"", member->key
		);
		jsimplon_append_str(&s->str, &s->str_size, ":");
		jsimplon_value_to_str(s, &member->value);
	}

	jsimplon_append_str(&s->str, &s->str_size, "}");
}

JSIMPLON_DEF_INTERNAL void jsimplon_array_to_str(Jsimplon_Serialiser *s, const Jsimplon_Array *array)
{
	jsimplon_append_str(&s->str, &s->str_size, "[");

	for (uint32_t i = 0; i < array->values_count; ++i) {
		if (i > 0)
			jsimplon_append_str(&s->str, &s->str_size, ",");

		jsimplon_value_to_str(s, &array->values[i]);
	}

	jsimplon_append_str(&s->str, &s->str_size, "]");
}

JSIMPLON_DEF_INTERNAL void jsimplon_value_destroy(Jsimplon_Value *value)
{
	switch (value->type) {
		case JSIMPLON_VALUE_STRING:
			free(value->string_value);
			break;
		case JSIMPLON_VALUE_OBJECT:
			jsimplon_object_destroy(&value->object_value);
			break;
		case JSIMPLON_VALUE_ARRAY:
			jsimplon_array_destroy(&value->array_value);
			break;
		default:
			break;
	}

	memset(value, 0, sizeof *value);
}

JSIMPLON_DEF_INTERNAL void jsimplon_object_destroy(Jsimplon_Object *object)
{
	if (object->members == NULL)
		return;

	for (uint32_t i = 0; i < object->members_count; ++i)
		jsimplon_member_destroy(&object->members[i]);
	free(object->members);
	memset(object, 0, sizeof *object);
}

JSIMPLON_DEF_INTERNAL void jsimplon_member_destroy(Jsimplon_Member *member)
{
	if (member->key == NULL)
		return;

	free(member->key);
	jsimplon_value_destroy(&member->value);
	memset(member, 0, sizeof *member);
}

JSIMPLON_DEF_INTERNAL void jsimplon_array_destroy(Jsimplon_Array *array)
{
	if (array->values == NULL)
		return;

	for (uint32_t i = 0; i < array->values_count; ++i)
		jsimplon_value_destroy(&array->values[i]);
	free(array->values);
	memset(array, 0, sizeof *array);
}

JSIMPLON_DEF_INTERNAL void jsimplon_append_str(char **str, size_t *str_size, const char *fmt, ...)
{
	if (str == NULL)
		return;

	va_list args;

	va_start(args, fmt);
	int append_len = vsnprintf(NULL, 0, fmt, args);
	va_end(args);

	size_t str_prev_len = strlen(*str);
	if (str_prev_len + append_len >= *str_size) {
		*str_size *= 2;
		*str_size += append_len;

		*str = realloc(*str, *str_size * (sizeof *(*str) ));
	}

	va_start(args, fmt);
	vsprintf(&(*str)[str_prev_len], fmt, args);
	va_end(args);
}

JSIMPLON_DEF_INTERNAL char *jsimplon_file_read(char **error, size_t *error_size, const char *file_name)
{
	char *buffer = NULL;

	FILE *file = fopen(file_name, "rb");
	if (file == NULL)
		goto jsimplon_file_read_defer;

	fseek(file, 0, SEEK_END);
	int64_t file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	if (file_size < 0)
		goto jsimplon_file_read_defer;

	buffer = malloc((file_size + 1) * (sizeof *buffer));
	fread(buffer, (sizeof *buffer), file_size, file);
	buffer[file_size] = 0;

	if (ferror(file)) {
		free(buffer);
		buffer = NULL;
		goto jsimplon_file_read_defer;
	}

jsimplon_file_read_defer:
	if (file != NULL)
		fclose(file);

	if (buffer == NULL) {
		jsimplon_append_str(
			error, error_size,
			"file read error: %s",
			strerror(errno)
		);
	}

	return buffer;
}

JSIMPLON_DEF_INTERNAL int jsimplon_file_write(char **error, size_t *error_size, const char *file_name, const char *src)
{
	int status = JSIMPLON_SUCCESS;

	FILE *file = fopen(file_name, "wb+");
	if (file == NULL) {
		status = JSIMPLON_FAILURE;
		goto jsimplon_file_write_defer;
	}

	fwrite(src, (sizeof *src), strlen(src), file);

	if (ferror(file))
		status = JSIMPLON_FAILURE;

jsimplon_file_write_defer:
	if (file != NULL)
		fclose(file);

	if (status != JSIMPLON_FAILURE) {
		jsimplon_append_str(
			error, error_size,
			"file write error: %s",
			strerror(errno)
		);
	}

	return status;
}

#endif // JSIMPLON_IMPLEMENTATION

#endif // JSIMPLON_H_

// TODO: Add support for escape sequences in strings
