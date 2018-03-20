/*
** Copyright (C) 2018, Elouan Martinet <exa@elou.world>
** All rights reserved.
**
** This file is part of fdchecker.
**
** fdchecker is free software. It comes without any warranty, to
** the extent permitted by applicable law. You can redistribute it
** and/or modify it under the terms of the Do What the Fuck You Want
** to Public License, Version 2, as published by Sam Hocevar. See
** <http://www.wtfpl.net/> for more details.
*/

#include <errno.h>
#include <grp.h>
#include <limits.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef STAT_MAX
# define STAT_MAX 20
#endif

static char	filetype(mode_t fmt)
{
	if (fmt == S_IFDIR)
		return ('d');
	if (fmt == S_IFCHR)
		return ('c');
	if (fmt == S_IFBLK)
		return ('b');
	if (fmt == S_IFREG)
		return ('-');
	if (fmt == S_IFLNK)
		return ('l');
	if (fmt == S_IFSOCK)
		return ('s');
	if (fmt == S_IFIFO)
		return ('p');
	if (fmt == S_IFWHT)
		return ('w');
	return ('?');
}

static void	print_mode(mode_t mode)
{
	printf("%c", filetype(mode & S_IFMT));
	printf("%c", mode & S_IRUSR ? 'r' : '-');
	printf("%c", mode & S_IWUSR ? 'w' : '-');
	if (mode & S_ISUID)
		printf("%c", mode & S_IXUSR ? 's' : 'S');
	else
		printf("%c", mode & S_IXUSR ? 'x' : '-');
	printf("%c", mode & S_IRGRP ? 'r' : '-');
	printf("%c", mode & S_IWGRP ? 'w' : '-');
	if (mode & S_ISGID)
		printf("%c", mode & S_IXGRP ? 's' : 'S');
	else
		printf("%c", mode & S_IXGRP ? 'x' : '-');
	printf("%c", mode & S_IROTH ? 'r' : '-');
	printf("%c", mode & S_IWOTH ? 'w' : '-');
	if (mode & S_ISVTX)
		printf("%c", mode & S_IXOTH ? 't' : 'T');
	else
		printf("%c", mode & S_IXOTH ? 'x' : '-');
}

void		stat_fd(int fd)
{
	struct stat		sb;
	struct passwd	*pw;
	struct group	*gr;

	if (fstat(fd, &sb) < 0)
		printf("%d: %s\n", fd, strerror(errno));
	else
	{
		pw = getpwuid(sb.st_uid);
		gr = getgrgid(sb.st_gid);
		printf("%d: ", fd);
		print_mode(sb.st_mode);
		printf("/%d %s/%d %s/%d\n", sb.st_mode,
			pw->pw_name, sb.st_uid,
			gr->gr_name, sb.st_gid);
	}
}

int			reader(void)
{
	int		i;
	char	buf[PIPE_BUF];

	while (1)
	{
		i = read(0, buf, PIPE_BUF);
		if (i < 0)
		{
			perror("read");
			return (2);
		}
		if (i == 0)
			return (0);
		i = write(1, buf, i);
		if (i < 0)
		{
			perror("write");
			return (3);
		}
	}
}

int			main(int argc, char **argv)
{
	int			i;
	struct stat	sb;

	i = 0;
	while (i < argc - 1)
		printf("'%s' ", argv[i++]);
	printf("'%s'\n", argv[i++]);
	i = 0;
	while (i < STAT_MAX)
		stat_fd(i++);
	if (fstat(0, &sb) < 0)
		return (1);
	if ((sb.st_mode & S_IFMT) != S_IFIFO)
		return (0);
	return (reader());
}
