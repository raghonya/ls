NAME	=	ft_ls
CC		=	cc
CFLAGS	=	-fsanitize=address -Wall -Wextra #-Werror
SRCS	=	$(wildcard *.c)
OBJDIR	=	obj
OBJS	=	$(SRCS:%.c=$(OBJDIR)/%.o)
INCS	=	-I libft
LINKS	=	-L libft -lm -lft

all: mk_obj_dir $(NAME)

mk_obj_dir:
	@mkdir -p $(OBJDIR)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LINKS)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all mk_obj_dir $(NAME) clean fclean