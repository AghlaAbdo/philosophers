NAME 	= ./philo/philo
CC 		= cc
CFLAGS	= #-Wall -Wextra -Werror
HEADER	= ./philo/philo.h
OBJ_DR 	= ./philo/objs/

SRCS	=	./philo/main.c	\
			./philo/grbg_cllctor.c	\
			./philo/tools_1.c
				
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