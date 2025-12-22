#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef JSIMPLON_H_
#define JSIMPLON_H_

#ifndef JSIMPLON_DEF
#define JSIMPLON_DEF static
#endif // JSIMPLON_DEF

typedef struct jsimplon_value Jsimplon_Value;
typedef struct jsimplon_object Jsimplon_Object;
typedef struct jsimplon_member Jsimplon_Member;
typedef struct jsimplon_array Jsimplon_Array;

typedef enum {
	JSON_VALUE_UNINITIALISED,
	JSON_VALUE_OBJECT,
	JSON_VALUE_ARRAY,
	JSON_VALUE_STRING,
	JSON_VALUE_NUMBER,
	JSON_VALUE_BOOL,
	JSON_VALUE_NULL
} Jsimplon_ValueType;

// error is opt-out so you can pass in NULL if you don't care about the message
// it also needs to be freed
JSIMPLON_DEF Jsimplon_Value *jsimplon_tree_from_str(char **error, const char *src);
JSIMPLON_DEF Jsimplon_Value *jsimplon_tree_from_file(char **error, const char *file_name);
JSIMPLON_DEF void            jsimplon_tree_destroy(Jsimplon_Value *root_value);

JSIMPLON_DEF Jsimplon_ValueType jsimplon_get_value_type(Jsimplon_Value *value);
JSIMPLON_DEF Jsimplon_Object *  jsimplon_get_value_object(Jsimplon_Value *value);
JSIMPLON_DEF Jsimplon_Array *   jsimplon_get_value_array(Jsimplon_Value *value);
JSIMPLON_DEF const char *       jsimplon_get_value_str(Jsimplon_Value *value);
JSIMPLON_DEF double             jsimplon_get_value_number(Jsimplon_Value *value);
JSIMPLON_DEF bool               jsimplon_get_value_bool(Jsimplon_Value *value);

JSIMPLON_DEF Jsimplon_Member *  jsimplon_get_object_member(Jsimplon_Object *object, const char *key);
JSIMPLON_DEF size_t             jsimplon_get_object_member_count(Jsimplon_Object *object);
JSIMPLON_DEF Jsimplon_Member *  jsimplon_get_object_member_index(Jsimplon_Object *object, size_t index);
JSIMPLON_DEF Jsimplon_ValueType jsimplon_get_object_member_type(Jsimplon_Object *object, const char *key);
JSIMPLON_DEF Jsimplon_Value *   jsimplon_get_object_member_value(Jsimplon_Object *object, const char *key);
JSIMPLON_DEF Jsimplon_Object *  jsimplon_get_object_member_object(Jsimplon_Object *object, const char *key);
JSIMPLON_DEF Jsimplon_Array *   jsimplon_get_object_member_array(Jsimplon_Object *object, const char *key);
JSIMPLON_DEF const char *       jsimplon_get_object_member_str(Jsimplon_Object *object, const char *key);
JSIMPLON_DEF double             jsimplon_get_object_member_number(Jsimplon_Object *object, const char *key);
JSIMPLON_DEF bool               jsimplon_get_object_member_bool(Jsimplon_Object *object, const char *key);

JSIMPLON_DEF const char *       jsimplon_get_member_key(Jsimplon_Member *member);
JSIMPLON_DEF Jsimplon_Value *   jsimplon_get_member_value(Jsimplon_Member *member);

JSIMPLON_DEF size_t             jsimplon_get_array_count(Jsimplon_Array *array);
JSIMPLON_DEF Jsimplon_Value *   jsimplon_get_array_index(Jsimplon_Array *array, size_t index);

#ifdef JSIMPLON_IMPLEMENTATION

#include <errno.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

