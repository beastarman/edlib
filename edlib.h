#define HIGHER 1
#define EQUAL 0
#define LOWER -1

#define RIGHT_ROTATION 1
#define LEFT_ROTATION 0

/*****Structs****/

	typedef int (func)(void* arg1, void* arg2);
	
	struct NODO
	{
		struct NODO* next;
		struct NODO* prev;
		struct NODO* left_son;
		struct NODO* right_son;
		struct NODO* dad;
		void* item;
	}
	
	typedef NODO;
	
	struct LIST
	{
		struct NODO* head;
		struct NODO* tail;
		struct NODO* root;
		int lenght;
		int item_size;
		func* compare_func;
	}
	
	typedef LIST;
	
	struct T_NODO
	{
		void* item;
		struct T_NODO* dad;
		LIST* sons;
	}
	
	typedef T_NODO;
	
	struct TREE
	{
		int item_size;
		struct T_NODO* root;
		int lenght;
		func* compare_func;
		int max_sons;
	}
	
	typedef TREE;

/**********************/



/*****Misc headers*****/

	int pass_content(void* source, void* target, int size);
	
	int add_nodo_to_tree(NODO* nodo, T_NODO* t_nodo, TREE* tree);
	
	TREE* extract_binary_tree_list(LIST* list, int limit_sons);

/**********************/



/*****List headers*****/

	int list_height_nodo(NODO* root);
	
	LIST* list_init(int size, func* function);
	
	int list_insert(void* subject, NODO* previous, LIST* list);
	
	int list_delete(NODO* subject, LIST* list);
	
	int list_set(void* new_value, NODO* subject, LIST* list);
	
	NODO* list_get_nodo(int pos, LIST* list);
	
	NODO* list_locate(void* input, LIST* list);
	
	void* list_get_item_nodo(NODO* nodo);
	
	int list_lenght(LIST* list);
	
	int list_double_rotation(NODO* pivot, int direction, LIST* list);
	
	int list_rotation_test(NODO* subject, LIST* list);
	
	int list_rotation_test(NODO* subject, LIST* list);

/***********************/



/*****Queue headers*****/

	typedef LIST QUEUE;
	
	QUEUE* queue_init();
	
	int queue_push(int* x, QUEUE* descritor);
	
	int queue_pop(QUEUE* descritor);
	
	void* queue_top(QUEUE* queue);

/***********************/



/*****Stack headers*****/

	typedef LIST STACK;
	
	STACK* stack_init();
	
	int stack_push(int* x, STACK* descritor);
	
	int stack_pop(STACK* descritor);
	
	void* stack_top(STACK* stack);

/**********************/



/*****Tree headers*****/

	int tree_compare_t_nodo(void* nodo1, void* nodo2);
	
	T_NODO* tree_get_fixed_son(T_NODO* dad, int son_num);
	
	int tree_height(T_NODO* root);
	
	T_NODO* tree_get_son(T_NODO* root, ...);
	
	void* tree_get_item_t_nodo(T_NODO* nodo);
	
	int tree_num_of_sons_t_nodo(T_NODO* nodo);
	
	TREE* tree_init(int size, func* compare, int max_sons);
	
	int tree_insert(void* item, T_NODO* dad, int pos, TREE* tree);
	
	int tree_delete(T_NODO* nodo, TREE* tree);
	
	T_NODO* tree_locate(void* search, T_NODO* root, TREE* tree);

/**********************/