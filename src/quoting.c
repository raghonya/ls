#include "ls.h"

static int	is_safe(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
		return (1);
	if (c == '+' || c == ',' || c == '-' || c == '.' || c == '_' || c == '/')
		return (1);
	return (0);
}

static void	put_control_char(char c)
{
	ft_putstr_fd("$'", 1);
	if (c == '\r')
		ft_putstr_fd("\\r", 1);
	else if (c == '\f')
		ft_putstr_fd("\\f", 1);
	else if (c == '\n')
		ft_putstr_fd("\\n", 1);
	else if (c == '\t')
		ft_putstr_fd("\\t", 1);
	ft_putchar_fd('\'', 1);
}

void	print_filename_quoted(char *s, int needs_padding)
{
	int	i;
	int	quote_type = 0; // 0: none, 1: single, 2: double, 3: control
	int	has_cntrl = 0;

	for (i = 0; s[i]; i++)
	{
		if (s[i] < 32 || s[i] >= 127)
			has_cntrl = 1;
		else if (!is_safe(s[i]) && quote_type == 0)
			quote_type = 1;
		if (s[i] == '\'')
			quote_type = 2;
	}
	if (has_cntrl)
		quote_type = 3;

	// Alignment: If the name isn't quoted, add a space to match the opening '
	if (quote_type == 0 && needs_padding)
		ft_putchar_fd(' ', 1);

	if (quote_type == 0)
		ft_putstr_fd(s, 1);
	else if (quote_type == 3)
	{
		ft_putstr_fd("''", 1);
		for (i = 0; s[i]; i++)
		{
			if (s[i] < 32 || s[i] >= 127)
				put_control_char(s[i]);
			else
			{
				// If text follows a control char, ls wraps it in single quotes
				ft_putchar_fd('\'', 1);
				while (s[i] && (s[i] >= 32 && s[i] < 127))
					ft_putchar_fd(s[i++], 1);
				ft_putchar_fd('\'', 1);
				if (!s[i]) break;
				i--;
			}
		}
	}
	else if (quote_type == 2)
	{
		ft_putchar_fd('"', 1);
		ft_putstr_fd(s, 1);
		ft_putchar_fd('"', 1);
	}
	else // quote_type == 1
	{
		ft_putchar_fd('\'', 1);
		ft_putstr_fd(s, 1);
		ft_putchar_fd('\'', 1);
	}
}