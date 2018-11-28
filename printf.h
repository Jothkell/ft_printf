/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkellehe <jkellehe@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/23 16:24:30 by jkellehe          #+#    #+#             */
/*   Updated: 2018/11/27 18:54:47 by jkellehe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINTF_H
# define PRINTF_H

# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include "libft/libft.h"
# include <locale.h>
# include <limits.h>

# define prec2(format) (format[-1] == 'h' || format[-1] == 'l' || format[-1] == 'L')
# define prec1(format) ((IS_TYPE(format[-1]) && format[-1] != '%') || format[-1] == '.' || format[-1] == '*' || prec2(format))
# define udigit1(format) ((is_unsign(format) && format[-1] == 'l') || format[0] == 'U' || format[0] == 'D')
# define plus(x) ((!O(x)) && (x->c[0] != 'u') && (!X(x)) && x->c[0] != 'p') 
# define thicc(x) (*x == 'S' || *x == 'C' || (x[-1] == 'l' && (*x == 's' || *x == 'c')))
# define baseTEN(x) (x == 'd' || x == 'D' || x == 'u' || x == 'U' || x == 'i')
# define ExOr(x, y) ((x && !y) || (!x && y))
# define INPUTS(x) (IS_TYPE(x) || isDIGIT(x) || isID(x) || isFLAG(x))
# define IS_UPPER(x) (x >= 'A' && x <= 'Z')
# define IS_LOW(x) (x >= 'a' && x <= 'z')
# define IS_TYPE(x) (x == 'R' || x == 'p' || x == 's' || x == 'S' || x == 'p' || x == 'd' || x == 'D' || x == 'i' || x == 'o' || x == 'O' || x == 'u' || x == 'U' || x == 'x' || x == 'X' || x == 'c' || x == 'C' || x == 'b' || x == 'f' || x == 'F' || x == 'a' || x == 'A' || x == '%')
# define NUMBERS(x) (*x == 'd' || *x == 'D' || *x == 'x' || *x == 'X' || *x == 'b' || *x == 'o' || *x == 'O' || *x == 'u' || *x == 'i' || *x == 'U')
# define FLOATS(x) (*x == 'f' || *x == 'F' || *x =='a' || *x == 'A')
# define isDIGIT(x) (x == '0' || x == '1' || x == '2' || x == '3' || x == '4' || x == '5' || x == '6' || x == '7' || x == '8' || x == '9')
# define isID(x) (x == 'h' || x == 'l' || x == 'j' || x == 'z')
# define HH(x) (x[-1] == 'h' && x[-2] == 'h')
# define LL(x) (x[-1] == 'l' && x[-2] == 'l')
# define is_unsign(x) (*x == 'x' || *x == 'X' || *x == 'o' || *x == 'O' || *x == 'u')
# define isFLAG(x) (x == '#' || x == '-' || x == '+')
# define Ox(tree) (hash(tree) && (!tree->zero || tree->c[0] == 'p') && (tree->c[0] == 'x' || tree->c[0] == 'X' || tree->c[0] == 'p'))
# define O(tree) (tree->c[0] == 'o' || tree->c[0] == 'O')
# define X(tree) (tree->c[0] == 'x' || tree->c[0] == 'X')
# define hash(tree) (tree->hash && (!tree->zero || tree->c[0] == 'p'))
# define SingleSpace(x) ((x->c[0] == 'd' || x->c[0] == 'i') && x->space && !x->neg && !x->left && !x->percent && !x->l && !x->ll && !x->decimal && !x->hash && !x->plus)
# define printf1(format) (!IS_TYPE(format[i]) && format[i + 1] != '}' && format[i] != '}' && format[(i + 1)] != '\0' && format[i] != '\0' && format[(i - 1)] != '\0' && format[i] != 10)
# define printf2(format) (format[i] == '\0' && format[(i -1)] == '%')
# define convzer(tree) ((O(tree) && !tree->hash) || X(tree) || tree->c[0] == 'd' || tree->c[0] == 'i' || tree->c[0] == 'u' || tree->c[0] == 'p')
# define ft_pad1(tree) ((tree->zero && (convzer(tree)) && (tree->prec == 10000)) && tree->dot && !tree->z_pad)
# define ft_pad2(tree) (!(tree->zero && tree->dot && !tree->z_pad) || ft_pad1(tree))
//(!((tree->zero && !O(tree) && (tree->prec == 10000)) && tree->dot && !tree->z_pad))


typedef struct s_ap t_ap;

struct					s_ap
{
	int len;
	int prec;
	int width;
	int ret;
	uint8_t preast;
	uint8_t posast;
	uint8_t left;
	uint8_t X;
	uint8_t O;
	uint8_t percent;
	uint8_t l;
	uint8_t ll;
	uint8_t fd;
	uint8_t decimal;
	uint8_t hash;
	uint8_t zero;
	uint8_t z_pad;
	uint8_t dot;
	uint8_t plus;
	uint8_t neg;
	uint8_t space;
	uint8_t set_ret;
	char *c;
	char car;

};

//width, precision, left align. 
int						count_usize(uintmax_t n);
int						ft_wstrlen(wchar_t *wc);
char            		*ft_pad(char *s, t_ap *tree);
void                	big_digit(va_list ap, char *format, t_ap *tree);
void            		wchar(va_list ap, char *format, t_ap *tree);
int 			        bt_putwstr(wchar_t *s, t_ap *tree);
char					*ft_wpad(wchar_t *s, t_ap *tree);
int						get_wstr_len(wchar_t *wc);
void					put_wchar(char c);
int						put_wc(wchar_t c);
int						bt_putchar(char c, int times);
void					ft_put_wstr_fd_prec(wchar_t *s, int fd, t_ap *tree);
char					*ft_spad(char *s, t_ap *tree);
char					*ft_umaxtoa_base(uintmax_t n, uintmax_t base, char *format, t_ap *tree);
int						bt_putstr_fd(char const *s, t_ap *tree);
void					floot(va_list ap, char *format, t_ap *tree);
char					*ft_ftoa_base(double n, long long base, char *format);
void					ft_putstr_fd_prec(char *s, t_ap *tree);
int						precision(char *format, va_list ap, t_ap *tree);
char            		*ft_maxtoa_base(intmax_t n, intmax_t base, char *format);
int						ass_f(void (**p) (va_list ap, char *format, t_ap *tree), t_ap *tree);
void					digit(va_list ap, char *format, t_ap *tree);
int						ft_printf(const char * restrict format, ...);
void					flags(char *c, t_ap *tree);
intmax_t				decimals(double holder, float base, t_ap *tree);
intmax_t				ft_abs(intmax_t in);

# endif
