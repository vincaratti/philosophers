/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcaratti <vcaratti@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 13:38:17 by vcaratti          #+#    #+#             */
/*   Updated: 2024/11/09 14:46:03 by vincenzocarat    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	char	out;

	if (n == -2147483648)
	{
		write(fd, "-", 1);
		write(fd, "2", 1);
		n += 2000000000;
		n = -n;
	}
	if (n < 0)
	{
		write(fd, "-", 1);
		n = -n;
	}
	if (n / 10)
		ft_putnbr_fd(n / 10, fd);
	out = n % 10 + 48;
	write(fd, &out, 1);
}
