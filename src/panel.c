#include <stdio.h>

#include <cgui.h>

void cgui_panel__draw(CGuiContext *ctx, CGuiTreeNode *node, void *user_data) {
	CGuiPanel *panel = (CGuiPanel *) node->value;
	CGuiPanelParams *params = (CGuiPanelParams *) user_data;
	cgui_draw_bevel_rect(ctx, params->x, params->y, params->width, params->height, params->background);

	int last_basis = 0;
	for (size_t i = 0; i < node->children_count; i++) {
		CGuiPanelParams child_params = *params;
		child_params.x += params->padding_left;
		child_params.y += params->padding_top;

		CGuiComponentProps *child_props = (CGuiComponentProps *) cgui_node_value__get_component_props(node->children[i], user_data);

		switch (panel->layout_mode) {
			case Horizontal: {
				if (child_props->basis == 0) {
					child_params.width /= (int) node->children_count;
				} else {
					child_params.width = child_props->basis;
				}

				child_params.x += last_basis * i;
				last_basis = child_params.width;
			} break;
			case Vertical: {
				if (child_props->basis == 0) {
					child_params.height /= (int) node->children_count;
				} else {
					child_params.height = child_props->basis;
				}

				child_params.y += last_basis * i;
				last_basis = child_params.height;
			} break;
		}

		child_params.width -= params->padding_left + params->padding_right;
		child_params.height -= params->padding_top + params->padding_bottom;

		cgui_node_value__draw(ctx, node->children[i], &child_params);

		// last_basis = child_props->basis;
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