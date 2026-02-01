#include "date.h"
#include "ls.h"

// int	recognize_month(char *month_str)
// {
// 	if		(ft_strcmp(month_str, "Jan") == 0) return (JAN);
// 	else if (ft_strcmp(month_str, "Feb") == 0) return (FEB);
// 	else if (ft_strcmp(month_str, "Mar") == 0) return (MAR);
// 	else if (ft_strcmp(month_str, "Apr") == 0) return (APR);
// 	else if (ft_strcmp(month_str, "May") == 0) return (MAY);
// 	else if (ft_strcmp(month_str, "Jun") == 0) return (JUN);
// 	else if (ft_strcmp(month_str, "Jul") == 0) return (JUL);
// 	else if (ft_strcmp(month_str, "Aug") == 0) return (AUG);
// 	else if (ft_strcmp(month_str, "Sep") == 0) return (SEP);
// 	else if (ft_strcmp(month_str, "Oct") == 0) return (OCT);
// 	else if (ft_strcmp(month_str, "Nov") == 0) return (NOV);
// 	else if (ft_strcmp(month_str, "Dec") == 0) return (DEC);

// 	return (0);
// }

// int	get_month_day_count(char *month_str, uint16_t year)
// {
// 	if		(ft_strcmp(month_str, "Jan") == 0) return (JAN_DAYS);
// 	else if (ft_strcmp(month_str, "Feb") == 0) return (FEB_DAYS(year));
// 	else if (ft_strcmp(month_str, "Mar") == 0) return (MAR_DAYS);
// 	else if (ft_strcmp(month_str, "Apr") == 0) return (APR_DAYS);
// 	else if (ft_strcmp(month_str, "May") == 0) return (MAY_DAYS);
// 	else if (ft_strcmp(month_str, "Jun") == 0) return (JUN_DAYS);
// 	else if (ft_strcmp(month_str, "Jul") == 0) return (JUL_DAYS);
// 	else if (ft_strcmp(month_str, "Aug") == 0) return (AUG_DAYS);
// 	else if (ft_strcmp(month_str, "Sep") == 0) return (SEP_DAYS);
// 	else if (ft_strcmp(month_str, "Oct") == 0) return (OCT_DAYS);
// 	else if (ft_strcmp(month_str, "Nov") == 0) return (NOV_DAYS);
// 	else if (ft_strcmp(month_str, "Dec") == 0) return (DEC_DAYS);

// 	return (0);
// }

// int	parse_ctime(char *time_str, t_my_time *my_time)
// {
// 	char	**date_parts;
// 	char	**time_parts;

// 	date_parts = ft_split(time_str, ' ');
// 	if (!date_parts || ft_len_2d_array(date_parts) != 5)
// 	{
// 		g_err.type = LS_ERR_TIME_PARSE_ERROR;
// 		return (LS_RETURN_CODE_FATAL);
// 	}
// 	my_time->month = recognize_month(date_parts[1]);
// 	if (my_time->month == 0)
// 	{
// 		ft_free_2d_array(date_parts);
// 		g_err.type = LS_ERR_TIME_PARSE_ERROR;
// 		return (LS_RETURN_CODE_FATAL);
// 	}
// 	my_time->day = ft_atoi(date_parts[2]);
// 	my_time->year = ft_atoi(date_parts[4]);
// 	time_parts = ft_split(date_parts[3], ':');
// 	if (!time_parts || ft_len_2d_array(time_parts) != 3)
// 	{
// 		ft_free_2d_array(date_parts);
// 		g_err.type = LS_ERR_TIME_PARSE_ERROR;
// 		return (LS_RETURN_CODE_FATAL);
// 	}
// 	my_time->hour = ft_atoi(time_parts[0]);
// 	my_time->min = ft_atoi(time_parts[1]);
// 	my_time->days_in_month = get_month_day_count(date_parts[1], my_time->year);
// 	ft_free_2d_array(time_parts);
// 	ft_free_2d_array(date_parts);

// 	return (LS_RETURN_CODE_NO_ERROR);
// }

char *get_current_time()
{
	time_t currentTime;

	currentTime = time(NULL);

	return (ctime(&currentTime));
}

int set_full_datetime_flag(time_t input_time, int *is_full_datetime)
{
	time_t		current_time;
	time_t		diff;

	current_time = time(NULL);
	diff = current_time - input_time;
	*is_full_datetime = !(diff > 15768000 || diff < 0); //15768000 seconds = 6 months

	return (LS_RETURN_CODE_NO_ERROR);
}

char **format_time_split(time_t input_time, int is_full_datetime)
{
	char	*tmp;
	char	*time_str;
	char	*formatted_str;
	char	**splitted;

	time_str = ctime(&input_time);
	if (!time_str)
	{
		g_err.type = LS_ERR_TIME_PARSE_ERROR;
		return (NULL);
	}
	formatted_str = NULL;
	splitted = ft_split(time_str, ' ');
	if (!splitted || ft_len_2d_array(splitted) != 5)
	{
		g_err.type = LS_ERR_TIME_PARSE_ERROR;
		goto end;
	}
	formatted_str = ft_strjoin_w_delim(splitted[1], splitted[2], ' ');
	if (!formatted_str)
	{
		g_err.type = LS_ERR_NOT_ENOUGH_MEMORY;
		goto end;
	}
	tmp = formatted_str;
	if (is_full_datetime)
	{
		splitted[3][5] = 0; // take only "HH:MM"
		formatted_str = ft_strjoin_w_delim(formatted_str, splitted[3], ' ');
	}
	else
	{
		splitted[4][4] = 0; // take only "yyyy"
		formatted_str = ft_strjoin_w_delim(formatted_str, splitted[4], ' ');
	}
	free (tmp);
end:
	ft_free_2d_array(splitted);
	splitted = ft_split(formatted_str, ' ');
	if (splitted == NULL)
		g_err.type = LS_ERR_NOT_ENOUGH_MEMORY;
	free(formatted_str);
	return (splitted);
}
