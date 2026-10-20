#include <button.h>

#include <raylib.h> 

#include <panel.h>
#include <drawing.h>

void cgui_button__draw(TreeNode *node, void *user_data) {
	CGuiButton *button = (CGuiButton *) node->value;
	CGuiPanelParams *params = (CGuiPanelParams *) user_data;

	Color background = button->normal_color;
	Vector2 mouse_pos = GetMousePosition();
	Rectangle rect = {
		(float) params->x,
		(float) params->y,
		(float) params->width,
		(float) params->height,
	};
	if (CheckCollisionPointRec(mouse_pos, rect)) {
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			background = button->press_color;
		} else {
			background = button->highlight_color;
		}
	}

	draw_bevel_rect(params->x, params->y, params->width, params->height, background);
}

CGuiButton cgui_button__create(button_cb_p callback) {
	static NodeValueVTable vtable = {
		.draw = cgui_button__draw,
	};
	CGuiButton button = {
		.vptr = &vtable,
		.callback = callback,
		.highlight_color = LIGHTGRAY,
		.press_color = DARKGRAY,
		.normal_color = GRAY,
	};
	return button;
}
