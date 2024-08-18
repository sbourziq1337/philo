SORS = philo.c helper.c check_routine.c
OBJC=$(SORS:.c=.o)
NAME = philo
RM = rm -f
CC = cc -ggdb #-fsanitize=thread -g3
CFLAGS = -Wall -Wextra -Werror #-g3 -fsanitize=thread

all: $(NAME)


$(NAME): $(OBJC) 
	$(CC) $(CFLAGS) -o $(NAME) $^

clean:
	$(RM) $(OBJC)

fclean: clean
	$(RM) $(NAME)

re: fclean all