#include <stdio.h>

#include <cgui.h>

void cgui_container__draw(CGuiContext *ctx, CGuiTreeNode *node, void *user_data) {
	CGuiContainer *container = (CGuiContainer *) node->value;
	CGuiContainerParams *params = (CGuiContainerParams *) user_data;

	switch (container->style) {
		case CONT_STYLE_INSET: {
			cgui_draw_bevel_rect(ctx, params->x, params->y, params->width, params->height, params->background, CGUI_DARKGRAY, CGUI_LIGHTGRAY);
		} break;
		case CONT_STYLE_RAISED: {
			cgui_draw_bevel_rect(ctx, params->x, params->y, params->width, params->height, params->background, CGUI_LIGHTGRAY, CGUI_DARKGRAY);
		} break;
		case CONT_STYLE_RECT: {
			ctx->draw_rectangle(params->x, params->y, params->width, params->height, 0, params->background);
			ctx->draw_rectangle(params->x, params->y, params->width, params->height, 1, params->border);
		} break;
	}

	if (node->children_count == 0)
		return;

	if (container->layout_mode == MODE_COORD) {
		for (size_t i = 0; i < node->children_count; i++) {
			CGuiComponentProps *child_props = cgui_node_value__get_component_props(node->children[i], user_data);
			CGuiContainerParams child_params = { 0 };
			child_params.x = child_props->x + params->x;
			child_params.y = child_props->y + params->y;
			child_params.width = child_props->width;
			child_params.height = child_props->height;

			if (child_params.x < params->x || 
				child_params.x + child_params.width >= params->x + params->width || 
				child_params.y < params->y ||
				child_params.y + child_params.height > params->y + params->height)
				continue;
			cgui_node_value__draw(ctx, node->children[i], &child_params);
		}
		return;
	}

	int auto_comps_len = 0;
	int total_manual_basis = 0;

	// Find any in auto mode, set their basis based on everyone else's basis
	for (size_t i = 0; i < node->children_count; i++) {
		CGuiComponentProps *child_props = cgui_node_value__get_component_props(node->children[i], user_data);
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
				padding = container->props.padding_left;
			} break;
			case MODE_FLEX_VERT: {
				size = params->height;
				padding = container->props.padding_top;
			} break;
		}
		auto_basis = (size - total_manual_basis - (padding * node->children_count)) / auto_comps_len;
	}

	// Calculate overflow
	int overflow = ((auto_basis * auto_comps_len) + total_manual_basis) - params->width;

	// Scroll wheel offset
	CGuiIVec mouse_pos = ctx->mouse_pos();
	if (mouse_pos.x >= params->x && 
		mouse_pos.x < params->x + params->width &&
		mouse_pos.y >= params->y &&
		mouse_pos.y < params->y + params->height) {
		container->overflow_scroll += ctx->mouse_wheel();
		container->overflow_scroll = max(0, min(overflow, container->overflow_scroll));
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
		child_params.x += container->props.padding_left;
		child_params.y += container->props.padding_top;

		CGuiComponentProps *child_props = cgui_node_value__get_component_props(node->children[i], user_data);

		if (child_props->flex_mode == FLEX_MODE_AUTO) {
			child_props->basis = auto_basis;
		}

		int mod = container->reversed ? -1 : 1;
		switch (container->layout_mode) {
			case MODE_FLEX_HORIZ: {
				child_params.width = child_props->basis;
				child_params.x = last_offset + container->props.padding_left;
				if (container->reversed && i == 0)
					child_params.x -= child_props->basis;
				
				last_basis = child_params.width;
				last_offset = child_params.x + last_basis * mod;
			} break;
			case MODE_FLEX_VERT: {
				child_params.height = child_props->basis;
				child_params.y = last_offset + container->props.padding_top;
				if (container->reversed && i == 0)
					child_params.y -= child_props->basis;
				
				last_basis = child_params.height;
				last_offset = child_params.y + last_basis * mod;
			} break;
		}

		child_params.width -= container->props.padding_left + container->props.padding_right;
		child_params.height -= container->props.padding_top + container->props.padding_bottom;

		if (child_params.x < params->x || child_params.x + child_params.width > params->x + params->width ||
			child_params.y < params->y || child_params.y + child_params.height > params->y + params->height)
			continue;

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
		.has_focus			= false,

		.background			= CGUI_GRAY,
		.border				= CGUI_BLACK,
	};
}