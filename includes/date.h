#ifndef DATE_H
# define DATE_H

// # define YEAR_WEIGHT	(12*30*24*60)
// # define MONTH_WEIGHT	(30*24*60)
# define DAY_WEIGHT		(24*60)
# define HOUR_WEIGHT	(60)
# define MIN_WEIGHT		(1)

# define JAN 1
# define FEB 2
# define MAR 3
# define APR 4
# define MAY 5
# define JUN 6
# define JUL 7
# define AUG 8
# define SEP 9
# define OCT 10
# define NOV 11
# define DEC 12

# define JAN_DAYS 31
# define FEB_DAYS(year) ((year % 4 == 0 ? 29 : 28))
# define MAR_DAYS 31
# define APR_DAYS 30
# define MAY_DAYS 31
# define JUN_DAYS 30
# define JUL_DAYS 31
# define AUG_DAYS 31
# define SEP_DAYS 30
# define OCT_DAYS 31
# define NOV_DAYS 30
# define DEC_DAYS 31

# include <stdint.h>
# include <stdlib.h>
# include <time.h>
# include "libft.h"

typedef struct s_my_time
{
	uint16_t	year;
	uint16_t	month;
	uint16_t	day;
	uint16_t	hour;
	uint16_t	min;
	uint16_t	days_in_month;
} t_my_time;

#endif