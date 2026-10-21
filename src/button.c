#include <cgui.h>

void cgui_button__draw(CGuiContext *ctx, CGuiTreeNode *node, void *user_data) {
	CGuiButton *button = (CGuiButton *) node->value;
	CGuiPanelParams *params = (CGuiPanelParams *) user_data;

	CGuiColor background = button->normal_color;
	CGuiIVec mouse_pos = ctx->mouse_pos();
	if (cgui_check_collision_point_rec(mouse_pos.x, mouse_pos.y, params->x, params->y, params->width, params->height)) {
		if (ctx->is_mouse_down(CGUI_MOUSE_LEFT)) {
			button->pressed = true;
			background = button->press_color;
		} else {
			background = button->highlight_color;
		}
	}

	if (!ctx->is_mouse_down(CGUI_MOUSE_LEFT)) {
		if (button->pressed) {
			button->callback(NULL);
		}
		button->pressed = false;
	}

	cgui_draw_bevel_rect(ctx, params->x, params->y, params->width, params->height, background);
}

CGuiButton cgui_button__create(button_cb_p callback) {
	static CGuiNodeValueVTable vtable = {
		.draw = cgui_button__draw,
	};
	CGuiButton button = {
		.vptr = &vtable,
		.callback = callback,
		.pressed = false,
		.highlight_color = CGUI_LIGHTGRAY,
		.press_color = CGUI_DARKGRAY,
		.normal_color = CGUI_GRAY,
	};
	return button;
}
