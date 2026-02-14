NAME	=	ft_ls
CC		=	gcc
CFLAGS	=	
CFLAGS	+=	-fsanitize=address -g
# CFLAGS	+=	-fsanitize=undefined -g
CFLAGS	+=	-Wall -Wextra -Werror
SRC_DIR	=	src
SRCS	=	$(wildcard $(SRC_DIR)/*.c)
OBJDIR	=	obj
OBJS	=	$(SRCS:$(SRC_DIR)/%.c=$(OBJDIR)/%.o)
LIBFT	=	libft
INC_DIR	=	includes
INCS	=	-I $(LIBFT) -I $(INC_DIR)
LINKS	=	-L $(LIBFT) -lm -lft


all: $(OBJDIR) run_libft $(NAME)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

run_libft:
	@make -C $(LIBFT) all bonus

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LINKS)

$(OBJDIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/ls.h Makefile
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	@make -C $(LIBFT) clean
	rm -rf $(OBJDIR)

fclean: clean
	@make -C $(LIBFT) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re