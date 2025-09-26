NAME	=	ft_ls
CC		=	gcc
CFLAGS	=	-fsanitize=address -g -Wall -Wextra #-Werror
SRCS	=	$(wildcard *.c)
OBJDIR	=	obj
OBJS	=	$(SRCS:%.c=$(OBJDIR)/%.o)
LIBFT	=	libft
INCS	=	-I $(LIBFT)
LINKS	=	-L $(LIBFT) -lm -lft

all: $(OBJDIR) run_libft $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

run_libft:
	@make -C $(LIBFT) all
	@make -C $(LIBFT) bonus

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LINKS)

$(OBJDIR)/%.o: %.c ls.h Makefile
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all $(OBJDIR) $(NAME) clean fclean $(OBJDIR)