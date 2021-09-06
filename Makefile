
NAME = webserv

CC = clang++

CFLAGS =  -I ./includes

SRCSDIR = ./srcs/

SRCSNAME = $(shell ls $(SRCSDIR) | grep -E ".+\.cpp")

SRCS = $(addprefix $(SRCSDIR), $(SRCSNAME))

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@echo Compiling $(NAME)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -fsanitize=address

%.o: %.cpp
		$(CC) $(CFLAGS) -o $@ -c $<

clean: 
	rm -rf $(OBJS)

fclean: clean
	 rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re