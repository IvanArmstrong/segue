#include <stdio.h>

#include <cgui.h>

void cgui_panel__draw(CGuiContext *ctx, CGuiTreeNode *node, void *user_data) {
	CGuiPanel *panel = (CGuiPanel *) node->value;
	CGuiPanelParams *params = (CGuiPanelParams *) user_data;
	cgui_draw_bevel_rect(ctx, params->x, params->y, params->width, params->height, params->background);

	for (size_t i = 0; i < node->children_count; i++) {
		CGuiPanelParams child_params = *params;
		child_params.x += params->padding_left;
		child_params.y += params->padding_top;

		switch (panel->layout_mode) {
			case Horizontal: {
				child_params.width /= (int) node->children_count;
				child_params.x += child_params.width * i;
			} break;
			case Vertical: {
				child_params.height /= (int) node->children_count;
				child_params.y += child_params.height * i;
			} break;
		}

		child_params.width -= params->padding_left + params->padding_right;
		child_params.height -= params->padding_top + params->padding_bottom;

		cgui_node_value__draw(ctx, node->children[i], &child_params);
	}
}

CGuiPanel cgui_panel__create() {
	static CGuiNodeValueVTable vtable = {
		.draw = cgui_panel__draw,
	};
	CGuiPanel panel = {
		.vptr = &vtable,
		.layout_mode = Vertical,
	};
	return panel;
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