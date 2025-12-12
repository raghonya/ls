#include "date.h"
#include "ls.h"

int	calc_date_diff(uint64_t diff)
{
	int		year;
	int		month;
	// int		day;
	// int		hour;
	// int		min;

	year = diff / YEAR_WEIGHT;
	diff -= year * YEAR_WEIGHT;
	month = diff / MONTH_WEIGHT;
	// diff -= month * MONTH_WEIGHT;
	// day = diff / DAY_WEIGHT;
	// diff -= day * DAY_WEIGHT;
	// hour = diff / HOUR_WEIGHT;
	// diff -= hour * HOUR_WEIGHT;
	// min = diff;

	return (month < 6);
}

int	recognize_month(char *month_str)
{
	if (ft_strcmp(month_str, "Jan") == 0)		return (JAN);
	else if (ft_strcmp(month_str, "Feb") == 0)	return (FEB);
	else if (ft_strcmp(month_str, "Mar") == 0)	return (MAR);
	else if (ft_strcmp(month_str, "Apr") == 0)	return (APR);
	else if (ft_strcmp(month_str, "May") == 0)	return (MAY);
	else if (ft_strcmp(month_str, "Jun") == 0)	return (JUN);
	else if (ft_strcmp(month_str, "Jul") == 0)	return (JUL);
	else if (ft_strcmp(month_str, "Aug") == 0)	return (AUG);
	else if (ft_strcmp(month_str, "Sep") == 0)	return (SEP);
	else if (ft_strcmp(month_str, "Oct") == 0)	return (OCT);
	else if (ft_strcmp(month_str, "Nov") == 0)	return (NOV);
	else if (ft_strcmp(month_str, "Dec") == 0)	return (DEC);
	return (0);
}

uint64_t	get_weighted_time(t_my_time *my_time)
{
	return (	my_time->year * YEAR_WEIGHT + 
				my_time->month * MONTH_WEIGHT + 
				my_time->day * DAY_WEIGHT + 
				my_time->hour * HOUR_WEIGHT + 
				my_time->min * MIN_WEIGHT);
}

int	parse_ctime(char *time_str, t_my_time *my_time)
{
	char	**date_parts;
	char	**time_parts;

	date_parts = ft_split(time_str, ' ');
	if (!date_parts || ft_len_2d_array(date_parts) != 5)
		return (1);
	my_time->month = recognize_month(date_parts[1]);
	my_time->day = ft_atoi(date_parts[2]);
	my_time->year = ft_atoi(date_parts[4]);
	time_parts = ft_split(date_parts[3], ':');
	if (!time_parts || ft_len_2d_array(time_parts) != 3)
		return (1);
	my_time->hour = ft_atoi(time_parts[0]);
	my_time->min = ft_atoi(time_parts[1]);
	ft_free_2d_array(time_parts);
	ft_free_2d_array(date_parts);

	return (0);
}

char *get_current_time()
{
	time_t currentTime;

	time(&currentTime);

	return (ctime(&currentTime));
}

int set_full_datetime_flag(arg_t *obj)
{
	int			ret;
	t_my_time	obj_datetime;
	t_my_time	current_datetime;
	
	ret = parse_ctime(ctime(&obj->last_modif), &obj_datetime);
	if (ret) return (ret);
	ret = parse_ctime(get_current_time(), &current_datetime);
	if (ret) return (ret);
	obj->is_full_datetime = calc_date_diff(
			get_weighted_time(&current_datetime) - 
			get_weighted_time(&obj_datetime));
	return (0);
}