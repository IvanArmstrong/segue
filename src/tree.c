#include <stdio.h>

#include <tree.h>

TreeNode *tree_node__create(void *value) {
	static int next_id = 0;
	TreeNode *node = malloc(sizeof *node);
	node->id = next_id++;
	node->value = value;
	node->children = NULL;
	node->children_count = 0;
	node->children_cap = 0;

	return node;
}

int tree_node__add_child(TreeNode *parent, TreeNode *child) {
	if (parent->children_count >= parent->children_cap) {
		size_t new_cap = parent->children_cap ? parent->children_cap * 2 : 4;
		TreeNode **temp = realloc(parent->children, new_cap * sizeof *temp);
		if (!temp)
			return -1;
		parent->children = temp;
		parent->children_cap = new_cap;
	}
	parent->children[parent->children_count++] = child;
	return 0;
}

void tree_node__free(TreeNode *node, void (*free_value_cb)(void *)) {
	if (!node)
		return;
	for (size_t i = 0; i < node->children_count; ++i) {
		tree_node__free(node->children[i], free_value_cb);
	}
	free(node->children);
	if (free_value_cb)
		free_value_cb(node->value);
	free(node);
}

void tree_node__execute(TreeNode *node, void (*callback)(TreeNode *, void *), void *user_data) {
	if (!node)
		return;

	callback(node, user_data);

	for (size_t i = 0; i < node->children_count; i++) {
		tree_node__execute(node->children[i], callback, user_data);
	}
}

void _node_value__draw(TreeNode *node, void *user_data) {
	printf("Drawing Nameless Thing!\n");
}

NodeValue node_value__create() {
	static NodeValueVTable vtable = {
		.draw = _node_value__draw,
	};
	NodeValue value = {
		.vptr = &vtable,
	};
	return value;
}

void node_value__draw(TreeNode *node, void *user_data) {
	if (!node->value)
		return;

	NodeValue *node_value = (NodeValue *) node->value;
	node_value->vptr->draw(node, user_data);
}
