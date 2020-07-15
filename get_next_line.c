/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: UTurkey <uturkey@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/11 14:35:37 by UTurkey           #+#    #+#             */
/*   Updated: 2020/07/14 16:51:14 by UTurkey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int			status_r(t_gnl *gnl)				//Функция не влезла в рекурсию, для работы с одинаковым дескриптором, обнуление признака чтения, и обнуление статуса_чтения
{
	int i;

	i = 1;
	gnl->status_read = 0;
	gnl->first = 0;
	return (i);
}

static int			read_next_block(int fd, t_gnl *gnl)		//Просто чтение блока размером буфер, +признак чтения файла(что он уже был открыт), позиция ноль, количество прочитанного
{
	int				status;

	gnl->fd = fd;
	gnl->first = 's';
	gnl->position = 0;
	status = read(fd, gnl->buffer, BUFFER_SIZE);
	gnl->status_read = status;
	if (status < 1)							//Если чтение вернуло ошибку, необходимо вернуть пустой лайн, для этого позиция = последняя минус 1(для нормальной работы рекурсии)
	{
		gnl->position = BUFFER_SIZE - 1;
		gnl->buffer[gnl->position] = '\0';
		return (status);
	}
	return (1);
}

static int			get_next_char(int fd, char *char_to, t_gnl *gnl)	//Взятие символа из строки, которую мы вытащили из файла
{
	int				status;

	if (gnl->first && gnl->status_read < 1 && gnl->fd == fd)			//Если ридблок вернул ошибку чтения
	{
		*char_to = '\0';
		return (gnl->status_read);
	}
	if (!gnl->first || gnl->position == BUFFER_SIZE || gnl->fd != fd)		//Если файл еще не открывали или кусок, который мы брали из файла закончился или это новый файл
		status = read_next_block(fd, gnl);
	else
		status = 1;
	if (gnl->position < (size_t)gnl->status_read)
		*char_to = gnl->buffer[gnl->position++];
	else
	{
		*char_to = '\0';
		status = 0;
	}
	if (gnl->position == (size_t)gnl->status_read &&
			(gnl->position != BUFFER_SIZE || fd < 3) && *char_to != '\n')
		status = 0;
	if (!status)
		status_r(gnl);
	return (status);
}

static int			get_next_line_recursive_write(int fd, char **line,
		size_t step, t_gnl *gnl)						//Запись в рекурсии, сначала нашли строку, потом выделили память и потом записали
{
	char			sym;
	int				status;

	status = get_next_char(fd, &sym, gnl);
	if (status == 1 && sym != '\n')
	{
		status = get_next_line_recursive_write(fd, line, step + 1, gnl);
		if (status < 0)
			return (status);
		(*line)[step] = sym;
	}
	else if (status == 0 || sym == '\n')
	{
		if (sym != '\n' && sym != '\0')
			*line = (char *)malloc(sizeof(char) * (step + 2));
		else
			*line = (char *)malloc(sizeof(char) * (step + 1));
		if (!*line)
			return (-1);
		if (sym != '\n' && sym != '\0')
			(*line)[step++] = sym;
		(*line)[step] = '\0';
	}
	return (status);
}

int					get_next_line(int fd, char **line)
{
	static t_gnl	gnl;

	if (fd < 0 || BUFFER_SIZE < 1 || !line)
		return (-1);
	return (get_next_line_recursive_write(fd, line, 0, &gnl));
}
