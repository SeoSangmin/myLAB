NAME	=	test

CC		=	c++
CFLAGS	=	-Werror -fsanitize=thread

SRCS	=	main.cpp Mutex.cpp Data.cpp A.cpp B.cpp
OBJS	=	$(SRCS:cpp=o)

INC		=

all	: $(NAME)

%.o	: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INC) $(MLX) $^ -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re