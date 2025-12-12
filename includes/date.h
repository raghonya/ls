#ifndef DATE_H
# define DATE_H

# define YEAR_WEIGHT	(24*60*30*12)
# define MONTH_WEIGHT	(24*60*30)
# define DAY_WEIGHT		(24*60)
# define HOUR_WEIGHT	(24)
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

# include <stdint.h>
# include <stdlib.h>
# include <time.h>
# include "libft.h"

typedef struct s_my_time
{
	uint64_t		year;
	uint64_t		month;
	uint64_t		day;
	uint64_t		hour;
	uint64_t		min;
} t_my_time;


#endif