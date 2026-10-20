#include <button.h>

#include <panel.h>
#include <graphics.h>
#include <drawing.h>
#include <utils.h>

void cgui_button__draw(TreeNode *node, void *user_data) {
	CGuiButton *button = (CGuiButton *) node->value;
	CGuiPanelParams *params = (CGuiPanelParams *) user_data;

	CGuiColor background = button->normal_color;
	CGuiIVec mouse_pos = cgui_mouse_pos();
	if (cgui_check_collision_point_rec(mouse_pos.x, mouse_pos.y, params->x, params->y, params->width, params->height)) {
		if (cgui_is_mouse_down(CGUI_MOUSE_LEFT)) {
			button->pressed = true;
			background = button->press_color;
		} else {
			background = button->highlight_color;
		}
	}

	if (!cgui_is_mouse_down(CGUI_MOUSE_LEFT)) {
		if (button->pressed) {
			button->callback(NULL);
		}
		button->pressed = false;
	}

	cgui_draw_bevel_rect(params->x, params->y, params->width, params->height, background);
}

CGuiButton cgui_button__create(button_cb_p callback) {
	static NodeValueVTable vtable = {
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
