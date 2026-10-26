#include <stdio.h>

#include <cgui.h>

void cgui_panel__draw(CGuiContext *ctx, CGuiTreeNode *node, void *user_data) {
	CGuiPanel *panel = (CGuiPanel *) node->value;
	CGuiPanelParams *params = (CGuiPanelParams *) user_data;
	cgui_draw_bevel_rect(ctx, params->x, params->y, params->width, params->height, params->background);

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
		switch (panel->layout_mode) {
			case Horizontal: {
				size = params->width;
				padding = params->padding_left;
			} break;
			case Vertical: {
				size = params->height;
				padding = params->padding_top;
			} break;
		}
		auto_basis = (size - total_manual_basis - (padding * node->children_count)) / auto_comps_len;
	}

	int last_offset;
	switch (panel->layout_mode) {
		case Horizontal: {
			last_offset = params->x;
		} break;
		case Vertical: {
			last_offset = params->y;
		} break;
	}

	int last_basis = 0;
	for (size_t i = 0; i < node->children_count; i++) {
		CGuiPanelParams child_params = *params;
		child_params.x += params->padding_left;
		child_params.y += params->padding_top;

		CGuiComponentProps *child_props = (CGuiComponentProps *) cgui_node_value__get_component_props(node->children[i], user_data);

		if (child_props->flex_mode == FLEX_MODE_AUTO) {
			child_props->basis = auto_basis;
		}

		switch (panel->layout_mode) {
			case Horizontal: {
				child_params.width = child_props->basis;
				child_params.x = last_offset + params->padding_left;
				last_basis = child_params.width;
				last_offset = child_params.x + last_basis;
			} break;
			case Vertical: {
				child_params.height = child_props->basis;
				child_params.y = last_offset + params->padding_top;
				last_basis = child_params.height;
				last_offset = child_params.y + last_basis;
			} break;
		}

		child_params.width -= params->padding_left + params->padding_right;
		child_params.height -= params->padding_top + params->padding_bottom;

		cgui_node_value__draw(ctx, node->children[i], &child_params);
	}
}

CGuiComponentProps *cgui_panel__get_props(CGuiTreeNode *node, void *user_data) {
	(void) user_data;

	CGuiPanel *panel = (CGuiPanel *) node->value;
	return &panel->props;
}

CGuiPanelParams cgui_panel_params__default() {
	return (CGuiPanelParams) {
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