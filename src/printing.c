#include "ls.h"

char **format_time_split(time_t input_time, int is_full_datetime);

void	print_spaces_until_word(int current_len, int target_len)
{
	while (current_len < target_len)
	{
		write (1, " ", 1);
		current_len++;
	}
}

int		print_name(arg_t *arg, t_info_max_lengths max_lengths, uint32_t opts, int triggers)
{
	char	**tmp;

	if (!(triggers & DONT_CHECK_OPT_a) && !(LS_OPTION_a & opts) && arg->name[0] == '.')
		return (0);
	if (LS_OPTION_l & opts)
	{
		// type and permissions
		// print_spaces_until_word(ft_strlen(arg->perm), 10);
		write (1, arg->perm, ft_strlen(arg->perm));
		write (1, " ", 1);
		
		// link count
		print_spaces_until_word(ft_numlen(arg->lnk_cnt), max_lengths.lnk_cnt_len);
		ft_putnbr_fd(arg->lnk_cnt, 1);
		write (1, " ", 1);
		
		// owner
		print_spaces_until_word(ft_strlen(arg->owner), max_lengths.owner_len);
		write (1, arg->owner, ft_strlen(arg->owner));
		write (1, " ", 1);
		
		// group
		print_spaces_until_word(ft_strlen(arg->group), max_lengths.group_len);
		write (1, arg->group, ft_strlen(arg->group));
		write (1, " ", 1);
		
		// size
		print_spaces_until_word(ft_numlen(arg->size), max_lengths.size_len);
		ft_putnbr_fd(arg->size, 1);
		write (1, " ", 1);

		// last modified time
		tmp = format_time_split(arg->last_modif, arg->is_full_datetime);
		if (!tmp)
		{
			// 
			return (LS_ERR_RETURN_CODE_FATAL);
		}
		write(1, tmp[0], ft_strlen(tmp[0]));
		write(1, " ", 1);
		print_spaces_until_word(ft_strlen(tmp[1]), max_lengths.day_len);
		write(1, tmp[1], ft_strlen(tmp[1]));
		write(1, " ", 1);
		write(1, tmp[2], ft_strlen(tmp[2]));
		write(1, " ", 1);
		ft_free_2d_array(tmp);

		// name
		write (1, arg->name, ft_strlen(arg->name));
		write (1, "\n", 1);
	}
	else
	{
		write(1, arg->name, ft_strlen(arg->name));
		if (!(triggers & DIR_LAST_ELEM))
			write (1, "  ", 2);
		else
			write (1, "\n", 1);
	}

	return (LS_ERR_RETURN_CODE_NO_ERROR);
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

int		count_arg_info_lengths(t_list *order, t_info_max_lengths *max_lengths)
{
	arg_t	*data;
	size_t	current_len;
	char	**time_split;

	ft_memset(max_lengths, 0, sizeof(t_info_max_lengths));
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
		current_len = ft_numlen(data->size);
		if (current_len > max_lengths->size_len)
			max_lengths->size_len = current_len;
		// day
		time_split = format_time_split(data->last_modif, data->is_full_datetime);
		if (!time_split)
			return (LS_ERR_RETURN_CODE_FATAL);
		current_len = ft_strlen(time_split[1]);
		if (current_len > max_lengths->day_len)
			max_lengths->day_len = current_len;
		ft_free_2d_array(time_split);
		order = order->next;
	}
	return (0);
}

int		print_ordered(t_list *order, uint32_t opts, int triggers)
{
	t_info_max_lengths	max_lengths;
	int					ret;
	if ((LS_OPTION_l & opts) && !(triggers & DONT_PRINT_TOTAL))
	{
		print_total_blocks(order);
	}
	ret = count_arg_info_lengths(order, &max_lengths);
	if (ret)
		return (ret);
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
