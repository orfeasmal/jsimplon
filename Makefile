NC     = \033[0m
BLUE   = \033[1;34m
CYAN   = \033[1;36m
GREEN  = \033[1;32m
YELLOW = \033[1;33m

CC = clang
LD = clang

CFLAGS =  -std=c11 -Wall -Wextra -Wpedantic -I.
CFLAGS += 

CFLAGS_DEB = -O0 -g -gdwarf-4 -fsanitize=address
CFLAGS_REL = -O3

LDFLAGS =

LDFLAGS_DEB = -fsanitize=address

rwildcard = $(foreach d, $(wildcard $1*), $(call rwildcard, $d/, $2) $(filter $(subst *, %, $2), $d))

OBJ_DEB_DIR = build/debug/obj
OBJ_REL_DIR = build/release/obj
SRC_DIR     = examples
SRC         = $(call rwildcard, $(SRC_DIR), *.c)
OBJ_DEB     = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DEB_DIR)/%.o, $(SRC))
OBJ_REL     = $(patsubst $(SRC_DIR)/%.c, $(OBJ_REL_DIR)/%.o, $(SRC))

EXE_REL = build/release/example
EXE_DEB = build/debug/example

.PHONY: run clean

debug: $(EXE_DEB)
release: $(EXE_REL)

$(EXE_DEB): $(OBJ_DEB)
	@ echo -e "$(GREEN)LINKING EXECUTABLE$(NC) $@"
	@ $(LD) $(OBJ_DEB) -o $@ $(LDFLAGS) $(LDFLAGS_DEB)

$(EXE_REL): $(OBJ_REL)
	@ echo -e "$(GREEN)LINKING EXECUTABLE$(NC) $@"
	@ $(LD) $(OBJ_REL) -o $@ $(LDFLAGS)

$(OBJ_REL_DIR)/%.o: $(SRC_DIR)/%.c
	@ mkdir -p $(@D)
	@ echo -e "$(GREEN)COMPILING OBJECT$(NC) $@"
	@ $(CC) $(CFLAGS) $(CFLAGS_REL) -c $< -o $@

$(OBJ_DEB_DIR)/%.o: $(SRC_DIR)/%.c
	@ mkdir -p $(@D)
	@ echo -e "$(GREEN)COMPILING OBJECT$(NC) $@"
	@ $(CC) $(CFLAGS) $(CFLAGS_DEB) -c $< -o $@

run: debug
	@ echo -e "$(CYAN)EXECUTING$(NC) $(EXE_DEB)"
	@ ./$(EXE_DEB) $(SRC_DIR)/test.json

clean:
	@ echo -e "$(YELLOW)CLEANING PROJECT$(NC)"
	@ rm -rf build
