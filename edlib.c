#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "edlib.h"

/*****Misc functions*****/

int pass_content(void* source, void* target, int size)
{
	char* temp1;
	char* temp2;
	int i;
	
	if (!(source && target)) return 0;
	
	temp1=source;
	temp2=target;
	
	for(i=0; i<size; temp2++, temp1++, i++)
	{
		*temp2=*temp1;
	}
	
	return 1;
}

int add_nodo_to_tree(NODO* nodo, T_NODO* t_nodo, TREE* tree)
{
	if (!nodo) return 0;
	
	tree_insert(nodo->item, t_nodo, -1, tree);
	
	add_nodo_to_tree(nodo->left_son, t_nodo->sons->tail->item, tree);
	add_nodo_to_tree(nodo->right_son, t_nodo->sons->tail->item, tree);
	
	return 1;
}

TREE* extract_binary_tree_list(LIST* list, int limit_sons)
{
	TREE* tree;
	
	if (!(list && list->root)) return NULL;
	
	tree=tree_init(list->item_size, list->compare_func, (limit_sons? 2:0));
	
	tree_insert(list->root->item, NULL, 0, tree);
	
	add_nodo_to_tree(list->root->left_son, tree->root, tree);
	
	add_nodo_to_tree(list->root->right_son, tree->root, tree);
	
	return tree;
}

/************************/

/*****List functions*****/
	
LIST* list_init(int size, func* function)
{
	LIST* temp;
	
	temp=malloc(sizeof(LIST));
	
	temp->head=NULL;
	temp->tail=NULL;
	temp->root=NULL;
	temp->lenght=0;
	temp->item_size=size;
	temp->compare_func=function;
	
	return temp;
}

int list_insert(void* subject, NODO* previous, LIST* list)
{
	NODO *cur;
	
	/*****List steps*****/
	{
		cur=malloc(sizeof(NODO));
		
		if (!(list && subject && cur)) return 0;
		
		cur->item=malloc(list->item_size);
		
		if (!cur->item)
		{
			free(cur);
			
			return 0;
		}
		
		pass_content(subject, cur->item, list->item_size);
		
		cur->prev=previous;
		
		if (previous)
		{
			cur->next=previous->next;
			
			previous->next=cur;
		}
		else
		{
			cur->next=list->head;
			
			list->head=cur;
		}
			
		if (cur->next)
		{
			cur->next->prev=cur;
		}
		else
		{
			list->tail=cur;
		}
		
		cur->dad=NULL;
		cur->left_son=NULL;
		cur->right_son=NULL;
		
		list->lenght++;
	}
	
	/*****Tree steps*****/
	{
		cur=list->root;
		
		while (cur)
		{
			if(list->compare_func((previous? previous->next : list->head)->item, cur->item)==LOWER)
			{
				if(cur->left_son)
				{
					cur=cur->left_son;
				}
				else
				{
					cur->left_son=(previous? previous->next : list->head);
					cur->left_son->dad=cur;
					
					break;
				}
			}
			else
			{
				if(cur->right_son)
				{
					cur=cur->right_son;
				}
				else
				{
					cur->right_son=(previous? previous->next : list->head);
					cur->right_son->dad=cur;
					
					break;
				}
			}
		}
		
		if (!cur)
		{
			list->root=list->head;
		}
		
		list_rotation_test((previous? previous->next: list->head), list);
	}
	
	return 1;
}

