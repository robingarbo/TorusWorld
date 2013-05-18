##
## Makefile
##
## Made by robin garbo
## Mail   <robingarbo@gmail.com>
##
##

NAME		=	TorusWorld

DIR		=	src/

SRC		=	$(DIR)main.cpp		\
			$(DIR)human.cpp		\
			$(DIR)game.cpp		\
			$(DIR)bitmapload.cpp

OBJ		=	$(SRC:.cpp=.o)

CXXFLAGS	+=	-W -g ##-Wall -Werror 

RM		=	rm -f

CC		=	g++

INCLUDE		+=	-I/home/robin/Documents/lib/name/include -I./include/ 

LDFLAGS		= -L./ -lX11 -lXi -lXmu -lglut -lGL -lGLU -lm




$(NAME)		: $(OBJ)
		$(CC) -o $(NAME) $(OBJ) $(LDFLAGS)

all		: $(NAME)

clean		:
		$(RM) $(OBJ)

fclean		: clean
		$(RM) $(NAME)

re		: fclean all

.cpp.o		:
		$(CC) $(CXXFLAGS) $(INCLUDE) -c $< -o $@
