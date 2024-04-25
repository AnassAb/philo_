CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -g3 -fsanitize=thread
RM = rm -rf

HFILE = philo.h
SRCS = checkers.c monitors.c philo.c routine.c simulation.c utils.c ft_atoi.c
OBJS = $(SRCS:.c=.o)

NAME = philo

all: $(NAME)

$(NAME): $(OBJS) $(HFILE)
	$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $? -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

.SECONDARY: $(OBJS)