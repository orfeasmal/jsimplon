#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef JSIMPLON_H
#define JSIMPLON_H

#ifndef JSIMPLON_DEF
#define JSIMPLON_DEF static
#endif // JSIMPLON_DEF

typedef struct jsimplon JSimplON;

// error is opt-out so you can pass in NULL if you don't care about the message
// it also needs to be freed
JSIMPLON_DEF JSimplON *jsimplon_create_from_str(char **error, const char *src);
JSIMPLON_DEF JSimplON *jsimplon_create_from_file(char **error, const char *file_name);

JSIMPLON_DEF void jsimplon_destroy(JSimplON *data);

#ifdef JSIMPLON_IMPLEMENTATION

#include <errno.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

typedef struct jsimplon {

} JSimplON;

JSIMPLON_DEF void jsimplon_append_error(char **error, size_t *error_size, const char *fmt, ...);

typedef enum {
	JSON_TOKEN_END,
	JSON_TOKEN_STRING_LITERAL,
	JSON_TOKEN_NUMBER_LITERAL,
	JSON_TOKEN_FLOAT,
	JSON_TOKEN_TRUE,
	JSON_TOKEN_FALSE,
	JSON_TOKEN_NULL,
	JSON_TOKEN_LBRACE,
	JSON_TOKEN_RBRACE,
	JSON_TOKEN_LBRACKET,
	JSON_TOKEN_RBRACKET,
	JSON_TOKEN_COMMA,
	JSON_TOKEN_COLON
} JSimplON_TokenType;

typedef struct {
	char *value; // NULL for things like { } , : and so on
	size_t value_size;
	JSimplON_TokenType type;
} JSimplON_Token;

#define JSIMPLON_STRING_LITERAL_MAX_LENGTH ((1 << 15) - 1)
#define JSIMPLON_NUMBER_LITERAL_MAX_LENGTH 21

typedef struct {
	const char *src;
	char *lexeme;
	size_t index;
	uint32_t line, column;
	char **error;
	size_t *error_size;
	uint32_t error_count;
} JSimplON_Lexer;

JSIMPLON_DEF JSimplON_Lexer jsimplon_lexer_create(char **error, size_t *error_size, const char *src);
JSIMPLON_DEF           void jsimplon_lexer_destroy(JSimplON_Lexer *lexer);
JSIMPLON_DEF JSimplON_Token jsimplon_lexer_next_token(JSimplON_Lexer *lexer);
JSIMPLON_DEF    const char *jsimplon_token_type_to_str(JSimplON_TokenType type);

// Returns NULL if failed
JSIMPLON_DEF char *jsimplon_file_read(char **error, size_t *error_size, const char *file_name);
// Returns 0 if success anything else if failed
JSIMPLON_DEF int jsimplon_file_write(char **error, size_t *error_size, const char *file_name, const char *src);

JSIMPLON_DEF JSimplON *jsimplon_create_from_str(char **error, const char *src)
{
	JSimplON *data = malloc(sizeof *data);

	size_t error_size = 0;
	if (error != NULL) {
		error_size = 1;
		*error = calloc(error_size, (sizeof *(*error)));
	}

	bool success = true;

	JSimplON_Lexer lexer = jsimplon_lexer_create(error, &error_size, src);

	JSimplON_Token token;
	do {
		token = jsimplon_lexer_next_token(&lexer);
		//printf("%s %s\n", jsimplon_token_type_to_str(token.type), token.value);

		if (token.value)
			free(token.value);
	} while (token.type != JSON_TOKEN_END);

	if (lexer.error_count > 0) {
		jsimplon_append_error(
				error, &error_size,
				"parser error: lexer generated %u error(s)",
				lexer.error_count
		);

		success = false;
	}

	jsimplon_lexer_destroy(&lexer);

	if (!success) {
		free(data);
		return NULL;
	}
	else {
		free(*error);
		*error = NULL;
		return data;
	}
}

JSIMPLON_DEF JSimplON *jsimplon_create_from_file(char **error, const char *file_name)
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

	JSimplON *data = jsimplon_create_from_str(error, src);
	free(src);

	return data;
}

