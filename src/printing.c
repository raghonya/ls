#include "ls.h"
#include <sys/sysmacros.h>

char	**format_time_split(time_t input_time, int is_full_datetime);
// void	quoting(const char *s, size_t n);
void	print_filename_quoted(char *s);

void	print_spaces(int current_len, int needed)
{
	while (current_len < needed)
	{
		write (1, " ", 1);
		current_len++;
	}
}

void	print_major_minor(off_t size, t_info_max_lengths *max_lengths)
{
	size_t	major_num;
	int		major_len;
	size_t	minor_num;
	int		minor_len;

	major_num = major(size);
	minor_num = minor(size);
	major_len = ft_numlen(major_num);
	minor_len = ft_numlen(minor_num);
	print_spaces(major_len, max_lengths->major_len);
	ft_putnbr_fd(major_num, 1);
	write (1, ", ", 2);
	print_spaces(minor_len, max_lengths->minor_len);
	ft_putnbr_fd(minor_num, 1);
}

int		print_link(char *link, size_t size)
{
	char	*buf;
	ssize_t	bufsiz;
	ssize_t	nbytes;

	bufsiz = size + 1;

	if (size == 0)
		bufsiz = PATH_MAX;

	buf = malloc(bufsiz);
	if (buf == NULL)
	{
		ft_strcpy(g_err.name, link);
		g_err.type = LS_ERR_NOT_ENOUGH_MEMORY;
		return (LS_RETURN_CODE_FATAL);
	}
	nbytes = readlink(link, buf, bufsiz);
	if (nbytes == -1)
	{
		ft_strcpy(g_err.name, link);
		g_err.type = LS_ERR_UNDEFINED;
		return (LS_RETURN_CODE_FATAL);
	}
	write (1, buf, nbytes);
	free(buf);

	return (LS_RETURN_CODE_NO_ERROR);
}

int		open_dir(arg_t *arg, char *last_dir, uint32_t opts, int triggers);

int		check_link(arg_t *arg, uint32_t opts, int triggers)
{
	// struct stat	sb;
	// int			ret;

	// if (stat(arg->path, &sb) == -1)
	// {
	// 	ft_strcpy(g_err.name, arg->path);
	// 	g_err.type = LS_ERR_PERMISSION_DENIED;
	// 	return (LS_RETURN_CODE_FATAL);
	// }
	// ret = show_contents(NULL, opts, 0);
	// ret = open_dir(arg, arg->path, opts, triggers);
	// if (ret)
	// 	return (ret);

	// return (LS_RETURN_CODE_NO_ERROR);
}

int		print_name(arg_t *arg, t_info_max_lengths *max_lengths, uint32_t opts, int triggers)
{
	char	**tmp;
	int		ret;

	if (!(triggers & DONT_CHECK_OPT_a) && !(LS_OPTION_a & opts) && arg->name[0] == '.')
		return (0);
	// printf ("bany ban %s\n", arg->name);
	if (LS_OPTION_l & opts)
	{
		// type and permissions
		ft_putstr_fd(arg->perm, 1);
		write (1, " ", 1);
		
		// link count
		print_spaces(ft_numlen(arg->lnk_cnt), max_lengths->lnk_cnt_len);
		ft_putnbr_fd(arg->lnk_cnt, 1);
		write (1, " ", 1);
		
		// owner
		// print_spaces(ft_strlen(arg->owner), max_lengths->owner_len);
		ft_putstr_fd(arg->owner, 1);
		print_spaces(ft_strlen(arg->owner), max_lengths->owner_len);
		write (1, " ", 1);
		
		// group
		// print_spaces(ft_strlen(arg->group), max_lengths->group_len);
		ft_putstr_fd(arg->group, 1);
		print_spaces(ft_strlen(arg->group), max_lengths->group_len);
		write (1, " ", 1);
		
		// size
		if (arg->type == CHAR_DEV || arg->type == BLK_DEV)
		{
			print_major_minor(arg->size, max_lengths);
		}
		else
		{
			print_spaces(ft_numlen(arg->size), max_lengths->size_len);
			ft_putnbr_fd(arg->size, 1);
		}
		write (1, " ", 1);

		// last modified time
		tmp = format_time_split(arg->last_modif, arg->is_full_datetime);
		if (!tmp)
		{
			ft_strcpy(g_err.name, arg->path);
			return (LS_RETURN_CODE_FATAL);
		}
		
		write(1, tmp[0], ft_strlen(tmp[0]));
		write(1, " ", 1);
		print_spaces(ft_strlen(tmp[1]), max_lengths->day_len);
		write(1, tmp[1], ft_strlen(tmp[1]));
		write(1, " ", 1);
		write(1, tmp[2], ft_strlen(tmp[2]));
		write(1, " ", 1);
		ft_free_2d_array(tmp);

		// name
		// quoting(arg->name, ft_strlen(arg->name));
		// write (1, arg->name, ft_strlen(arg->name));
		print_filename_quoted(arg->name);
		if (arg->type == LINK)
		{
			// if (arg->type == LINK && )
			// ret = check_link(arg->path, opts);
			// if (ret) return (ret);
			write (1, " -> ", 4);
			print_link(arg->path, arg->size);
		}
		write (1, "\n", 1);
	}
	else
	{
		// ret = check_link(arg, opts, triggers);
		// if (ret) return (ret);
		
		// print_filename_quoted(arg->name);
		write(1, arg->name, ft_strlen(arg->name));
		// printf ("%d\n",write(1, arg->name, ft_strlen(arg->name)));
		if (!(triggers & DIR_LAST_ELEM))
			write (1, "  ", 2);
		else
			write (1, "\n", 1);
	}
		// printf("smth\n");

	return (LS_RETURN_CODE_NO_ERROR);
}

