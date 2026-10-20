#ifndef CGUI_TREE_H
#define CGUI_TREE_H

#include <stdlib.h>

typedef struct TreeNode {
	int				id;
	void			*value;
	struct TreeNode	**children;
	size_t			children_count;
	size_t			children_cap;
} TreeNode;

TreeNode	*tree_node__create(void *value);
int			tree_node__add_child(TreeNode *parent, TreeNode *child);
void		tree_node__free(TreeNode *node, void (*free_value_cb)(void *));
void		tree_node__execute(TreeNode *node, void (*callback)(TreeNode *, void *), void *user_data);

typedef struct {
	void	(*draw)(TreeNode *, void *);
} NodeValueVTable;

typedef NodeValueVTable *node_vptr;

typedef struct {
	node_vptr vptr;
} NodeValue;

void		_node_value__draw(TreeNode *node, void *user_data);
NodeValue	node_value__create();
void		node_value__draw(TreeNode *node, void *user_data);

#endif // CGUI_TREE_H