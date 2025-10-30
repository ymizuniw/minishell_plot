#include "../../../../includes/minishell.h"

// generate a random num
size_t	ft_rand(size_t *seed)
{
	*seed = (*seed % INT_MAX);
	return (*seed);
}

char	*ft_mkstmp(char *template)
{
	static const char	charset[] = "ABCDEFGHIJKLMNOPQRSTUVWYZabcdefghijklmnopqrstuvwxyz0123456789";
	char				*p;
	size_t				len;
	size_t				num;
	void				*ptr;
	size_t				num_keep;
	char				*name;
	size_t				random;
	size_t				idx;

	len = strlen(template);
	ptr = &num;
	num = (unsigned long)ptr;
	num_keep = num;
	name = strdup(template);
	if (!name)
		return (NULL);
	p = name + len - 1;
	p = strchr(name, 'X');
	while (p != NULL)
	{
		random = ft_rand(&num);
		if (random == 0)
			random = 1;
		idx = (num / random) % (size_t)(sizeof(charset) - 1);
		*p = charset[idx];
		num /= 10;
		if (num == 0)
			num = num_keep;
		p = strchr(name, 'X');
	}
	return (name);
}

int	ft_mkstmpfd(char *template, char **filename)
{
	size_t	try_limit;
	int		fd;

	try_limit = 10000;
	if (template == NULL)
		return (-1);
	while (try_limit > 0)
	{
		*filename = ft_mkstmp(template);
		fd = open(*filename, O_CREAT | O_WRONLY | O_EXCL, 0600);
		if (fd >= 0)
			return (fd);
		try_limit--;
	}
	return (-1);
}
