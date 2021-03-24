/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_intlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 15:03:31 by clorin            #+#    #+#             */
/*   Updated: 2021/03/17 15:05:46 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t			ft_intlen(intmax_t nb)
{
	size_t		len;

	len = 0;
	if (!nb)
		len++;
	while (nb)
	{
		nb = nb / 10;
		len++;
	}
	return (len);
}
