#include "../inc/raycaster.h"

static char	*ft_join_and_free(char *result, char *buffer)
{
	char	*temp;

	temp = ft_strjoin(result, buffer);
	free(result);
	return (temp);
}

static char	*ft_rest(char *buffer)
{
	int		i;
	int		j;
	char	*line;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (!buffer[i])
	{
		free(buffer);
		return (NULL);
	}
	line = ft_calloc((ft_strlen(buffer) - i + 1), sizeof(char));
	if (!line)
		return (NULL);
	j = 0;
	i++;
	while (buffer[i])
	{
		line[j] = buffer[i];
		i++;
		j++;
	}
	free(buffer);
	return (line);
}

static char	*ft_find_line(char *buffer)
{
	char	*line;
	int		i;

	i = 0;
	if (!buffer[i])
	{
		return (NULL);
	}
	while (buffer[i] && buffer[i] != '\n')
		i++;
	line = ft_calloc(i + 2, sizeof(char));
	if (!line)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		line[i] = buffer[i];
		i++;
	}
	if (buffer[i] && buffer[i] == '\n')
		line[i] = '\n';
	return (line);
}

static char	*read_from_file(int fd, char *result)
{
	char	*buffer;
	int		byte_readed;

	if (!result)
	{
		result = ft_calloc(1, 1);
		if (!result)
			return (NULL);
	}
	buffer = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!buffer)
		return (NULL);
	byte_readed = 1;
	while (byte_readed > 0)
	{
		byte_readed = read(fd, buffer, BUFFER_SIZE);
		if (byte_readed == -1)
			return (free(buffer), free(result), NULL);
		buffer[byte_readed] = 0;
		result = ft_join_and_free(result, buffer);
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	return (free(buffer), result);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = read_from_file(fd, buffer);
	if (!buffer)
		return (NULL);
	line = ft_find_line(buffer);
	buffer = ft_rest(buffer);
	return (line);
}
