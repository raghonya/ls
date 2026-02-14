#include "ls.h"
#include <stdbool.h>

bool	is_safe(unsigned char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
		return true;
	return (c == '.' || c == '_' || c == '-' || c == '/');
}

static void put_octal(unsigned char c)
{
	ft_putstr_fd("\\", STDOUT_FILENO);
	ft_putchar_fd(((c >> 6) & 7) + '0', STDOUT_FILENO);
	ft_putchar_fd(((c >> 3) & 7) + '0', STDOUT_FILENO);
	ft_putchar_fd((c & 7) + '0', STDOUT_FILENO);
}

static void handle_ctrl_chars(const char *name, int *i)
{
	ft_putstr_fd("'$'", STDOUT_FILENO);
	while (name[*i] && ((unsigned char)name[*i] < 32 || (unsigned char)name[*i] >= 127))
	{
		unsigned char c = (unsigned char)name[*i];
		if (c == '\r')		ft_putstr_fd("\\r", STDOUT_FILENO);
		else if (c == '\n') ft_putstr_fd("\\n", STDOUT_FILENO);
		else if (c == '\f') ft_putstr_fd("\\f", STDOUT_FILENO);
		else if (c == '\t') ft_putstr_fd("\\t", STDOUT_FILENO);
		else if (c == '\v') ft_putstr_fd("\\v", STDOUT_FILENO);
		else				put_octal(c);
		(*i)++;
	}
	ft_putstr_fd("''", STDOUT_FILENO);
}

void print_filename_quoted(const char *name, uint16_t triggers)
{
	bool has_ctrl = false;
	bool has_unsafe = false;
	bool has_squote = false;

	for (int i = 0; name[i]; i++)
	{
		unsigned char c = (unsigned char)name[i];
		if (c < 32 || c >= 127)
			has_ctrl = has_unsafe = true;
		else if (!is_safe(c))
			has_unsafe = true;	
		if (c == '\'')
			has_squote = true;
	}

	if (!has_unsafe)
	{
		if (triggers & SPACE_BEFORE_NAME)
			ft_putchar_fd(' ', STDOUT_FILENO);
		ft_putstr_fd((char *)name, STDOUT_FILENO);
		return;
	}

	if (has_ctrl) {
		ft_putchar_fd('\'', STDOUT_FILENO);
		for (int i = 0; name[i]; )
		{
			if ((unsigned char)name[i] < 32 || (unsigned char)name[i] >= 127)
				handle_ctrl_chars(name, &i);
			else
			{
				if (name[i] == '\'')
					ft_putstr_fd("'\\''", STDOUT_FILENO);
				else
					ft_putchar_fd(name[i], STDOUT_FILENO);
				i++;
			}
		}
		ft_putchar_fd('\'', STDOUT_FILENO);
	}
	else if (has_squote && !ft_strchr(name, '"'))
	{
		ft_putchar_fd('"', STDOUT_FILENO);
		ft_putstr_fd((char *)name, STDOUT_FILENO);
		ft_putchar_fd('"', STDOUT_FILENO);
	}
	else
	{
		ft_putchar_fd('\'', STDOUT_FILENO);
		for (int i = 0; name[i]; i++)
		{
			if (name[i] == '\'')
				ft_putstr_fd("'\\''", STDOUT_FILENO);
			else
				ft_putchar_fd(name[i], STDOUT_FILENO);
		}
		ft_putchar_fd('\'', STDOUT_FILENO);
	}
}