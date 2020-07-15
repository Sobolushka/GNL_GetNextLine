/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: UTurkey <uturkey@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/11 14:39:09 by UTurkey           #+#    #+#             */
/*   Updated: 2020/07/11 14:40:04 by UTurkey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H
# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 65534
# endif
# if BUFFER_SIZE < 1
#  error "error BUFFER_SIZE"
# endif
# if BUFFER_SIZE > 65534
#  error "error BUFFER_SIZE"
# endif

typedef struct		s_gnl
{
	char			buffer[BUFFER_SIZE];
	int				status_read;
	char			first;
	size_t			position;
	int				fd;
}					t_gnl;

int					get_next_line(int fd, char **line);

#endif
