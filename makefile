NAME = minishell

CC = cc

CFLAGS = -Wall -Werror -Wextra

# Includes
INCLUDES = -I ./includes/

# Directories
SRC_DIR = ./src/
OBJS_DIR = ./objs/
LIBFT_DIR = ./libft/

# Source Files
SRC         =   $(shell find src/ -name '*.c')

# Object Files
OBJS        =   $(patsubst src/%.c,objs/%.o,$(SRC))

# Libraries
LIBFT = $(LIBFT_DIR)libft.a
LIBRARIES = -lft -lreadline

all: ${NAME}

$(LIBFT):
	make bonus -C $(LIBFT_DIR)

# creating executables
${NAME}: $(LIBFT) $(OBJS_DIR) $(OBJS) 
	${CC} ${CFLAGS} ${OBJS} -L${LIBFT_DIR} ${LIBRARIES} -o ${NAME}

# Creating Object file folder
$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

# Creating object files
$(OBJS_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(@D)
	${CC} ${CFLAGS} -c ${INCLUDES} $< -o $@

clean:
	rm -rf $(OBJS_DIR)
	rm -rf ./libft/*.o

fclean: clean
	rm -rf ${NAME} out* valgrind*
	rm -rf ./libft/*.a

re: fclean ${NAME}
	@clear

test: re
	@./minishell

rebonus: fclean bonus

valgrind:
	@valgrind --leak-check=full --track-fds=yes --suppressions=readline.supp -s --show-leak-kinds=all --track-origins=yes ./minishell

.PHONY: all clean fclean re