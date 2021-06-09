/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonpark <joonpark@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/09 14:59:22 by joonpark          #+#    #+#             */
/*   Updated: 2021/06/09 19:57:22 by joonpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "state_machine.h"
#include "print_header.h"
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>

static int		get_flags(const char *str, va_list ap, ft_opt *opt)
{
	const char	*t;
	int			state;
	int			token;
	int			prev_state;

	t = str;
	state = 0;
	init_opt(opt);
	while (*str)
	{
		prev_state = state;
		token = get_token(*str);
		state = get_state(state, token);
		//printf("[%c, %d] %d -> %d\n", *str, token, prev_state, state);
		if (state == 1)
			write(1, str, 1);
		if (state == 2)
		{
			if (*str == '-')
			{
				opt->hypen = 1;
				opt->padc = ' ';
			}
			else if (*str == '0' && opt->hypen != 1)
				opt->padc = '0';
		}
		if (state == 3)
		{
			opt->width *= 10;
			opt->width += (*str - '0');
		}
		if (state == 4)
		{
			opt->width = va_arg(ap, int);
			if (opt->width < 0 && !opt->hypen)
			{
				opt->hypen = 1;
				opt->padc = ' ';
			}
			if (opt->width < 0)
				opt->width = -opt->width;
		}
		if (state == 5)
		{
			opt->dot = 1;
			opt->length = 0;
		}
		if (state == 6)
		{
			opt->length *= 10;
			opt->length += (*str - '0');
		}
		if (state == 7)
			opt->length = va_arg(ap, int);
		if (state == 8) // 현재 % 에 대한 것들은 다 읽은 거니까 종료해야지
			break ;
		if (state == 10)
			break ;
		if (state == -1)
			return (-1);
		str++;
	}
	return (str - t);
}

static int		ft_helper(const char *str, va_list ap)
{
	int			ret;
	int			cnt;
	ft_opt		opt;

	ret = 0;
	while (*str)
	{
		cnt = get_flags(str, ap, &opt);
		if (ret == -1)
		{
			write(1, "ERROR!\n", 7);
			return (-1);
		}
		str += cnt;
		if (*str == 'c')
		{
			print_c(ap, &opt);
			++str;
		}
		if (*str == 's')
		{
			print_s(ap, &opt);
			++str;
		}
		if (*str == 'd' || *str == 'i')
		{
			print_d(ap, &opt);
			++str;
		}
		if (*str == 'u')
		{
			print_u(ap, &opt);
			++str;
		}
		if (*str == 'x')
		{
			print_x(ap, &opt);
			++str;
		}
		if (*str == 'X')
		{
			print_xx(ap, &opt);
			++str;
		}
		if (*str == 'p')
		{
			print_p(ap, &opt);
			++str;
		}
		if (*str == '%')
		{
			print_percentage(&opt);
			++str;
		}
		ret += opt.tot;
	}
	//printf("RETURN: %d\n", ret);
	return (ret);
}

int		ft_printf(const char *str, ...)
{
	va_list		ap;
	int			res;

	va_start(ap, str);
	res = ft_helper(str, ap);
	va_end(ap);
	return (res);
}

/*
int		main()
{
	char		*str = "%.10d/";

	return (0);
}
*/
