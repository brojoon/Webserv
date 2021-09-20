
NAME = webserv

CC = clang++

CFLAGS = -Wall -Wextra -Werror -I ./includes -std=c++98

SRCSDIR = ./srcs/

SRCSNAME = $(shell ls $(SRCSDIR) | grep -E ".+\.cpp")

SRCS = $(addprefix $(SRCSDIR), $(SRCSNAME))

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@echo Compiling $(NAME)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp
		$(CC) $(CFLAGS) -o $@ -c $<

clean: 
	rm -rf $(OBJS)

fclean: clean
	 rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re