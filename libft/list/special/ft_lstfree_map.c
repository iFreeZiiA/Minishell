/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstfree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 11:17:40 by jjorda            #+#    #+#             */
/*   Updated: 2024/11/11 21:05:17 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

/**
 * Frees all elements of the linked list and their associated memory.
 * 
 * @param head The first element of the linked list.
 * This function terminates the program after freeing the memory.
 */
void	ft_lstfree_map(t_map **head)
{
	t_map	*next;

	if (!head || !*head)
		return ;
	while (*head)
	{
		next = (*head)->next;
		free((*head)->line);
		free(*head);
		*head = next;
	}
	*head = NULL;
}

/* #include <stdio.h>

// Helper function to create a new list node
t_list *create_node(void *content)
{
	t_list *new_node = (t_list *)malloc(sizeof(t_list));
	if (!new_node)
		return NULL;
	new_node->content = content;
	new_node->next = NULL;
	return new_node;
}

// Helper function to print the list (for debugging purposes)
void print_list(t_list *head)
{
	while (head)
	{
		printf("%s -> ", (char *)head->content);
		head = head->next;
	}
	printf("NULL\n");
}

// Test function for ft_lstfree
void test_ft_lstfree()
{
	// Create a sample linked list
	t_list *head = create_node("Node 1");
	head->next = create_node("Node 2");
	head->next->next = create_node("Node 3");

	// Print the list before freeing
	printf("List before freeing:\n");
	print_list(head);

	ft_lstfree(&head);

	// Print the list after freeing (should be NULL)
	printf("List after freeing:\n");
	print_list(head); // This should not print anything as the list is freed
}

int main()
{
	test_ft_lstfree();
	return 0;
} */