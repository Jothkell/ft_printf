/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mod.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkellehe <jkellehe@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 18:51:31 by jkellehe          #+#    #+#             */
/*   Updated: 2018/09/14 16:11:09 by jkellehe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"

int    bt_putstr_fd(char const *s, int fd, t_ap *tree)
{
	int ret;

	ret = 0;
    if (!s)
        return (0);
	//tree->ret += (tree->hash && !tree->zero) ? (write(1, "0x", 2)) : (0);
    while (*s != '\0')
    {
        write(fd, s, 1);
        s++;
		ret++;
    }
	return (ret);
}

static  int     count_size(intmax_t n)
{
    size_t      i;

    i = 1;
    while (n /= 2)
        i++;
    return (i);
}
int bt_putchar(char c, int times)
{
	int count;

	count = times;
	while (times > 0)
	{
		write(1, &c, 1);
		times--;
	}
	return(count);
}



char *ft_pad(char *s, int prec, t_ap *tree)
{
	int i;
	int hold;
    int widthh;
    int prech;
	int len;

    prech = tree->prec;
    prech = (prech > ft_strlen(s)) ? (prech) : (10000);
	tree->prec = prech;
    widthh = tree->width;
    widthh -= (prech != 10000) ? (prech) : (ft_strlen(s)); //- tree->zero);
	widthh -= (tree->plus && !tree->neg) ? (1) : (0); 
	widthh += (tree->zero && (O(tree) || X(tree))) ? (1) : (0);
	prech = (prech != 10000) ? (prech - ft_strlen(s)) : (prech);
	prech = (tree->car == 'x' || tree->car == 'X') ? (10000) : (prech);
	tree->ret += (SingleSpace(tree) && s[0] != '-') ? (write(1, " ", 1)) : (0);
	tree->ret += (tree->plus && s[0] != '-' && tree->left) ? (write(1, "+", 1)) : (0);
    tree->ret += (Oxl(tree) && IS_LOWER(tree->car)) ? (write(1, "0x", 2)) : (0);
    tree->ret += (Oxl(tree) && !IS_LOWER(tree->car)) ? (write(1, "0X", 2)) : (0);
	tree->ret += (O(tree) && hash(tree) && (tree->left || tree->z_pad)) ? (write(1, "0", 1)) : (0);
	tree->ret += (s[0] == '-' && tree->z_pad) ? (write(1, "-", 1)) : (0);
	s += (s[0] == '-' && tree->z_pad) ? (1) : (0);
	while (prech != 10000 && tree->left && (prech > 0) && NUMBERS(tree->c))
	{
		tree->ret += ((tree->c[0] != 'x' && tree->c[0] != 'X') || tree->z_pad) ? (write(1, "0", 1)) : (write(1, " ", 1));
		prech--;
	}
    tree->ret += (tree->left && Oxr(tree) && IS_LOWER(tree->car)) ? (write(1, "0x", 2)) : (0);
    tree->ret += (tree->left && Oxr(tree) && !IS_LOWER(tree->car)) ? (write(1, "0X", 2)) : (0);
    tree->ret += (tree->left && O(tree) && !tree->left) ? (write(1, "0", 1)) : (0);
    tree->ret += (tree->left && !(tree->zero && tree->dot && !(tree->hash && O(tree)))) ? (bt_putstr_fd(ft_strsub(s, 0, tree->prec), 1, tree)) : (0);
    while (widthh > 0)
    {
        tree->ret += (tree->z_pad && !tree->left) ? (write(1, "0", 1)) : (write(1, " ", 1));
        widthh--;
    }
    while (prech != 10000 && !tree->left && (prech > 0) && NUMBERS(tree->c))
    {
        tree->ret += ((tree->c[0] != 'x' && tree->c[0] != 'X') || tree->z_pad) ? (write(1, "0", 1)) : (write(1, " ", 1));
        prech--;
    }
    tree->ret += (tree->plus && s[0] != '-' && !(tree->left || tree->z_pad)) ? (write(1, "+", 1)) : (0);
	tree->ret += (!tree->left && Oxr(tree) && IS_LOWER(tree->car)) ? (write(1, "0x", 2)) : (0);
    tree->ret += (!tree->left && Oxr(tree) && !IS_LOWER(tree->car)) ? (write(1, "0X", 2)) : (0);
    tree->ret += (O(tree) && hash(tree) && !tree->left) ? (write(1, "0", 1)) : (0);
	tree->ret += (!tree->left && !(tree->zero && tree->dot && !(tree->hash && O(tree)))) ? (bt_putstr_fd(ft_strsub(s, 0, tree->prec), 1, tree)) : (0);
	return (s);
}

char *ft_spad(char *s, int prec, t_ap *tree)
{
    int i;
    int hold;

    int hwidth;
    int hprec;

	hprec = tree->prec;
    hprec -= (hprec > ft_strlen(s)) ? (hprec) : (0);
    hwidth = tree->width;
    hwidth -= (ft_strlen(s) - hprec);
	tree->ret += (tree->left && !(tree->zero && tree->dot)) ? (bt_putstr_fd(ft_strsub(s, 0, tree->prec), 1, tree)) : (0);
    while (hwidth > 0)
    {
        tree->ret += (tree->z_pad && !tree->left) ? (write(1, "0", 1)) : (write(1, " ", 1));
        hwidth--;
    }
    while (hprec != 10000 && (hprec > 0) && NUMBERS(tree->c))
    {
        tree->ret += ((tree->c[0] != 'x' && tree->c[0] != 'X') || tree->z_pad) ? (write(1, "0", 1)) : (write(1, " ", 1));
        hprec--;
    }
    tree->ret += (!tree->left && !(tree->zero && tree->dot)) ? (bt_putstr_fd(ft_strsub(s, 0, tree->prec), 1, tree)) : (0);
    return (s);
}

void	ft_fpad(char *s, t_ap *tree)
{
	tree->width -= (tree->prec == 10000) ? (ft_strlen(s) + 6) : (ft_strlen(s) + tree->prec + 1);
	tree->width = (tree->width < 0) ? (0) : (tree->width);
	tree->width += (tree->prec == 0) ? (1) : (0);
	while(((tree->width) > 0) && !(tree->decimal))
	{
		write(1, " ", 1);
		tree->width--;
		tree->ret++;
	}
    tree->ret += bt_putstr_fd(s, 1, tree);
	tree->prec -= (tree->decimal) ? (ft_strlen(s)) : (0);
    while ((tree->prec > 0) && tree->decimal)
    {
        write(1, "0", 1);
        tree->prec--;
		tree->ret++;
    }
    tree->decimal = 1;
}

void    ft_putstr_fd_prec(char *s, int fd, int prec, t_ap *tree, char c)
{
	int i;

	i = 0;
	if (FLOATS(&c))
		ft_fpad(ft_strsub(s, 0, prec), tree);
	else if (NUMBERS(&c))
		ft_pad(s, prec, tree);
	else
		ft_spad(s, prec, tree);
}

void    ft_put_wstr_fd_prec(wchar_t *s, int fd, int prec, t_ap *tree)
{
    int i;

    i = 0;
    if (FLOATS(tree->c))
        i+=1;//ft_fpad(ft_strsub(s, 0, prec), tree);
    else if (NUMBERS(tree->c))
        i++;//ft_pad(s, prec, tree);
    else
        i++;//ft_spad(s, prec, tree);
}

char            *ft_maxtoa_base(intmax_t n, intmax_t base, char *format)
{
    char            *str;
    int             len;
    intmax_t	    tmp;
	char			*digits;
	char			*hold;

	digits = "0123456789ABCDEF";
	if (IS_LOWER(format[0]))
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

char            *ft_umaxtoa_base(uintmax_t n, uintmax_t base, char *format)
{
    char            *str;
    int             len;
    uintmax_t        tmp;
    char            *digits;
    char            *hold;

    digits = "0123456789ABCDEF";
    if (IS_LOWER(format[0]))
        digits = "0123456789abcdef";
    len = count_size(n);
    tmp = n;
    if (!(str = ft_strnew(len)))
        return (NULL);
	//if(n == 0 && !(str[0] = '\0'))
	//return(str);
    str[--len] = digits[tmp % base];
    while ((tmp = tmp / base))
        str[--len] = digits[(tmp % base)];
    hold = ft_strdup(&str[len]);
    free(str);
    return (hold);
}

char            *ft_ftoa_base(double n, long long base, char *format)
{
    char            *str;
    int             len;
    long long       tmp;
    char            *digits;
    char            *hold;
	int count;

	count = 1;
	n *= 1000000;
    digits = "0123456789ABCDEF";
    if (IS_LOWER(format[0]))
        digits = "0123456789abcdef";
    len = count_size(n);
    tmp = n;
    if (n < 0 && ++len)
        tmp = -n;
    if (!(str = ft_strnew(len + 6)))
        return (NULL);
	len += 6;
    str[--len] = digits[tmp % base];
    while ((tmp = tmp / base) && (count++ >5))
        str[--len] = digits[(tmp % base)];
	str[--len] = '.';
    while ((tmp = tmp / base))
        str[--len] = digits[(tmp % base)];
    if (n < 0)
        str[--len] = '-';
    hold = ft_strdup(&str[len]);
    free(str);
    return (hold);
}

int decimals(double holder, float base, t_ap *tree)
{
	int i = tree->prec;
	int tip = 0;
	float top = 0;

	holder *= (holder < 0) ? (-1) : (1);
	i = (i == 10000) ? (6) : (i);
	tip = (int)holder;
	top = (float)tip;
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

int floot(va_list ap, char *format, t_ap *tree)
{
	double holder;
	float base;
	int temp;//cut out later by placing decimals in next line's call

    if (format[0] == 'f' || format[0] == 'F')//whyd you have to go and make it so complicated
		base = 10;
	else
		base = 16;
    holder = va_arg(ap, double);
	temp = precision(format, ap, tree);
	if((tree->prec == 0) && ((((int)(holder * base) % (int)base) >= ((int)base /2)) ||
							 (((int)(-1 * holder * base) % (int)base) >= ((int)base /2))))
		holder += (holder > 0) ? (1) : (-1);
    ft_putstr_fd_prec(ft_maxtoa_base((int)holder, (int)base, format), 1, temp, tree, tree->c[0]);
	temp = (decimals(holder, base, tree));
	ft_putstr_fd_prec(ft_maxtoa_base((int)temp, (int)base, format), 1, tree->prec, tree, tree->c[0]);
    return(0);
}
