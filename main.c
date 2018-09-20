/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkellehe <jkellehe@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/22 15:12:22 by jkellehe          #+#    #+#             */
/*   Updated: 2018/09/20 11:43:47 by jkellehe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"

int					precision(char *format, va_list ap, t_ap *tree)
{
	tree->prec = 10000;
	tree->width = 0;
	while (isDIGIT(format[-1]) || format[-1] == '.')
		format--;
	tree->z_pad = (format[0] == '0' && format[1] != '.') ? (1) : (tree->z_pad);
	tree->prec = (format[0] == '.') ? (ft_atoi(&format[1])) : (tree->prec);
	tree->width = (isDIGIT(format[0])) ? (ft_atoi(&format[0])) : (tree->width);
	while (tree->dot && format[0] != '.')
		format++;
	tree->width = (format[1] == '*' && tree->dot) ? (va_arg(ap, int))
		: (tree->width);
	tree->prec = (isDIGIT(format[1]) && tree->dot) ? (ft_atoi(&format[1]))
		: (tree->prec);
	tree->width -= (tree->hash && X(tree)) ? (2) : (0);
	tree->width -= ((O(tree) && tree->hash)) ? (1) : (0);
	return (tree->prec);
}

void				udigit(va_list ap, char *format, t_ap *tree)
{
	intmax_t		base;
	uintmax_t		uholder;

	uholder = 0;
	if (!(baseTEN(*format)) || !(base = 10))
		base = (format[0] == 'x' || format[0] == 'X') ? (16) : (2);
	base = (format[0] == 'o' || format[0] == 'O') ? (8) : (base);
	if (is_unsign(format) && HH(format))
		uholder = (uintmax_t)va_arg(ap, unsigned char);
	else if (format[-1] == 'h' && is_unsign(format))
		uholder = (uintmax_t)va_arg(ap, unsigned short);
	else if (is_unsign(format) && LL(format))
		uholder = (uintmax_t)va_arg(ap, unsigned long long);
	else if ((is_unsign(format) && format[-1] == 'l') || format[0] == 'U')
		uholder = (uintmax_t)va_arg(ap, unsigned long);
	else if (is_unsign(format) && format[-1] == 'j')
		uholder = (uintmax_t)va_arg(ap, uintmax_t);
	else if (format[-1] == 'z')
		uholder = (uintmax_t)va_arg(ap, size_t);
	else
		uholder = (uintmax_t)va_arg(ap, unsigned int);
	tree->zero = (uholder == 0) ? (1) : (0);
	ft_putstr_fd_prec(ft_umaxtoa_base(uholder, base, tree->c), 1,
		precision(format, ap, tree), tree);
}

void				digit(va_list ap, char *format, t_ap *tree)
{
	intmax_t		holder;
	intmax_t		base;

	holder = 0;
	if (!(baseTEN(*format)) || !(base = 10))
		base = (format[0] == 'x' || format[0] == 'X') ? (16) : (2);
	base = (format[0] == 'o' || format[0] == 'O') ? (8) : (base);
	if (HH(format))
		holder = (intmax_t)va_arg(ap, char);
	else if (format[-1] == 'h')
		holder = (intmax_t)va_arg(ap, short);
	else if (LL(format))
		holder = (intmax_t)va_arg(ap, long long);
	else if (format[-1] == 'l')
		holder = (intmax_t)va_arg(ap, long);
	else if (format[-1] == 'j')
		holder = (intmax_t)va_arg(ap, intmax_t);
	else if (format[-1] == 'z')
		holder = (intmax_t)va_arg(ap, size_t);
	else
		holder = (intmax_t)va_arg(ap, int);
	tree->neg = (holder < 0) ? (1) : (0);
	tree->zero = (holder == 0) ? (1) : (0);
	ft_putstr_fd_prec(ft_maxtoa_base(holder, base, format), 1,
		precision(format, ap, tree), tree);
}

void			str(va_list ap, char *format, t_ap *tree)
{
	char		*hold;

	hold = va_arg(ap, char*);
	if (!hold)
		tree->ret += write(1, "(null)", 6);
	else
		ft_putstr_fd_prec(hold, 1, precision(format, ap, tree), tree);
}

void			character(va_list ap, char *format, t_ap *tree)
{
	wchar_t		c;

	c = (unsigned char)va_arg(ap, int);
	precision(format, ap, tree);
	tree->width--;
	tree->ret += ((tree->width > 0) && !tree->left) ?
		(bt_putchar(' ', tree->width)) : (0);
	if (!c)
		tree->ret += (write(1, "^@", 2) - 1);
	else
		tree->ret += write(1, &c, 1);
	tree->ret += ((tree->width > 0) && tree->left) ?
		(bt_putchar(' ', tree->width)) : (0);
}

void			percent(va_list ap, char *format, t_ap *tree)
{
	ap += 0;
	precision(format, ap, tree);
	tree->width--;
	while (((tree->width) > 0) && !(tree->left))
	{
		tree->ret += (tree->z_pad) ? (write(1, "0", 1)) : (write(1, " ", 1));
		tree->width--;
	}
	write(1, "%", 1);
	tree->ret++;
	while (((tree->width) > 0) && (tree->left))
	{
		tree->ret += (tree->z_pad) ? (write(1, "0", 1)) : (write(1, " ", 1));
		tree->width--;
	}
}

int		zero_struct(t_ap *tree)
{
	tree->fd = 1;
	tree->decimal = 0;
	tree->zero = 0;
	tree->prec = 0;
	tree->left = 0;
	tree->X = 0;
	tree->O = 0;
	tree->percent = 0;
	tree->l = 0;
	tree->ll = 0;
	tree->fd = 0;
	tree->decimal = 0;
	tree->hash = 0;
	tree->zero = 0;
	tree->z_pad = 0;
	return ((tree->dot = 0));
}

int		ass_f(void (**p) (va_list ap, char *format, t_ap *tree), t_ap *tree)
{
	p['U'] = udigit;
	p['u'] = udigit;
	p['O'] = udigit;
	p['o'] = udigit;
	p['b'] = digit;
	p['X'] = udigit;
	p['x'] = udigit;
	p['l'] = digit;
	p['d'] = digit;
	p['D'] = udigit;
	p['i'] = digit;
	p['s'] = str;
	p['S'] = str;
	p['c'] = character;
	p['%'] = percent;
	p['f'] = floot;
	p['F'] = floot;
	p['a'] = floot;
	p['A'] = floot;
	return (zero_struct(tree));
}

void			flags(char *c, t_ap *tree)
{
	tree->left = (*c == '-') ? (1) : (tree->left);
	tree->l = (*c == 'l') ? (1) : (tree->l);
	tree->ll = (*c == 'l' && c[-1] == 'l') ? (1) : (tree->ll);
	tree->hash = (*c == '#') ? (1) : (tree->hash);
	tree->dot = (*c == '.') ? (1) : (tree->dot);
	tree->plus = (*c == '+') ? (1) : (tree->plus);
	tree->space = (*c == ' ') ? (1) : (tree->space);
	tree->z_pad = (*c == '0' && c[-1] == '%') ? (1) : (tree->z_pad);
}

int				ft_printf(const char *restrict format, ...)
{
	va_list		ap;
	void		(*p[123]) (va_list ap, char *format, t_ap *tree);
	int			i;
	t_ap		*tree;

	if (!(tree = (t_ap*)ft_memalloc(sizeof(t_ap) * 3)))
		return (0);
	i = ass_f(p, tree);
	tree->ret = 0;
	va_start(ap, format);
	while (format[i] != '\0')
		if (format[i] == '%')
		{
			i++;
			while (!IS_TYPE(format[i]) && format[i] != '\0')
				flags((char*)&format[i++], tree);
			tree->c = (char*)&format[i];
			p[format[i]](ap, (char*)&format[i++], tree);
		}
		else
			tree->ret += write(1, &format[i++], 1);
	va_end(ap);
	return (tree->ret);
}

int main()
{
	char	*hey = "whoa";
	unsigned long	dude = 420;
	int ret = 0;
	int ret2 = 0;
	int fort2 = 42;
	unsigned short USHRT_MAX = 65535;
	double dog = 420420420420.555555;
	double doggy = 420.55555555555555;
	ret = ft_printf("%f", dog);
	printf("\n");
	ret2 = printf("%f", dog);
 	printf("\n");
	printf("%d %d\n", ret, ret2);
	return(0);
}

