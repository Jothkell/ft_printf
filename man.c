/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   man.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkellehe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/22 14:35:37 by jkellehe          #+#    #+#             */
/*   Updated: 2018/11/29 13:47:47 by jkellehe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"

int main()
{
	int ret = 0;
    int ret2 = 0;
	//char c = 46;

    setlocale(LC_ALL, "");
	printf("mine\n");
	ret = ft_printf("%.0f", -0.02); 
	printf("\ntheirs\n");
	ret2 = printf("%.0f", -0.02);
	printf("\n");
	printf("myret: %d\ntheyret: %d\n", ret, ret2);
	//while(1);
    return(0);
	}
