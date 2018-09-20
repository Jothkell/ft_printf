/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mod.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkellehe <jkellehe@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 18:51:31 by jkellehe          #+#    #+#             */
/*   Updated: 2018/09/20 11:53:54 by jkellehe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"

int				bt_putstr_fd(char const *s, int fd, t_ap *tree)
{
	int			ret;

	ret = 0;
	if (!s)
		return (0);
	while (*s != '\0')
	{
		write(fd, s, 1);
		s++;
		ret++;
	}
	return (ret);
}

static int		count_size(intmax_t	n)
{
	size_t		i;

	i = 1;
	while (n /= 2)
		i++;
	return (i);
}
int	bt_putchar(char c, int times)
{
	int		count;

	count = times;
	while (times > 0)
	{
		write(1, &c, 1);
		times--;
	}
	return (count);
}

void			precwidth(int many, t_ap *tree, int prec)
{
	while (many > 0)
	{
		tree->ret += ((tree->z_pad && !tree->left && (tree->prec == 10000))
		|| prec) ? (write(1, "0", 1)) : (write(1, " ", 1));
		many--;
	}
}

int				bt_strlen(const char *s, t_ap *tree, int prec)
{
	int			i;
	int			len;

	len = 0;
	i = 0;
	while (s[i] != '\0')
		len += (s[i++] == '-' && prec) ? (0) : (1);
	len += (tree->plus && !tree->neg && !prec) ? (1) : (0);
	return (len);
}

char			*ft_pad(char *s, t_ap *tree)
{
	tree->prec = (tree->prec > ft_strlen(s)) ? (tree->prec) : (10000);
	tree->prec -= (tree->prec == 10000) ? (0) : (bt_strlen(s, tree, 1));
	tree->width -= (tree->prec == 10000) ? (bt_strlen(s, tree, 0))
		: (tree->prec + bt_strlen(s, tree, 0));
	tree->width += (!(tree->zero && tree->dot && !tree->z_pad)) ? (0) : (1);
	(!tree->left && !tree->z_pad) ? (precwidth(tree->width, tree, 0)) : (0);
	tree->ret += (Ox(tree) && IS_LOW(tree->c[0])) ? (write(1, "0x", 2)) : (0);
	tree->ret += (Ox(tree) && !IS_LOW(tree->c[0])) ? (write(1, "0X", 2)) : (0);
	tree->ret += (O(tree) && hash(tree)) ? (write(1, "0", 1)) : (0);
	tree->ret += (tree->plus && s[0] != '-' && tree->c[0] != 'u')
	? (write(1, "+", 1)) : (0);
	tree->ret += (s[0] == '-') ? (write(1, "-", 1)) : (0);
	s += (s[0] == '-') ? (1) : (0);
	(tree->z_pad && !tree->left) ? (precwidth(tree->width, tree, 0)) : (0);
	(tree->prec != 10000) ? (precwidth(tree->prec, tree, 1)) : (0);
	tree->ret += (SingleSpace(tree)) ? (write(1, " ", 1)) : (0);
	tree->ret += (!(tree->zero && tree->dot && !tree->z_pad)) ?
		(bt_putstr_fd(s, 1, tree)) : (0);
	(tree->left) ? (precwidth(tree->width, tree, 0)) : (0);
	return (s);
}

char			*ft_spad(char *s, int prec, t_ap *tree)
{
	char		*delet;

	tree->prec = (tree->prec > ft_strlen(s)) ? (ft_strlen(s)) : (tree->prec);
	tree->width -= (tree->prec == ft_strlen(s)) ?
		(ft_strlen(s)) : (ft_strlen(s) - tree->prec);

	delet = ft_strsub(s, 0, tree->prec);
	tree->ret += (tree->left && !(tree->zero && tree->dot)) ?
		(bt_putstr_fd(delet, 1, tree)) : (0);
	while (tree->width > 0)
	{
		tree->ret += (tree->z_pad && !tree->left) ?
			(write(1, "0", 1)) : (write(1, " ", 1));
		tree->width--;
	}
	while (tree->prec != 10000 && (tree->prec > 0) && NUMBERS(tree->c))
	{
		tree->ret += ((tree->c[0] != 'x' && tree->c[0] != 'X') || tree->z_pad) ?
			(write(1, "0", 1)) : (write(1, " ", 1));
		tree->prec--;
	}
	tree->ret += (!tree->left && !(tree->zero && tree->dot))
		? (bt_putstr_fd(delet, 1, tree)) : (0);
	return (s);
}

void			ft_putstr_fd_prec(char *s, int fd, int prec, t_ap *tree)
{
	if (FLOATS(tree->c))
		ft_pad(ft_strsub(s, 0, prec), tree);
	else if (NUMBERS(tree->c))
		ft_pad(s, tree);
	else
		ft_spad(s, prec, tree);
}

void			ft_put_wstr_fd_prec(wchar_t *s, int fd, int prec, t_ap *tree)
{
	int			i;

	i = 0;
	if (FLOATS(tree->c))
		i+=1;
	else if (NUMBERS(tree->c))
		i++;
	else
		i++;
}

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
	if (n == -9223372036854775808 && base == 10)
		return ("-9223372036854775808");
	len = count_size(n);
	tmp = n;
	if (n < 0 && ++len)
		tmp = -n;
	if (!(str = ft_strnew(len)))
		return (NULL);
	str[--len] = digits[tmp % base];
	while ((tmp = tmp / base))
		str[--len] = digits[(tmp % base)];
	if (n < 0)
		str[--len] = '-';
	hold = ft_strdup(&str[len]);
	free(str);
	return (hold);
}

char			*ft_umaxtoa_base(uintmax_t n, uintmax_t base, char *format)
{
	char		*str;
	int			len;
	uintmax_t	tmp;
	char		*digits;
	char		*hold;

	digits = "0123456789ABCDEF";
	if (IS_LOW(format[0]))
		digits = "0123456789abcdef";
	len = count_size(n);
	tmp = n;
	if (!(str = ft_strnew(len)))
		return (NULL);
	str[--len] = digits[tmp % base];
	while ((tmp = tmp / base))
		str[--len] = digits[(tmp % base)];
	hold = ft_strdup(&str[len]);
	free(str);
	return (hold);
}

intmax_t			decimals(double holder, float base, t_ap *tree)
{
	intmax_t		i = tree->prec;
	intmax_t 	tip;
	double 	top;

	holder *= (holder < 0) ? (-1) : (1);
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
	tip *= (tip < 0) ? (-1) : (1);
	if (((int)(holder * base) % (int)base) >= ((int)base /2))
	{
		tip += 1;
	}
	tree->ret += (tree->prec) ? (write(1, ".", 1)) : (0);
	return (tip);
}

void		floot(va_list ap, char *format, t_ap *tree)
{
	double	holder;
	float	base;
	intmax_t		temp;
	if (format[0] == 'f' || format[0] == 'F')
		base = 10;
	else
		base = 16;
	holder = va_arg(ap, double);
	temp = precision(format, ap, tree);
	if((tree->prec == 0) && ((((intmax_t)(holder * base) % (intmax_t)base) >= ((intmax_t)base /2)) ||
							 (((intmax_t)(-1 * holder * base) % (intmax_t)base) >= ((intmax_t)base /2))))
		holder += (holder > 0) ? (1) : (-1);
	ft_putstr_fd_prec(ft_maxtoa_base((intmax_t)holder, (intmax_t)base, format), 1, temp, tree);
	temp = (decimals(holder, base, tree));
	ft_putstr_fd_prec(ft_maxtoa_base((intmax_t)temp, (intmax_t)base, format), 1, tree->prec, tree);
}
