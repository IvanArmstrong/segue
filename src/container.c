#include <stdio.h>

#include <cgui.h>

void cgui_container__draw(CGuiContext *ctx, CGuiTreeNode *node, void *user_data) {
	CGuiContainer *container = (CGuiContainer *) node->value;
	CGuiContainerParams *params = (CGuiContainerParams *) user_data;
	cgui_draw_bevel_rect(ctx, params->x, params->y, params->width, params->height, params->background);

	if (node->children_count == 0)
		return;

	if (container->layout_mode == MODE_COORD) {
		for (size_t i = 0; i < node->children_count; i++) {
			CGuiComponentProps *child_props = (CGuiComponentProps *) cgui_node_value__get_component_props(node->children[i], user_data);
			CGuiContainerParams child_params = { 0 };
			child_params.x = child_props->x + params->x;
			child_params.y = child_props->y + params->y;
			child_params.width = child_props->width;
			child_params.height = child_props->height;
			cgui_node_value__draw(ctx, node->children[i], &child_params);
		}
		return;
	}

	int auto_comps_len = 0;
	int total_manual_basis = 0;

	// Find any in auto mode, set their basis based on everyone else's basis
	for (size_t i = 0; i < node->children_count; i++) {
		CGuiComponentProps *child_props = (CGuiComponentProps *) cgui_node_value__get_component_props(node->children[i], user_data);
		if (child_props->flex_mode == FLEX_MODE_AUTO) {
			auto_comps_len++;
		} else {
			total_manual_basis += child_props->basis;
		}
	}

	// Calculate automatic basis
	int auto_basis = 0;
	if (auto_comps_len) {
		int size;
		int padding;
		switch (container->layout_mode) {
			case MODE_FLEX_HORIZ: {
				size = params->width;
				padding = params->padding_left;
			} break;
			case MODE_FLEX_VERT: {
				size = params->height;
				padding = params->padding_top;
			} break;
		}
		auto_basis = (size - total_manual_basis - (padding * node->children_count)) / auto_comps_len;
	}

	int last_offset;
	switch (container->layout_mode) {
		case MODE_FLEX_HORIZ: {
			last_offset = params->x;
			if (container->reversed)
				last_offset += params->width;
		} break;
		case MODE_FLEX_VERT: {
			last_offset = params->y;
			if (container->reversed)
				last_offset += params->height;
		} break;
	}

	int last_basis = 0;
	for (size_t i = 0; i < node->children_count; i++) {
		CGuiContainerParams child_params = *params;
		child_params.x += params->padding_left;
		child_params.y += params->padding_top;

		CGuiComponentProps *child_props = (CGuiComponentProps *) cgui_node_value__get_component_props(node->children[i], user_data);

		if (child_props->flex_mode == FLEX_MODE_AUTO) {
			child_props->basis = auto_basis;
		}

		int mod = container->reversed ? -1 : 1;
		switch (container->layout_mode) {
			case MODE_FLEX_HORIZ: {
				child_params.width = child_props->basis;
				child_params.x = last_offset + params->padding_left;
				if (container->reversed && i == 0)
					child_params.x -= child_props->basis;
				
				last_basis = child_params.width;
				last_offset = child_params.x + last_basis * mod;
			} break;
			case MODE_FLEX_VERT: {
				child_params.height = child_props->basis;
				child_params.y = last_offset + params->padding_top;
				if (container->reversed && i == 0)
					child_params.y -= child_props->basis;
				
				last_basis = child_params.height;
				last_offset = child_params.y + last_basis * mod;
			} break;
		}

		child_params.width -= params->padding_left + params->padding_right;
		child_params.height -= params->padding_top + params->padding_bottom;

		cgui_node_value__draw(ctx, node->children[i], &child_params);
	}
}

CGuiComponentProps *cgui_container__get_props(CGuiTreeNode *node, void *user_data) {
	(void) user_data;

	CGuiContainer *container = (CGuiContainer *) node->value;
	return &container->props;
}

CGuiContainerParams cgui_container_params__default() {
	return (CGuiContainerParams) {
		.x					= 0,
		.y					= 0,
		.width				= 100,
		.height				= 100,
		.padding_top		= 5,
		.padding_bottom		= 5,
		.padding_left		= 5,
		.padding_right		= 5,
		.has_focus			= false,

		.background			= CGUI_GRAY,
		.border				= CGUI_LIGHTGRAY,
	};
}