JSIMPLON_DEF void jsimplon_destroy(JSimplON *data)
{
	free(data);
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

JSIMPLON_DEF JSimplON_Lexer jsimplon_lexer_create(char **error, size_t *error_size, const char *src)
{
	JSimplON_Lexer lexer = {
		.src = src,
		.error = error,
		.error_size = error_size,
		.line = 1,
	};

	lexer.lexeme = malloc(JSIMPLON_STRING_LITERAL_MAX_LENGTH + 1);

	return lexer;
}

JSIMPLON_DEF void jsimplon_lexer_destroy(JSimplON_Lexer *lexer)
{
	free(lexer->lexeme);
}

JSIMPLON_DEF JSimplON_Token jsimplon_lexer_next_token(JSimplON_Lexer *lexer)
{
	JSimplON_Token token = { 0 };

	size_t lexeme_index;

	bool looking_for_string = false;
	bool escape_char_in_effect = false;

	bool looking_for_number = false;
	// bool dot_in_effect = false // lol

	do {
	 	char c = lexer->src[lexer->index];

		if (c == 0) {
			token.type = JSON_TOKEN_END;
			break;
		}

		++lexer->index;
		++lexer->column;

		if (looking_for_string) {
			if (c == '\\') {
				if (!escape_char_in_effect) {
					escape_char_in_effect = true;
					continue;
				}
			}
			else if (c == '\"') {
				if (!escape_char_in_effect) {
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
					lexer->line, lexer->column,
					lexer->lexeme
				);
				++lexer->error_count;

				break;
			}

			if (lexeme_index == JSIMPLON_STRING_LITERAL_MAX_LENGTH) {
				jsimplon_append_error(
					lexer->error, lexer->error_size,
					"lexer error: %u:%u: string literal \"%s...\" exceeded maximum length of %d\n",
					lexer->line, lexer->column, 
					lexer->lexeme, JSIMPLON_STRING_LITERAL_MAX_LENGTH
				);
				++lexer->error_count;

				break;
			}

			lexer->lexeme[lexeme_index++] = c;
			escape_char_in_effect = false;

			continue;
		}
		else {
			if (c == '\"') {
				looking_for_string = true;

				lexeme_index = 0;
				memset(lexer->lexeme, 0, JSIMPLON_STRING_LITERAL_MAX_LENGTH + 1);

				continue;
			}
		}

		if (isdigit(c)) {
			if (looking_for_number) {
				if (lexeme_index == JSIMPLON_NUMBER_LITERAL_MAX_LENGTH) {
					jsimplon_append_error(
						lexer->error, lexer->error_size,
						"lexer error: %u:%u: number literal '%s...' exceeded maximum length of %d\n",
						lexer->line, lexer->column,
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
			// if (c == '.') {
			// 	if (dot_in_effect) {
			// 		jsimplon_append_error(
			// 			lexer->error, lexer->error_size,
			// 			"lexer error: %u:%u: number literal '%s.' has more than one decimal place\n",
			// 			lexer->line, lexer->column,
			// 			lexer->lexeme
			// 		);

			// 		break;
			// 	}

			// 	dot_in_effect = true;
			// }

			looking_for_number = false;

			token.type = JSON_TOKEN_NUMBER_LITERAL;
			token.value = malloc(strlen(lexer->lexeme) + 1);
			strcpy(token.value, lexer->lexeme);
		}

		if (isspace(c)) {
			if (c == '\n') {
				lexer->column = 0;
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
					lexer->line, lexer->column,
					c
				);
				++lexer->error_count;

				break;
		}
	} while (token.type == 0);

	return token;
}

JSIMPLON_DEF const char *jsimplon_token_type_to_str(JSimplON_TokenType type)
{
	switch (type) {
		case JSON_TOKEN_END:
			return "JSON_TOKEN_END";
			break;
		case JSON_TOKEN_STRING_LITERAL:
			return "JSON_TOKEN_STRING_LITERAL";
			break;
		case JSON_TOKEN_NUMBER_LITERAL:
			return "JSON_TOKEN_NUMBER_LITERAL";
			break;
		case JSON_TOKEN_FLOAT:
			return "JSON_TOKEN_FLOAT";
			break;
		case JSON_TOKEN_TRUE:
			return "JSON_TOKEN_TRUE";
			break;
		case JSON_TOKEN_FALSE:
			return "JSON_TOKEN_FALSE";
			break;
		case JSON_TOKEN_NULL:
			return "JSON_TOKEN_NULL";
			break;
		case JSON_TOKEN_LBRACE:
			return "JSON_TOKEN_LBRACE";
			break;
		case JSON_TOKEN_RBRACE:
			return "JSON_TOKEN_RBRACE";
			break;
		case JSON_TOKEN_LBRACKET:
			return "JSON_TOKEN_LBRACKET";
			break;
		case JSON_TOKEN_RBRACKET:
			return "JSON_TOKEN_RBRACKET";
			break;
		case JSON_TOKEN_COMMA:
			return "JSON_TOKEN_COMMA";
			break;
		case JSON_TOKEN_COLON:
			return "JSON_TOKEN_COLON";
			break;
	}
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

#endif // JSIMPLON_H
