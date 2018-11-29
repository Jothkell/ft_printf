/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itoa.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkellehe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/27 19:36:01 by jkellehe          #+#    #+#             */
/*   Updated: 2018/11/29 14:42:42 by jkellehe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"

char			*ft_maxtoa_base(intmax_t n, intmax_t base, char *format)
{
	char		*str;
	int			len;
	intmax_t	tmp;
	char		*digits;
	char		*hold;

	digits = "0123456789ABCDEF";
	if (IS_LOW(format[0]))
		digits = "0123456789abcdef";
	len = count_size(n);
	tmp = n;
	if (n < 0 && ++len)
		tmp *= -1;
	if (!(str = ft_strnew(len)))
		return (NULL);
	str[--len] = digits[ft_abs(tmp % base)];
	while ((tmp = tmp / base))
		str[--len] = digits[ft_abs(tmp % base)];
	if (n < 0)
		str[--len] = '-';
	hold = ft_strdup(&str[len]);
	free(str);
	return (hold);
}

char			*fubase(uintmax_t n, uintmax_t base, char *format, t_ap *tree)
{
	char		str[70];
	int			len;
	uintmax_t	tmp;
	char		*digits;

	digits = "0123456789ABCDEF";
	if (IS_LOW(format[0]))
		digits = "0123456789abcdef";
	len = count_usize(n);
	tmp = n;
	str[len--] = '\0';
	str[len] = digits[tmp % base];
	while ((tmp = tmp / base))
		str[--len] = digits[(tmp % base)];
	ft_putstr_fd_prec(&str[len], tree);
	return (NULL);
}

intmax_t		decimals(double holder, float base, t_ap *tree)
{
	intmax_t	i;
	intmax_t	tip;
	double		top;

	i = tree->prec;
	i = (i == 10000) ? (6) : (i);
	tip = (intmax_t)holder;
	top = (double)tip;
	holder -= top;
	while (i)
	{
		holder *= base;
		i--;
	}
	tip = (int)holder;
	holder *= (holder < 0) ? (-1) : (1);
	if (((int)(holder * base) % (int)base) >= ((int)base / 2))
	{
		if (tip == 0)
			tree->rd = 1;
		else
			tip += (tip > 0) ? (1) : (-1);
	}
	return (tip);
}

void			floot(va_list ap, char *format, t_ap *tree)
{
	long double holder;
	float		base;
	intmax_t	temp;
	int			prec;

	base = 10;
	if (format[-1] == 'L')
		holder = va_arg(ap, long double);
	else
		holder = (long double)va_arg(ap, double);
	precision(format, ap, tree);
	prec = tree->prec;
	temp = (decimals(holder, 10, tree));
	holder = (tree->rd && holder >= 0) ? (holder + 1) : (holder);
	holder = (tree->rd && holder < 0) ? (holder - 1) : (holder);
	tree->width -= (FLOOT(tree)) ? (7) : (tree->prec + 1);
	tree->prec = 10000;
	tree->ret += (holder > -1 && holder < 0) ? (write(1, "-", 1)) : (0);
	ft_putstr_fd_prec(ft_maxtoa_base((intmax_t)holder,
									(intmax_t)base, format), tree);
	tree->prec = (FLOOT(tree)) ? (6) : (prec);
	tree->ret += (tree->prec) ? (write(1, ".", 1)) : (0);
	tree->width = 0;
	ft_putstr_fd_prec(ft_maxtoa_base((intmax_t)temp,
									(intmax_t)base, format), tree);
}
