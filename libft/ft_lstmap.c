/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 08:42:36 by ksevciko          #+#    #+#             */
/*   Updated: 2024/09/22 21:16:22 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	*allocate_next_node(t_list **temp,
	void (*del)(void *), t_list *result)
{
	(*temp)->next = NULL;
	if ((*temp)->content == NULL)
	{
		ft_lstclear(&result, del);
		return (NULL);
	}
	(*temp)->next = (t_list *)malloc(1 * sizeof(t_list));
	if (!((*temp)->next))
	{
		ft_lstclear(&result, del);
		return (NULL);
	}
	(*temp) = (*temp)->next;
	return (*temp);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*result;
	t_list	*temp;

	result = (t_list *)malloc(1 * sizeof(t_list));
	if (!result || !f || !lst || !del)
	{
		free(result);
		return (NULL);
	}
	temp = result;
	while (lst->next)
	{
		temp->content = f(lst->content);
		if (allocate_next_node(&temp, del, result) == NULL)
			return (NULL);
		lst = lst->next;
	}
	temp->content = f(lst->content);
	temp->next = NULL;
	if (temp->content == NULL)
	{
		ft_lstclear(&result, del);
		return (NULL);
	}
	return (result);
}
