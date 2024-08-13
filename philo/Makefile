NAME 	= philo
CC 		= cc
# CFLAGS	= -fsanitize=thread #-Wall -Wextra -Werror
# CFLAGS	= -Wall -Wextra -Werror
HEADER	= philo.h
OBJ_DR 	= objs/

SRCS	=	main.c	\
			tools_1.c		\
			tools_2.c		\
			init.c			\
			simulation.c	\
			getters_setters.c
				
OBJS	= $(addprefix $(OBJ_DR),$(SRCS:.c=.o))

all: $(NAME) 

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJ_DR)%.o: %.c $(HEADER) 
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	rm -rf $(OBJ_DR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all bonus libft clean fclean re