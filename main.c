/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkellehe <jkellehe@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/22 15:12:22 by jkellehe          #+#    #+#             */
/*   Updated: 2018/11/27 16:33:24 by jkellehe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"

int					precision(char *format, va_list ap, t_ap *tree)
{
	tree->prec = 10000;
	tree->width = 0;
	while (isDIGIT(format[-1]) || prec1(format))
		format--;
	(format[0] == '*') ? (tree->width = va_arg(ap, int)) : (0);
	tree->left = (tree->width < 0) ? (1) : (tree->left);
	tree->width = (tree->width < 0) ? (-tree->width) : (tree->width);
	format += (format[0] == '*') ? (1) : (0);
	tree->z_pad = (format[0] == '0' && format[1] != '.') ? (1) : (tree->z_pad);
	tree->prec = (format[0] == '.') ? (ft_atoi(&format[1])) : (tree->prec);
	tree->width = (isDIGIT(format[0])) ? (ft_atoi(&format[0])) : (tree->width);
	format += (isDIGIT(format[0])) ? (1) : (0);
	(format[0] == '*') ? (tree->width = va_arg(ap, int)) : (0);
	while (tree->dot && format[0] != '.')
		format++;
	tree->prec = (format[1] == '*' && tree->dot) ? (va_arg(ap, int))
		: (tree->prec);
	tree->prec = (IS_TYPE(format[1]) && tree->dot) ? (0)
		: (tree->prec);
	tree->prec = (isDIGIT(format[1]) && tree->dot) ? (ft_atoi(&format[1]))
		: (tree->prec);
	tree->width -= ((tree->hash && X(tree)) || tree->c[0] == 'p') ? (2) : (0);
	tree->width -= ((O(tree) && tree->hash)) ? (1) : (0);
	return (tree->prec);
}

void				udigit(va_list ap, char *format, t_ap *tree)
{
	intmax_t		base;
	uintmax_t		uholder;
	//char			*delet;

	if (!(baseTEN(*format)) || !(base = 10))
		base = (format[0] == 'x' || format[0] == 'X') ? (16) : (2);
	base = (format[0] == 'o' || format[0] == 'O') ? (8) : (base);
	if (format[0] == 'O')
		uholder = (uintmax_t)va_arg(ap, unsigned long);
	else if (is_unsign(format) && HH(format))
		uholder = (uintmax_t)(unsigned char)va_arg(ap, int);
	else if (format[-1] == 'h' && is_unsign(format))
		uholder = (uintmax_t)(unsigned short)va_arg(ap, int);
	else if (is_unsign(format) && LL(format))
		uholder = (uintmax_t)va_arg(ap, unsigned long long);
	else if (udigit1(format))
		uholder = (uintmax_t)va_arg(ap, unsigned long);
	else if (is_unsign(format) && format[-1] == 'j')
		uholder = (uintmax_t)va_arg(ap, uintmax_t);
	else if (format[-1] == 'z')
		uholder = (uintmax_t)va_arg(ap, size_t);
	else
		uholder = (uintmax_t)va_arg(ap, unsigned int);
	tree->zero = (uholder == 0) ? (1) : (0);
	precision(format, ap, tree);
	ft_umaxtoa_base(uholder, base, tree->c, tree);
	//ft_putstr_fd_prec((delet = ft_umaxtoa_base(uholder, base, tree->c)), tree);
	//free(delet);  //this is all the leaks somehow
}

void				digit(va_list ap, char *format, t_ap *tree)
{
	intmax_t		holder;
	intmax_t		base;
	char *delet;

	precision(format, ap, tree);
	holder = 0;
	if (!(baseTEN(*format)) || !(base = 10))
		base = (format[0] == 'x' || format[0] == 'X') ? (16) : (2);
	base = (format[0] == 'o' || format[0] == 'O') ? (8) : (base);
	if (HH(format))
		holder = (intmax_t)(char)va_arg(ap, int);
	else if (format[-1] == 'h')
		holder = (intmax_t)(short)va_arg(ap, int);
	else if (LL(format))
		holder = (intmax_t)va_arg(ap, long long);
	else if (format[-1] == 'l' || format[0] == 'D')
		holder = (intmax_t)va_arg(ap, long);
	else if (format[-1] == 'j')
		holder = (intmax_t)va_arg(ap, intmax_t);
	else if (format[-1] == 'z')
		holder = (intmax_t)va_arg(ap, size_t);
	else
		holder = (intmax_t)va_arg(ap, int);
	tree->neg = (holder < 0) ? (1) : (0);
	tree->zero = (holder == 0) ? (1) : (0);
	ft_putstr_fd_prec((delet = ft_maxtoa_base(holder, base, format)), tree);
	free(delet);
}

void				big_digit(va_list ap, char *format, t_ap *tree)
{
	intmax_t		holder;
	intmax_t		base;
	char			*delet;

	precision(format, ap, tree);
	base = 10;
	holder = 0;
	if (format[-1] == 'h')
		holder = (intmax_t)va_arg(ap, int);
	else if (format[0] == 'D')
		holder = (intmax_t)va_arg(ap, long);
	tree->neg = (holder < 0) ? (1) : (0);
	tree->zero = (holder == 0) ? (1) : (0);
	ft_putstr_fd_prec((delet = ft_maxtoa_base(holder, base, format)), tree);
	free(delet);
}

int			bt_putwstr(wchar_t *s, t_ap *tree)
{
	int i;
	int len;

	i = 0;
	len = 0;
	if (tree->c[0] == 'c' || tree->c[0] == 'C')
		return (put_wc(s[0]));
	while (s[i] && i < tree->prec && len < tree->prec)
	{
		len += put_wc(s[i]);
		i++;
	}
	return (len);
}

void			str(va_list ap, char *format, t_ap *tree)
{
	char		*hold;
	wchar_t		*yeah;

	precision(format, ap, tree);
	if (thicc(tree->c))
	{
		yeah = va_arg(ap, wchar_t*);
		if (!yeah && !tree->z_pad)
			tree->ret += write(1, "(null)", 6);
		else if (!yeah)
			ft_wpad((wchar_t*)"\0", tree);
		else
			ft_wpad(yeah, tree);
		return ;
	}
	hold = va_arg(ap, char*);
	if (!hold && !tree->z_pad)
		tree->ret += write(1, "(null)", 6);
	else if (!hold)
		ft_putstr_fd_prec("\0", tree);
	else
		ft_putstr_fd_prec(hold, tree);
}

void			wchar(va_list ap, char *format, t_ap *tree)
{
	wchar_t	c;

	precision(format, ap, tree);
	c = va_arg(ap, wchar_t);
	tree->width--;
	tree->ret += ((tree->width > 0) && !tree->left) ?
		(bt_putchar(' ', tree->width)) : (0);
	ft_wpad(&c, tree);
	tree->ret += ((tree->width > 0) && tree->left) ?
		(bt_putchar(' ', tree->width)) : (0);
}

void			character(va_list ap, char *format, t_ap *tree)
{
	char		c;

	if (format[-1] == 'l')
	{
		wchar(ap, format, tree);
		return ;
	}
	precision(format, ap, tree);
	tree->ret += ((--tree->width > 0) && !tree->left && !tree->z_pad) ?
		(bt_putchar(' ', tree->width)) : (0);
	tree->ret += ((tree->width > 0) && !tree->left && tree->z_pad) ?
		(bt_putchar('0', tree->width)) : (0);
	c = (char)va_arg(ap, int);
	tree->ret += write(1, &c, 1);
	tree->ret += ((tree->width > 0) && tree->left && !tree->z_pad) ?
	(bt_putchar(' ', tree->width)) : (0);
	tree->ret += ((tree->width > 0) && tree->left && tree->z_pad) ?
		(bt_putchar('0', tree->width)) : (0);
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

int		zs(t_ap *tree, int i)
{
	i += 0;
	tree->dot = 0;
	tree->preast = 0;
	tree->posast = 0;
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
	tree->fd = 1;
	tree->decimal = 0;
	tree->hash = 0;
	tree->zero = 0;
	tree->z_pad = 0;
	return ((tree->dot = 0));
}

void	oh(va_list ap, char *format, t_ap *tree)
{
	ap += 0;
	format += 0;
	tree->ret = -1;
}

void	addy(va_list ap, char *format, t_ap *tree)
{
	char	*yeah;
	char	*hold;

	precision(format, ap, tree);
	yeah = va_arg(ap, char*);
	tree->zero = (yeah == NULL) ? (1) : (0);
	tree->hash = 1;
	if (!yeah)
	{
		hold = (char*)malloc(sizeof(char) * 2);
		hold[0] = '0';
		hold[1] = '\0';
	}
	else
		hold = ft_maxtoa_base((intmax_t)yeah, 16, format);
	ft_pad(hold, tree);
	free(hold);
}

void	non(va_list ap, char *format, t_ap *tree)
{
	char	*hold;
	int		prec;

    prec = precision(format, ap, tree);
    hold = (char*)malloc(sizeof(char));
	if(*format == 0 && format[-1] == '%')
		hold += 0;
	else if (*format == 10)
	{
		ft_strncpy(hold, format, 1);
		ft_putstr_fd_prec(hold, tree);
	}
	else
	{
		ft_strncpy(hold, tree->c, 1);
		ft_putstr_fd_prec(hold, tree);
	}
	free(hold);
	return ;
}

void	nullify(void (**p) (va_list ap, char *format, t_ap *tree), t_ap *tree)
{
	int i;

	i = 0;
	tree->ret = 0;
	while (i <= 126)
	{
		p[i] = non;
		i++;
	}
}

int		ass_f(void (**p) (va_list ap, char *format, t_ap *tree), t_ap *tree)
{
	nullify(p, tree);
	p['p'] = addy;
	p['U'] = udigit;
	p['u'] = udigit;
	p['O'] = udigit;
	p['o'] = udigit;
	p['b'] = digit;
	p['X'] = udigit;
	p['x'] = udigit;
	p['l'] = digit;
	p['d'] = digit;
	p['D'] = big_digit;
	p['i'] = digit;
	p['s'] = str;
	p['S'] = str;
	p['C'] = wchar;
	p['c'] = character;
	p['%'] = percent;
	p['f'] = floot;
	p['F'] = floot;
	p['a'] = floot;
	p['A'] = floot;
	p['}'] = non;
	return (zs(tree, 1));
}

void			flags(char *c, t_ap *tree)
{
	tree->preast = (*c == '*' && !tree->dot) ? (1) : (tree->preast);
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
	void		(*p[126]) (va_list ap, char *format, t_ap *tree);
	int			i;
	t_ap		*tree;

	if (!(tree = (t_ap*)ft_memalloc(sizeof(t_ap) * 3)))
		return (0);
	i = ass_f(p, tree);
	va_start(ap, format);
	while (format[i] != '\0')//(format[(i - 1)] != '\0' && tree->percent))
		if (format[i] == '%')
		{
			i++;
			while (printf1(format))
				flags((char*)&format[i++], tree);
			tree->c = (char*)&format[i];
			p[(int)format[i]](ap, (char*)&format[i], tree);
			i += (printf2(format)) ? (zs(tree, i)) : ((zs(tree, i) + 1));
		}
		else
			tree->ret += write(1, &format[i++], 1);
	va_end(ap);
	i = tree->ret;
	free(tree);
	return (i);
}