int list_delete(NODO* subject, LIST* list)
{
	if (!(subject && list)) return 0;

	free(subject->item);

	/*****List Steps*****/
	{
		if(subject->prev)
		{
			subject->prev->next = subject->next;
		}
		else
		{
			list->head = subject->next;
		}
		
		if(subject->next)
		{
			subject->next->prev = subject->prev;
		}
		else
		{
			list->tail = subject->prev;
		}
		
		list->lenght--;
	}
	
	NODO* aux;
	
	/*****Tree Steps*****/
	{
		if(!(subject->right_son || subject->left_son))
		{
			if (subject->dad==NULL)
			{
				list->root=NULL;
			}
			else if(subject->dad->left_son==subject)
			{
				subject->dad->left_son=NULL;
			}
			else
			{
				subject->dad->right_son=NULL;
			}
		}
		else if(list_height_nodo(subject->left_son)>list_height_nodo(subject->right_son))
		{
			aux=subject->left_son;
			
			while(aux->right_son) aux=aux->right_son;
			
			if(subject->dad)
			{
				if(subject->dad->left_son==subject)
				{
					subject->dad->left_son=aux;
				}
				else
				{
					subject->dad->right_son=aux;
				}
			}
			else
			{
				list->root=aux;
			}
			
			if (aux->dad != subject)
			{
				aux->dad->right_son=aux->left_son;
				
				if (aux->left_son) aux->left_son->dad=aux->dad;
				
				subject->left_son->dad=aux;
				
				aux->left_son=subject->left_son;
			}
			
			aux->dad=subject->dad;
			
			aux->right_son=subject->right_son;
			
			if(aux->right_son) aux->right_son->dad=aux;
			
			aux=subject->left_son;
			while(aux->right_son) aux=aux->right_son;
			list_rotation_test(aux, list);
		}
		else
		{
			aux=subject->right_son;
			
			while(aux->left_son) aux=aux->left_son;
			
			if(subject->dad)
			{
				if (subject->dad->right_son==subject)
				{
					subject->dad->right_son=aux;
				}
				else
				{
					subject->dad->left_son=aux;
				}
			}
			else
			{
				list->root=aux;
			}
			
			if (aux->dad != subject)
			{
				aux->dad->left_son=aux->right_son;
				
				if (aux->right_son) aux->right_son->dad=aux->dad;
				
				subject->right_son->dad=aux;
				
				aux->right_son=subject->right_son;
			}
			
			aux->dad=subject->dad;
			
			aux->left_son=subject->left_son;
			
			if(aux->left_son) aux->left_son->dad=aux;
			
			aux=subject->right_son;
			while(aux->left_son) aux=aux->left_son;
			list_rotation_test(aux, list);
		}
	}
	
	free(subject);
	
	return 1;
}

int list_set(void* new_value, NODO* subject, LIST* list)
{
	if(!(list && subject)) return 0;
	
	return pass_content(new_value, subject->item, list->item_size);
}

NODO* list_get_nodo(int pos, LIST* list)
{
	NODO* cur;
	int temp;
	
	if (pos<-1 || pos>=list_lenght(list)) return NULL;
	
	if (pos==-1) return list->tail;
	
	if (pos<list->lenght/2)
	{
		for (cur=list->head, temp=0; temp<pos; cur=cur->next, temp++);
		
		return cur;
	}
	else
	{
		for (cur=list->tail, temp=list->lenght-1; temp>pos; cur=cur->prev, temp--);
		
		return cur;
	}
}

void* list_get_item_nodo(NODO* nodo)
{
	if (!nodo) return NULL;
	
	return nodo->item;
}

NODO* list_locate(void* input, LIST* list)
{
NODO* cur=list->root;

while(cur)
{
	if(list->compare_func(input, cur->item)==EQUAL) break;
	
	if(list->compare_func(input, cur->item)==HIGHER)
	{
		cur=cur->right_son;
	}
	else
	{
		cur=cur->left_son;
	}
}

return cur;
}

int list_height_nodo(NODO* root)
{
	if(!root) return 0;
	
	return list_height_nodo(root->right_son)+list_height_nodo(root->left_son)+1;
}

int list_lenght(LIST* list)
{
	if (!list) return 0;
	
	return list->lenght;
}