void	print_total_blocks(t_list *order)
{
	int total;

	total = 0;
	while (order)
	{
		total += ((arg_t *)order->data)->blocks;
		order = order->next;
	}
	write(1, "total ", 6);
	ft_putnbr_fd(total / 2, 1);
	write(1, "\n", 1);
}

int		count_arg_info_lengths(t_list *order, t_info_max_lengths *max_lengths, int triggers)
{
	arg_t	*data;
	size_t	current_len;
	char	**time_split;

	while (order)
	{
		data = order->data;
		// link count
		current_len = ft_numlen(data->lnk_cnt);
		if (current_len > max_lengths->lnk_cnt_len)
			max_lengths->lnk_cnt_len = current_len;
		// owner
		current_len = ft_strlen(data->owner);
		if (current_len > max_lengths->owner_len)
			max_lengths->owner_len = current_len;
		// group
		current_len = ft_strlen(data->group);
		if (current_len > max_lengths->group_len)
			max_lengths->group_len = current_len;
		// size
		if (data->type == CHAR_DEV || data->type == BLK_DEV)
		{
			current_len = ft_numlen(major(data->size));
			if (current_len > max_lengths->major_len)
				max_lengths->major_len = current_len;
			current_len = ft_numlen(minor(data->size));
			if (current_len > max_lengths->minor_len)
				max_lengths->minor_len = current_len;
			current_len = max_lengths->major_len + max_lengths->minor_len + 2; // ", "
		}
		else
			current_len = ft_numlen(data->size);
		if (current_len > max_lengths->size_len)
			max_lengths->size_len = current_len;
		// day
		time_split = format_time_split(data->last_modif, data->is_full_datetime);
		if (!time_split)
		{
			ft_strcpy(g_err.name, data->path);
			return (_err_code(triggers));
		}
		current_len = ft_strlen(time_split[1]);
		if (current_len > max_lengths->day_len)
			max_lengths->day_len = current_len;
		ft_free_2d_array(time_split);
		order = order->next;
	}
	return (LS_RETURN_CODE_NO_ERROR);
}

int		print_ordered(t_list *order, t_info_max_lengths *max_lengths, uint32_t opts, int triggers)
{
	int		ret;

	if ((LS_OPTION_l & opts) && !(triggers & DONT_PRINT_TOTAL))
	{
		print_total_blocks(order);
	}
	// print(order);
	while (order)
	{
		if (order->next == NULL)
			triggers |= DIR_LAST_ELEM;
		print_name(order->data, max_lengths, opts, triggers);
		order = order->next;
	}
	if ((triggers & PRINT_DIR_NAME) && !(triggers & LAST_ARG))
		write (1, "\n", 1);
}
