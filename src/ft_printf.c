/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonpark <joonpark@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/09 14:59:22 by joonpark          #+#    #+#             */
/*   Updated: 2021/06/09 15:06:00 by joonpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "state_machine.h"
#include <stdio.h>

int		main()
{
  char		*str = "%.10d/";
  int		state;
  int		prev_state;
  int		token;

  state = 0;
  while (*str)
  {
      prev_state = state;
      token = get_token(*str);
      state = get_state(state, token);
      printf("[%c, %d] %d -> %d\n", *str, token, prev_state, state);
      if (state == 8)
        state = 0;
      if (state == 10)
        break ;
      if (state == -1)
        return (-1);
      str++;
    }
  return (0);
}