int list_simple_rotation(NODO* pivot, int rotate_right, LIST* list)
{
	if(rotate_right)
	{
		if(pivot->dad)
		{
			if(pivot==pivot->dad->left_son)
			{
				pivot->dad->left_son = pivot->left_son;
			}
			else
			{
				pivot->dad->right_son = pivot->left_son;
			}
		}
		else
		{
			list->root=pivot->left_son;
		}
		
		pivot->left_son->dad=pivot->dad;
		
		pivot->dad=pivot->left_son;
		
		pivot->left_son=pivot->dad->right_son;
		
		if(pivot->left_son) pivot->left_son->dad=pivot;
		
		pivot->dad->right_son=pivot;
	}
	
	
	else
	
	
	{
		if(pivot->dad)
		{
			if(pivot==pivot->dad->left_son)
			{
				pivot->dad->left_son = pivot->right_son;
			}
			else
			{
				pivot->dad->right_son = pivot->right_son;
			}
		}
		else
		{
			list->root=pivot->right_son;
		}
		
		pivot->right_son->dad=pivot->dad;
		
		pivot->dad=pivot->right_son;
		
		pivot->right_son=pivot->dad->left_son;
		
		if(pivot->right_son) pivot->right_son->dad=pivot;
		
		pivot->dad->left_son=pivot;
	}
	
	return 0;
}

int list_double_rotation(NODO* pivot, int direction, LIST* list)
{
	list_simple_rotation((direction==RIGHT_ROTATION? pivot->left_son: pivot->right_son), !direction, list);
	list_simple_rotation(pivot, direction, list);
		
	return 1;	
}

int list_rotation_test(NODO* subject, LIST* list)
{
	while (subject)
	{
		if (list_height_nodo(subject->left_son)>list_height_nodo(subject->right_son))
		{
			if (list_height_nodo(subject->left_son->left_son)>list_height_nodo(subject->left_son->right_son))
			{
				list_simple_rotation(subject, RIGHT_ROTATION, list);
				
				return 1;
			}
			
			if (list_height_nodo(subject->left_son->right_son)>list_height_nodo(subject->left_son->left_son))
			{
				list_double_rotation(subject, RIGHT_ROTATION, list);
				
				return 1;
			}
		}
		if (list_height_nodo(subject->right_son)>list_height_nodo(subject->left_son))
		{
			if (list_height_nodo(subject->right_son->right_son)>list_height_nodo(subject->right_son->left_son))
			{
				list_simple_rotation(subject, LEFT_ROTATION, list);
				
				return 1;
			}
			
			if (list_height_nodo(subject->right_son->left_son)>list_height_nodo(subject->right_son->right_son))
			{
				list_double_rotation(subject, LEFT_ROTATION, list);
				
				return 1;
			}
		}
		
		subject=subject->dad;
	}
	
	return 0;
}
	
/***********************/



/*****Queue functions*****/

int queue_push(int* x, QUEUE* queue)
{
	if (queue) return list_insert(x ,NULL, (LIST*)queue);
	
	return 0;
}

int queue_pop(QUEUE* queue)
{
	if (queue)
	{
		return list_delete(queue->tail, queue);
	}
	
	return 0;
}

void* queue_top(QUEUE* queue)
{
	return (queue && queue->tail? queue->tail->item: NULL);
}

QUEUE* queue_init(int size, func* function)
{
	return (QUEUE*)list_init(size, function);
}

/*************************/



/*****Stack functions*****/

int stack_push(int* x, STACK* stack)
{
	if (stack) return list_insert(x, stack->tail, (LIST*)stack);
	
	return 0;
}

int stack_pop(STACK* stack)
{
	if (stack)
	{
		return list_delete(stack->tail, stack);
	}
	
	return 0;
}

void* stack_top(STACK* stack)
{
	return (stack && stack->tail? stack->tail->item: NULL);
}

STACK* stack_init(int size, func* function)
{
	return (STACK*)list_init(size, function);
}

/*************************/



/*****Tree functions*****/

int tree_compare_t_nodo(void* nodo1, void* nodo2)
{
	if(nodo1==nodo2) return EQUAL;
	if(nodo1>nodo2) return HIGHER;
	return LOWER;
}

