#include <stdio.h>

#include <cgui.h>

CGuiTreeNode *cgui_tree_node__create(void *value) {
	static int next_id = 0;
	CGuiTreeNode *node = malloc(sizeof *node);
	node->id = next_id++;
	node->value = value;
	node->children = NULL;
	node->children_count = 0;
	node->children_cap = 0;

	return node;
}

int cgui_tree_node__add_child(CGuiTreeNode *parent, CGuiTreeNode *child) {
	if (parent->children_count >= parent->children_cap) {
		size_t new_cap = parent->children_cap ? parent->children_cap * 2 : 4;
		CGuiTreeNode **temp = realloc(parent->children, new_cap * sizeof *temp);
		if (!temp)
			return -1;
		parent->children = temp;
		parent->children_cap = new_cap;
	}
	parent->children[parent->children_count++] = child;
	return 0;
}

void cgui_tree_node__free(CGuiTreeNode *node, void (*free_value_cb)(void *)) {
	if (!node)
		return;
	for (size_t i = 0; i < node->children_count; ++i) {
		cgui_tree_node__free(node->children[i], free_value_cb);
	}
	free(node->children);
	if (free_value_cb)
		free_value_cb(node->value);
	free(node);
}

void cgui_tree_node__execute(CGuiTreeNode *node, void (*callback)(CGuiTreeNode *, void *), void *user_data) {
	if (!node)
		return;

	callback(node, user_data);

	for (size_t i = 0; i < node->children_count; i++) {
		cgui_tree_node__execute(node->children[i], callback, user_data);
	}
}

void cgui_tree_node__draw_tree(CGuiContext *ctx, CGuiTreeNode *node, int indent, void *user_data) {
	CGuiPanelParams *params = (CGuiPanelParams *) user_data;

	ctx->draw_rectangle(params->x, params->y, params->width, params->height, -1, CGUI_GRAY);
	ctx->draw_rectangle(params->x, params->y, params->width, params->height, 1, CGUI_DARKGRAY);

	if (!node->value)
		return;

	CGuiNodeValue *value = (CGuiNodeValue *) node->value;
	if (value->expanded) {
		for (size_t i = 0; i < node->children_count; i++) {
			CGuiPanelParams child_params = *params;
			child_params.x += indent * 16;
			cgui_tree_node__draw_tree(ctx, node->children[i], indent + 1, &child_params);
		}
	}
}

void _cgui_node_value__draw(CGuiContext *ctx, CGuiTreeNode *node, void *user_data) {
	CGuiPanelParams params = cgui_panel_params__default();
	cgui_tree_node__draw_tree(ctx, node, 0, &params);
}

bool _cgui_node_value__expanded(CGuiTreeNode *node, void *user_data) {
	if (!node->value)
		return false;
	CGuiNodeValue *value = (CGuiNodeValue *) node->value;
	return value->expanded;
}

CGuiComponentProps *_cgui_node_value__get_component_props(CGuiTreeNode *node, void *user_data) {
	(void) node;
	(void) user_data;
	return NULL;
}

CGuiNodeValue cgui_node_value__create() {
	static CGuiNodeValueVTable vtable = {
		.draw = _cgui_node_value__draw,
		.get_props = _cgui_node_value__get_component_props,
	};
	CGuiNodeValue value = {
		.vptr = &vtable,
		.expanded = true,
	};
	return value;
}

void cgui_node_value__draw(CGuiContext *ctx, CGuiTreeNode *node, void *user_data) {
	if (!node->value)
		return;

	CGuiNodeValue *node_value = (CGuiNodeValue *) node->value;
	node_value->vptr->draw(ctx, node, user_data);
}

CGuiComponentProps *cgui_node_value__get_component_props(CGuiTreeNode *node, void *user_data) {
	if (!node->value)
		return NULL;

	CGuiNodeValue *node_value = (CGuiNodeValue *) node->value;
	return node_value->vptr->get_props(node, user_data);
}