typedef enum {
	JSON_TOKEN_END,
	JSON_TOKEN_STRING_LITERAL,
	JSON_TOKEN_NUMBER_LITERAL,
	JSON_TOKEN_TRUE,
	JSON_TOKEN_FALSE,
	JSON_TOKEN_NULL,
	JSON_TOKEN_LBRACE,
	JSON_TOKEN_RBRACE,
	JSON_TOKEN_LBRACKET,
	JSON_TOKEN_RBRACKET,
	JSON_TOKEN_COMMA,
	JSON_TOKEN_COLON
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
JSIMPLON_DEF Jsimplon_Parser jsimplon_parser_create(char **error, size_t *error_size, const char *src);
JSIMPLON_DEF            void jsimplon_parser_destroy(Jsimplon_Parser *parser);
JSIMPLON_DEF  Jsimplon_Value jsimplon_parser_parse_value(Jsimplon_Parser *parser);
JSIMPLON_DEF Jsimplon_Object jsimplon_parser_parse_object(Jsimplon_Parser *parser);
JSIMPLON_DEF Jsimplon_Member jsimplon_parser_parse_member(Jsimplon_Parser *parser);
JSIMPLON_DEF  Jsimplon_Array jsimplon_parser_parse_array(Jsimplon_Parser *parser);

JSIMPLON_DEF void jsimplon_value_destroy(Jsimplon_Value *value);
JSIMPLON_DEF void jsimplon_object_destroy(Jsimplon_Object *object);
JSIMPLON_DEF void jsimplon_member_destroy(Jsimplon_Member *member);
JSIMPLON_DEF void jsimplon_array_destroy(Jsimplon_Array *array);

/* Lexer functions */
JSIMPLON_DEF Jsimplon_Lexer jsimplon_lexer_create(char **error, size_t *error_size, const char *src);
JSIMPLON_DEF           void jsimplon_lexer_destroy(Jsimplon_Lexer *lexer);
// TODO: Add support for escape sequences in strings
JSIMPLON_DEF Jsimplon_Token jsimplon_lexer_next_token(Jsimplon_Lexer *lexer);
JSIMPLON_DEF    const char *jsimplon_token_to_str(Jsimplon_Token token);

/* Utility functions */
JSIMPLON_DEF  void jsimplon_append_error(char **error, size_t *error_size, const char *fmt, ...);
JSIMPLON_DEF char *jsimplon_file_read(char **error, size_t *error_size, const char *file_name); // Returns NULL if failed
JSIMPLON_DEF   int jsimplon_file_write(char **error, size_t *error_size, const char *file_name, const char *src); // Returns 0 if success anything else if failed

JSIMPLON_DEF Jsimplon_Value *jsimplon_tree_from_str(char **error, const char *src)
{
	Jsimplon_Value *tree = malloc(sizeof *tree);

	size_t error_size = 0;
	if (error != NULL) {
		error_size = 1;
		*error = calloc(error_size, (sizeof *(*error)));
	}

	bool success = true;

	Jsimplon_Parser parser = jsimplon_parser_create(error, &error_size, src);

	parser.token = jsimplon_lexer_next_token(&parser.lexer);
	*tree = jsimplon_parser_parse_value(&parser);

	if (parser.lexer.error_count > 0) {
		success = false;

		jsimplon_append_error(
			error, &error_size,
			"lexer generated %u error(s)\n",
			parser.lexer.error_count
		);
	}

	if (parser.error_count > 0) {
		success = false;

		jsimplon_append_error(
			error, &error_size,
			"parser generated %u error(s)\n",
			parser.error_count
		);
	}

	jsimplon_parser_destroy(&parser);

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
		*error = calloc(error_size , sizeof *(*error));
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

JSIMPLON_DEF void jsimplon_tree_destroy(Jsimplon_Value *tree)
{
	if (tree == NULL)
		return;

	jsimplon_value_destroy(tree);
	free(tree);
}

JSIMPLON_DEF Jsimplon_ValueType jsimplon_get_value_type(Jsimplon_Value *value)
{
	return value->type;
}

JSIMPLON_DEF Jsimplon_Object *jsimplon_get_value_object(Jsimplon_Value *value)
{
	return &value->object_value;
}

JSIMPLON_DEF Jsimplon_Array *jsimplon_get_value_array(Jsimplon_Value *value)
{
	return &value->array_value;
}

JSIMPLON_DEF const char *jsimplon_get_value_str(Jsimplon_Value *value)
{
	return value->string_value;
}

JSIMPLON_DEF double jsimplon_get_value_number(Jsimplon_Value *value)
{
	return value->number_value;
}

JSIMPLON_DEF bool jsimplon_get_value_bool(Jsimplon_Value *value)
{
	return value->bool_value;
}

JSIMPLON_DEF Jsimplon_Member *jsimplon_get_object_member(Jsimplon_Object *object, const char *key)
{
	for (size_t i = 0; i < object->members_count; ++i) {
		Jsimplon_Member *member = &object->members[i];

		if (strcmp(member->key, key) == 0)
			return member;
	}

	return NULL;
}

JSIMPLON_DEF size_t jsimplon_get_object_member_count(Jsimplon_Object *object)
{
	return object->members_count;
}

JSIMPLON_DEF Jsimplon_Member *jsimplon_get_object_member_index(Jsimplon_Object *object, size_t index)
{
	if (index >= object->members_count)
		return NULL;

	return &object->members[index];
}

JSIMPLON_DEF Jsimplon_ValueType jsimplon_get_object_member_type(Jsimplon_Object *object, const char *key)
{
	return jsimplon_get_value_type(jsimplon_get_object_member_value(object, key));
}

JSIMPLON_DEF Jsimplon_Value *jsimplon_get_object_member_value(Jsimplon_Object *object, const char *key)
{
	return &jsimplon_get_object_member(object, key)->value;
}

JSIMPLON_DEF Jsimplon_Object *jsimplon_get_object_member_object(Jsimplon_Object *object, const char *key)
{
	return jsimplon_get_value_object(jsimplon_get_object_member_value(object, key));
}

JSIMPLON_DEF Jsimplon_Array *jsimplon_get_object_member_array(Jsimplon_Object *object, const char *key)
{
	return jsimplon_get_value_array(jsimplon_get_object_member_value(object, key));
}

JSIMPLON_DEF const char *jsimplon_get_object_member_str(Jsimplon_Object *object, const char *key)
{
	return jsimplon_get_value_str(jsimplon_get_object_member_value(object, key));
}

JSIMPLON_DEF double jsimplon_get_object_member_number(Jsimplon_Object *object, const char *key)
{
	return jsimplon_get_value_number(jsimplon_get_object_member_value(object, key));
}

JSIMPLON_DEF bool jsimplon_get_object_member_bool(Jsimplon_Object *object, const char *key)
{
	return jsimplon_get_value_bool(jsimplon_get_object_member_value(object, key));
}

JSIMPLON_DEF const char *jsimplon_get_member_key(Jsimplon_Member *member)
{
	return member->key;
}

JSIMPLON_DEF Jsimplon_Value *jsimplon_get_member_value(Jsimplon_Member *member)
{
	return &member->value;
}

JSIMPLON_DEF size_t jsimplon_get_array_count(Jsimplon_Array *array)
{
	return array->values_count;
}

JSIMPLON_DEF Jsimplon_Value *jsimplon_get_array_index(Jsimplon_Array *array, size_t index)
{
	if (index >= array->values_count)
		return NULL;

	return &array->values[index];
}

JSIMPLON_DEF Jsimplon_Parser jsimplon_parser_create(char **error, size_t *error_size, const char *src)
{
	return (Jsimplon_Parser) {
		.lexer = jsimplon_lexer_create(error, error_size, src),
		.is_at_beginning = true
	};
}

JSIMPLON_DEF void jsimplon_parser_destroy(Jsimplon_Parser *parser)
{
	jsimplon_lexer_destroy(&parser->lexer);
}

JSIMPLON_DEF Jsimplon_Value jsimplon_parser_parse_value(Jsimplon_Parser *parser)
{
	Jsimplon_Value value = { 0 };

	if (parser->is_at_beginning) {
		parser->is_at_beginning = false;

		if (parser->token.type != JSON_TOKEN_LBRACE && parser->token.type != JSON_TOKEN_LBRACKET) {
			jsimplon_append_error(
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
		case JSON_TOKEN_END:
			break;
		case JSON_TOKEN_STRING_LITERAL:
			value.type = JSON_VALUE_STRING;
			value.string_value = parser->token.value;
			break;
		case JSON_TOKEN_NUMBER_LITERAL:
			value.type = JSON_VALUE_NUMBER;
			value.number_value = strtod(parser->token.value, NULL);
			free(parser->token.value);
			break;
		case JSON_TOKEN_TRUE:
			value.type = JSON_VALUE_BOOL;
			value.bool_value = true;
			break;
		case JSON_TOKEN_FALSE:
			value.type = JSON_VALUE_BOOL;
			value.bool_value = false;
			break;
		case JSON_TOKEN_NULL:
			value.type = JSON_VALUE_NULL;
			value.null_value = NULL;
			break;
		case JSON_TOKEN_LBRACE:
			value.type = JSON_VALUE_OBJECT;
			value.object_value = jsimplon_parser_parse_object(parser);
			break;
		case JSON_TOKEN_LBRACKET:
			value.type = JSON_VALUE_ARRAY;
			value.array_value = jsimplon_parser_parse_array(parser);
			break;
		default:
			jsimplon_append_error(
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

JSIMPLON_DEF Jsimplon_Object jsimplon_parser_parse_object(Jsimplon_Parser *parser)
{
	Jsimplon_Object object = { 0 };
	bool expecting_comma = false;

	while (true) {
		parser->token = jsimplon_lexer_next_token(&parser->lexer);

		if (parser->token.type == JSON_TOKEN_RBRACE)
			break;

		if (expecting_comma) {
			expecting_comma = false;

			if (parser->token.type != JSON_TOKEN_COMMA) {
				jsimplon_append_error(
					parser->lexer.error, parser->lexer.error_size,
					"parser error: %u:%u: expected ',', got '%s'\n",
					parser->token.line, parser->token.column,
					jsimplon_token_to_str(parser->token)
				);
				++parser->error_count;
			}

			continue;
		}

		if (parser->token.type != JSON_TOKEN_STRING_LITERAL) {
			jsimplon_append_error(
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

JSIMPLON_DEF Jsimplon_Member jsimplon_parser_parse_member(Jsimplon_Parser *parser)
{
	Jsimplon_Member member = { 0 };

	member.key = parser->token.value;
	
	parser->token = jsimplon_lexer_next_token(&parser->lexer);

	if (parser->token.type != JSON_TOKEN_COLON) {
		jsimplon_append_error(
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

JSIMPLON_DEF Jsimplon_Array jsimplon_parser_parse_array(Jsimplon_Parser *parser)
{
	Jsimplon_Array array = { 0 };
	bool expecting_comma = false;

	while (true) {
		parser->token = jsimplon_lexer_next_token(&parser->lexer);

		if (parser->token.type == JSON_TOKEN_RBRACKET)
			break;

		if (parser->token.type == JSON_TOKEN_END) {
			jsimplon_append_error(
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

			if (parser->token.type != JSON_TOKEN_COMMA) {
				jsimplon_append_error(
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

JSIMPLON_DEF void jsimplon_value_destroy(Jsimplon_Value *value)
{
	switch (value->type) {
		case JSON_VALUE_STRING:
			free(value->string_value);
			break;
		case JSON_VALUE_OBJECT:
			jsimplon_object_destroy(&value->object_value);
			break;
		case JSON_VALUE_ARRAY:
			jsimplon_array_destroy(&value->array_value);
			break;
		default:
			break;
	}

	memset(value, 0, sizeof *value);
}

JSIMPLON_DEF void jsimplon_object_destroy(Jsimplon_Object *object)
{
	if (object->members == NULL)
		return;

	for (uint32_t i = 0; i < object->members_count; ++i)
		jsimplon_member_destroy(&object->members[i]);
	free(object->members);
	memset(object, 0, sizeof *object);
}

JSIMPLON_DEF void jsimplon_member_destroy(Jsimplon_Member *member)
{
	if (member->key == NULL)
		return;

	free(member->key);
	jsimplon_value_destroy(&member->value);
	memset(member, 0, sizeof *member);
}

JSIMPLON_DEF void jsimplon_array_destroy(Jsimplon_Array *array)
{
	if (array->values == NULL)
		return;

	for (uint32_t i = 0; i < array->values_count; ++i)
		jsimplon_value_destroy(&array->values[i]);
	free(array->values);
	memset(array, 0, sizeof *array);
}

JSIMPLON_DEF Jsimplon_Lexer jsimplon_lexer_create(char **error, size_t *error_size, const char *src)
{
	Jsimplon_Lexer lexer = {
		.src = src,
		.error = error,
		.error_size = error_size,
		.line = 1,
	};

	lexer.lexeme = malloc(JSIMPLON_STRING_LITERAL_MAX_LENGTH + 1);

	return lexer;
}

JSIMPLON_DEF void jsimplon_lexer_destroy(Jsimplon_Lexer *lexer)
{
	free(lexer->lexeme);
}

/*
 ** TODO: Refactor lexer to remove spaghetti code!!! **

JSIMPLON_DEF Jsimplon_Token jsimplon_lexer_next_token(Jsimplon_Lexer *l)
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
			token.type = JSON_TOKEN_LBRACE;
			break;
		case '}':
			token.type = JSON_TOKEN_RBRACE;
			break;
		case '[':
			token.type = JSON_TOKEN_LBRACKET;
			break;
		case ']':
			token.type = JSON_TOKEN_RBRACKET;
			break;
		case ':':
			token.type = JSON_TOKEN_COLON;
			break;
		case ',':
			token.type = JSON_TOKEN_COMMA;
			break;
		default:
			jsimplon_append_error(
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

JSIMPLON_DEF Jsimplon_Token jsimplon_lexer_next_token(Jsimplon_Lexer *lexer)
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
			token.type = JSON_TOKEN_END;
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

					token.type = JSON_TOKEN_STRING_LITERAL;
					token.value = malloc(strlen(lexer->lexeme) + 1);
					strcpy(token.value, lexer->lexeme);

					break;
				}
			}
			else if (c == '\n') {
				jsimplon_append_error(
					lexer->error, lexer->error_size,
					"lexer error: %u:%u: newline character inserted in the middle of string literal %s\n",
					token.line, token.column,
					lexer->lexeme
				);
				++lexer->error_count;

				break;
			}

			if (lexeme_index == JSIMPLON_STRING_LITERAL_MAX_LENGTH) {
				jsimplon_append_error(
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
					jsimplon_append_error(
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
					jsimplon_append_error(
						lexer->error, lexer->error_size,
						"lexer error: %u:%u: number literal '%s.' has more than one decimal place\n",
						token.line, token.column,
						lexer->lexeme
					);

					break;
				}

				if (exponent_in_effect) {
					jsimplon_append_error(
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
					jsimplon_append_error(
						lexer->error, lexer->error_size,
						"lexer error: %u:%u: number literal '%se' has more than one exponent marker\n",
						token.line, token.column,
						lexer->lexeme
					);

					break;
				}

				if (lexer->lexeme[lexeme_index - 1] == '.') {
					jsimplon_append_error(
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
					jsimplon_append_error(
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

			token.type = JSON_TOKEN_NUMBER_LITERAL;
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
				token.type = JSON_TOKEN_TRUE;
			}
			else if (strcmp(lexer->lexeme, "false") == 0) {
				token.type = JSON_TOKEN_NULL;
			}
			else if (strcmp(lexer->lexeme, "null") == 0) {
				token.type = JSON_TOKEN_NULL;
			}
			else {
				jsimplon_append_error(
					lexer->error, lexer->error_size,
					"lexer error: %u:%u: unknown character sequence '%s'\n",
					token.line, token.column,
					lexer->lexeme,
					JSIMPLON_NUMBER_LITERAL_MAX_LENGTH
				);
				++lexer->error_count;

			}

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
				token.type = JSON_TOKEN_LBRACE;
				break;
			case '}':
				token.type = JSON_TOKEN_RBRACE;
				break;
			case '[':
				token.type = JSON_TOKEN_LBRACKET;
				break;
			case ']':
				token.type = JSON_TOKEN_RBRACKET;
				break;
			case ':':
				token.type = JSON_TOKEN_COLON;
				break;
			case ',':
				token.type = JSON_TOKEN_COMMA;
				break;
			default:
				jsimplon_append_error(
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

JSIMPLON_DEF const char *jsimplon_token_to_str(Jsimplon_Token token)
{
	switch (token.type) {
		case JSON_TOKEN_END:
			return "TOKEN_END";
		case JSON_TOKEN_STRING_LITERAL:
		case JSON_TOKEN_NUMBER_LITERAL:
			return token.value;
		case JSON_TOKEN_TRUE:
			return "true";
		case JSON_TOKEN_FALSE:
			return "false";
		case JSON_TOKEN_NULL:
			return "null";
		case JSON_TOKEN_LBRACE:
			return "{";
		case JSON_TOKEN_RBRACE:
			return "}";
		case JSON_TOKEN_LBRACKET:
			return "[";
		case JSON_TOKEN_RBRACKET:
			return "]";
		case JSON_TOKEN_COMMA:
			return ",";
		case JSON_TOKEN_COLON:
			return ":";
	}
}

JSIMPLON_DEF void jsimplon_append_error(char **error, size_t *error_size, const char *fmt, ...)
{
	if (error == NULL)
		return;

	va_list args;

	va_start(args, fmt);
	size_t append_len = vsnprintf(NULL, 0, fmt, args);
	va_end(args);

	size_t error_prev_len = strlen(*error);
	if (error_prev_len + append_len >= *error_size) {
		*error_size *= 2;
		*error_size += append_len;

		*error = realloc(*error, *error_size * (sizeof *(*error) ));
	}

	va_start(args, fmt);
	vsprintf(&(*error)[error_prev_len], fmt, args);
	va_end(args);
}

JSIMPLON_DEF char *jsimplon_file_read(char **error, size_t *error_size, const char *file_name)
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
		jsimplon_append_error(
			error, error_size,
			"file read error: %s",
			strerror(errno)
		);
	}

	return buffer;
}

JSIMPLON_DEF int jsimplon_file_write(char **error, size_t *error_size, const char *file_name, const char *src)
{
	int status = 0;

	FILE *file = fopen(file_name, "wb+");
	if (file == NULL) {
		status = 1;
		goto jsimplon_file_write_defer;
	}

	fwrite(src, (sizeof *src), strlen(src), file);

	if (ferror(file))
		status = 1;

jsimplon_file_write_defer:
	if (file != NULL)
		fclose(file);

	if (status != 0) {
		jsimplon_append_error(
			error, error_size,
			"file write error: %s",
			strerror(errno)
		);
	}

	return status;
}

#endif // JSIMPLON_IMPLEMENTATION

#endif // JSIMPLON_H_