T_NODO* tree_get_fixed_son(T_NODO* dad, int son_num)
{
	if(!(dad && son_num>=0)) return NULL;
	
	if (dad->sons->lenght>son_num)
	{
		return list_get_nodo(son_num, dad->sons)->item;
	}
	
	return NULL;
}

int tree_height(T_NODO* root)
{
	int num=0;
	int son=0;
	int temp=0;
	
	if (!root) return 0;
	
	for(son=0; son<tree_num_of_sons_t_nodo(root); son++)
	{
		if((temp=tree_height(tree_get_fixed_son(root, son)))>num) num=temp;
	}
	
	return num+1;
}

T_NODO* tree_get_son(T_NODO* root, ...)
{
	int limit=tree_height(root)+1;
	int temp;
	va_list sons;
	
	va_start(sons, limit);
	
	while(root)
	{
		temp=va_arg(sons, int);
		
		if(temp==-1) break;
		
		root=tree_get_fixed_son(root, temp);
	}
	
	va_end(sons);
	
	return root;
}

void* tree_get_item_t_nodo(T_NODO* nodo)
{
	if (!nodo) return 0;
	
	return nodo->item;
}

int tree_num_of_sons_t_nodo(T_NODO* nodo)
{
	if (!nodo) return 0;
	
	return nodo->sons->lenght;
}

TREE* tree_init(int size, func* compare, int max_sons)
{
	TREE* tree;
	
	tree=malloc(sizeof(TREE));
	
	tree->item_size=size;
	tree->root=NULL;
	tree->lenght=0;
	tree->compare_func=compare;
	tree->max_sons=max_sons;
	
	return tree;
}

int tree_insert(void* item, T_NODO* dad, int pos, TREE* tree)
{
	T_NODO* nodo;
	
	if (tree->max_sons && tree_num_of_sons_t_nodo(dad)>=tree->max_sons) return 0;
	
	nodo=malloc(sizeof(T_NODO));
	
	if (!nodo) return 0;
	
	nodo->dad=dad;
	nodo->item=malloc(tree->item_size);
	pass_content(item, nodo->item, tree->item_size);
	nodo->sons=list_init(sizeof(T_NODO), tree_compare_t_nodo);
	
	pos--;
	
	if (!dad)
	{
		if (tree->root)
		{
			tree->root->dad=nodo;
			list_insert(tree->root, nodo->sons->tail, nodo->sons);
		}
		
		tree->root=nodo;
	}
	else
	{
		if (pos==-2)
		{
			list_insert(nodo, dad->sons->tail, dad->sons);
		}
		else
		{
			list_insert(nodo, list_get_nodo(pos, dad->sons), dad->sons);
		}
	}
	
	tree->lenght++;
	
	return 1;
}

int tree_delete(T_NODO* nodo, TREE* tree)
{
	int son;
	int num;
	
	if (!(nodo && tree)) return 0;
	
	for(son=tree_num_of_sons_t_nodo(nodo)-1; son>=0; son--)
	{
		tree_delete(tree_get_fixed_son(nodo, son), tree);
	}
	
	if(nodo->item) free(nodo->item);
	if(nodo->sons) free(nodo->sons);
	
	if (nodo->dad)
	{
		for (num=tree_num_of_sons_t_nodo(nodo->dad)-1; num>=0; num--)
		{
			if (nodo==tree_get_fixed_son(nodo->dad, num)) break;
		}
		
		list_delete(list_get_nodo(num, nodo->dad->sons), nodo->dad->sons);
	}
	else
	{
		tree->root=NULL;
	}
	
	tree->lenght--;
	
	return 1;
}

T_NODO* tree_locate(void* search,T_NODO* root, TREE* tree)
{
	int son;
	T_NODO* temp;
	
	if (!(search && root && tree)) return NULL;
	
	if (tree->compare_func(search, root->item)==EQUAL) return root;
	
	for (son=0; son<tree_num_of_sons_t_nodo(root); son++)
	{
		temp=tree_locate(search, tree_get_fixed_son(root, son), tree);
		
		if (temp) return temp;
	}
	
	return NULL;
}

/*************************/