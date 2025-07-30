/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 17:44:11 by ksevciko          #+#    #+#             */
/*   Updated: 2024/09/20 20:54:09 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*ptr;

	if (!new)
	{
		return ;
	}
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	ptr = *lst;
	while (ptr->next)
	{
		ptr = ptr->next;
	}
	ptr->next = new;
}

// void	ft_lstadd_back(t_list **lst, t_list *new)
// {
// 	t_list	**ptr;
// 	//t_list	*p;

// 	if (!new)
// 	{
// 		return ;
// 	}
// 	ptr = lst;
// 	//p = *ptr;
// 	if (!*ptr)
// 	{
// 		*ptr = new;
// 	}
// 	else
// 	{
// 		while ((**ptr).next)
// 		{
// 			*ptr = (**ptr).next;
// 		}
// 		(**ptr).next = new;	
// 	}
// }

// int	main(void)
// {
// 	t_list	**lst;
// 	int		i;
// 	t_list	*ptr;

// 	ptr = ft_lstnew("first node");
// 	lst = &ptr;
// 	i = 0;
// 	while (i < 6)
// 	{
// 		ft_lstadd_back(&ptr, ft_lstnew("new node at end"));
// 		i++;
// 	}
// 	while (i < 10)
// 	{
// 		ft_lstadd_front(&ptr, ft_lstnew("new node at begining"));
// 		i++;
// 	}
// 	while (i > 0 && ptr)
// 	{
// 		ft_putendl_fd(ptr->content, 1);
// 		ptr = ptr->next;
// 		i--;
// 	}
// 	return (0);
// }
