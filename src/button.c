#include <cgui.h>

void cgui_button__draw(CGuiContext *ctx, CGuiTreeNode *node, void *user_data) {
	CGuiButton *button = (CGuiButton *) node->value;
	CGuiContainerParams *params = (CGuiContainerParams *) user_data;

	CGuiColor background = button->normal_color;
	CGuiIVec mouse_pos = ctx->mouse_pos();
	if (cgui_check_collision_point_rec(mouse_pos.x, mouse_pos.y, params->x, params->y, params->width, params->height)) {
		if (ctx->is_mouse_down(CGUI_MOUSE_LEFT)) {
			button->pressed = true;
		} else {
			background = button->highlight_color;
		}
	} else {
		button->pressed = false;
	}

	if (button->callback) {
		if (!ctx->is_mouse_down(CGUI_MOUSE_LEFT)) {
			if (button->pressed) {
				button->callback(ctx, node);
			}
			button->pressed = false;
		}
	}

	if (button->pressed)
		cgui_draw_bevel_rect(ctx, params->x, params->y, params->width, params->height, background, CGUI_DARKGRAY, CGUI_LIGHTGRAY);
	else
		cgui_draw_bevel_rect(ctx, params->x, params->y, params->width, params->height, background, CGUI_LIGHTGRAY, CGUI_DARKGRAY);
	cgui_draw_styled_text(ctx, &button->text_object, params);
}

CGuiComponentProps *cgui_button__get_props(CGuiTreeNode *node, void *user_data) {
	(void) user_data;
	
	CGuiButton *button = (CGuiButton *) node->value;
	return &button->props;
}