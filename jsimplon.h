#define JSIMPLON_IMPLEMENTATION

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef JSIMPLON_H
#define JSIMPLON_H

typedef struct jsimplon JSimplON;

static const char *jsimplon_get_error(void);

// Returns false if failed
static bool jsimplon_parse_str(JSimplON *dest, const char *src);
// Returns false if failed
static bool jsimplon_parse_file(JSimplON *dest, const char *file_name);

// Returns false if failed
static bool jsimplon_write_to_file(const JSimplON *jsimplon_data, const char *file_name);

#ifdef JSIMPLON_IMPLEMENTATION

typedef struct jsimplon {

} JSimplON;

/* Returns Errno */
static bool jsimplon_internal_file_read(char **dest, const char *file_name);
/* Returns Errno */
static bool jsimplon_internal_file_write(const char *file_name, const char *src);

#define JSIMPLON_INTERNAL_ERROR_MAX_COUNT 2048

#define JSIMPLON_INTERNAL_SET_ERROR(fmt, ...)\
	jsimplon_internal_error_returned = false;\
	snprintf(jsimplon_internal_error, JSIMPLON_INTERNAL_ERROR_MAX_COUNT, fmt, __VA_ARGS__);

bool jsimplon_internal_error_returned = false;
static char jsimplon_internal_error[JSIMPLON_INTERNAL_ERROR_MAX_COUNT + 1];

static const char *jsimplon_get_error(void)
{
	if (jsimplon_internal_error_returned) {
		memset(jsimplon_internal_error, 0, JSIMPLON_INTERNAL_ERROR_MAX_COUNT);
		jsimplon_internal_error_returned = false;
	}

	return jsimplon_internal_error;
}

static bool jsimplon_parse_str(JSimplON *dest, const char *src)
{

	return true;
}

static bool jsimplon_parse_file(JSimplON *dest, const char *file_name)
{
	bool success = true;
	char *src = NULL;

	if (!jsimplon_internal_file_read(&src, file_name)) {
		success = false;
		goto jsimplon_parse_file_defer;
	}

	printf("%s\n", src);

	if (!jsimplon_parse_str(dest, src)) {
		success = false;
		goto jsimplon_parse_file_defer;
	}

jsimplon_parse_file_defer:
	free(src);

	return success;
}

static bool jsimplon_write_to_file(const JSimplON *jsimplon_data, const char *file_name)
{
	char *str = NULL;

	if (!jsimplon_internal_file_write(file_name, str))
		return false;

	return true;
}

static bool jsimplon_internal_file_read(char **dest, const char *file_name)
{
	bool success = true;

	FILE *file = fopen(file_name, "rb");
	if (file == NULL) {
		success = false;
		goto jsimplon_internal_file_read_defer;
	}

	fseek(file, 0, SEEK_END);
	int64_t file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	if (file_size < 0) {
		success = false;
		goto jsimplon_internal_file_read_defer;
	}

	char *buffer = malloc((file_size + 1) * (sizeof *buffer));
	fread(buffer, (sizeof *buffer), file_size, file);
	buffer[file_size] = 0;

	if (ferror(file)) {
		free(buffer);
		success = false;
		goto jsimplon_internal_file_read_defer;
	}

	*dest = buffer;

jsimplon_internal_file_read_defer:
	if (*dest != NULL)
		free(NULL);
	if (file != NULL)
		fclose(file);

	if (!success)
		JSIMPLON_INTERNAL_SET_ERROR("file read error: %s", strerror(errno));

	return success;
}

static bool jsimplon_internal_file_write(const char *file_name, const char *src)
{
	bool success = true;

	FILE *file = fopen(file_name, "wb+");
	if (file == NULL) {
		success = false;
		goto jsimplon_internal_file_write_defer;
	}

	fwrite(src, (sizeof *src), strlen(src), file);

	if (ferror(file))
		success = false;

jsimplon_internal_file_write_defer:
	if (file != NULL)
		fclose(file);

	if (!success)
		JSIMPLON_INTERNAL_SET_ERROR("file write error: %s", strerror(errno));

	return success;
}

#endif // JSIMPLON_IMPLEMENTATION

#endif // JSIMPLON_H
