
#include "../../../../includes/minishell.h"

char	*ext_unit(char *src, size_t start, size_t end)
{
	char	*unit;

	unit = xmalloc(sizeof(char) * (end - start + 2));
	strncpy(unit, &src[start], end - start + 2);
	return (unit);
}

int	join_value(char **res, char *value, size_t size1, size_t size2)
{
	*res = realloc(*res, sizeof(char) * (size1 + size2 + 1));
	if (res == NULL)
		return (0);
	strncpy(*res, value, size2 + 1);
	return (1);
}

// heredoc mode will expand all environment variables and quotations. It means that it won't recognize quotations.
char	*heredoc_value_expansion(char *line, bool in_quote, size_t line_len)
{
	size_t	i;
	char	*start;
	char	*res;
	size_t	start_idx;
	size_t	end_idx;
	char	*doller;
	char	*unit;

	i = 0;
	start = line;
	res = NULL;
	start_idx = 1;
	end_idx = 1;
	(void)in_quote;
	while (i < line_len)
	{
		doller = strchr(start, '$');
		if (doller != NULL)
		{
			while (&doller[end_idx] < &line[line_len])
			{
				unit = ext_unit(doller, start_idx, end_idx);
				if (unit == NULL)
					break ;
				// char *var = ft_getenv(unit);
				char *var = getenv(unit); // temporary.
				if (var != NULL)
				{
					if (join_value(&res, unit, strlen(res), strlen(var)))
						return (NULL);
					i += end_idx;
					break ;
				}
				end_idx++;
			}
		}
		start = &start[end_idx] + 1;
	}
	return (res);
}

char	*heredoc_expansion(char *line, bool in_quote, size_t line_len)
{
	char	*new;

	new = heredoc_value_expansion(line, in_quote, line_len);
	if (new == NULL)
		return (NULL);
	return (new);
}

int	get_document(t_redir *hd, char **document, size_t *document_len)
{
	char	*delim;
	size_t	delim_len;
	char	*line;
	size_t	line_len;
	char	*value;
	size_t	value_len;

	delim = hd->filename;
	delim_len = strlen(delim);
	line = NULL;
	line_len = 0;
	while (1)
	{
		line = readline("> ");
		if (strncmp(line, delim, delim_len) == 0)
			return (1);
		if (line != NULL)
			line_len = strlen(line);
		value = heredoc_expansion(line, hd->delim_quoted, line_len);
		value_len = strlen(value);
		if (join_value(document, value, *document_len, value_len) < 0)
			return (-1);
		*document_len += value_len;
		xfree(line);
		xfree(value);
	}
	return (-1);
}

ssize_t	heredoc_write_to_fd(int herepipe[2], char *document,
		size_t document_len)
{
	ssize_t	wb;

	wb = 0;
	wb = write(herepipe[1], document, document_len);
	if (wb != (ssize_t)document_len)
		return (0);
	return (wb);
}

int	get_tmp_fd(char *src, size_t size, char **filename)
{
	int		tmp_fd;
	ssize_t	wb;

	tmp_fd = ft_mkstmpfd(HERE_TEMPLATE, filename);
	if (tmp_fd < 0)
		return (-1);
	wb = write(tmp_fd, src, size);
	if (wb != (ssize_t)size)
	{
		close(tmp_fd);
		return (-1);
	}
	return (tmp_fd);
}

int	make_heredoc(t_redir *hd)
{
	char	*line;
	char	*document;
	size_t	document_len;
	char	*filename;
	int		fd;
	int		herepipe[2];
	int		ret;
	ssize_t	wb;
	int		tmp_fd;
	int		fd;

	line = NULL;
	document = NULL;
	document_len = 0;
	filename = NULL;
	if (get_document(hd, &document, &document_len) < 0)
		return (-1);
	if (document_len == 0)
	{
		fd = open("dev/null", O_RDONLY);
		if (fd < 0)
			perror("open: ");
		xfree(document);
		return (fd);
	}
	if (document_len + 1 <= HERE_PIPE_SIZE)
	{
		ret = pipe(herepipe);
		if (ret < 0)
		{
			perror("pipe: ");
			xfree(document);
		}
		wb = heredoc_write_to_fd(herepipe, document, document_len);
		close(herepipe[1]);
		if (wb == 0)
		{
			close(herepipe[0]);
			unlink(filename);
			free(filename);
			return (-1);
		}
		return (herepipe[0]);
	}
	else
	{
		tmp_fd = get_tmp_fd(document, document_len, &filename);
		if (tmp_fd < 0)
		{
			xfree(line);
			xfree(document);
			return (-1);
		}
		fd = open(filename, O_RDONLY);
		if (fd < 0)
		{
			perror("open: ");
			close(tmp_fd);
			unlink(filename);
			xfree(filename);
			return (-1);
		}
		close(tmp_fd);
		return (fd);
	}
	return (-1);
}
