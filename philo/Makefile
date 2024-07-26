SORS = philo.c
OBJC=$(SORS:.c=.o)
NAME = philo
RM = rm -f
CC = cc -ggdb -fsanitize=thread -g3
#CFLAGS = -Wall -Wextra -Werror -fsanitize=thread -g3

all: $(NAME)


$(NAME): $(OBJC) 
	$(CC) -o $(NAME) $^

clean:
	$(RM) $(OBJC)

fclean: clean
	$(RM) $(NAME)

re: fclean all