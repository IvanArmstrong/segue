#include "graphics_imp.h"

#include <raylib.h>

Color col_convert(CGuiColor color) {
	return *((Color *) &color);
}

void cgui_draw_line(int x1, int y1, int x2, int y2, int thickness, CGuiColor color) {
	DrawLineEx((Vector2) { (float) x1, (float) y1}, (Vector2) { (float) x2, (float) y2 }, thickness, col_convert(color));
}

void cgui_draw_rectangle(int x, int y, int width, int height, int thickness, CGuiColor color) {
	if (thickness > 0) {
		Rectangle rect = {
			(float) x,
			(float) y,
			(float) width,
			(float) height,
		};
		DrawRectangleLinesEx(rect, thickness, col_convert(color));
	} else {
		DrawRectangle(x, y, width, height, col_convert(color));
	}
}

void cgui_draw_text(const char *text, int x, int y, int font_size, CGuiColor color) {
	DrawText(text, x, y, font_size, col_convert(color));
}

int cgui_measure_text(const char *text, int font_size) {
	return MeasureText(text, font_size);
}

bool cgui_is_mouse_pressed(CGuiMouseButton button) {
	return IsMouseButtonPressed(button);
}

bool cgui_is_mouse_down(CGuiMouseButton button) {
	return IsMouseButtonDown(button);
}

bool cgui_is_mouse_up(CGuiMouseButton button) {
	return IsMouseButtonReleased(button);
}

CGuiIVec cgui_mouse_pos() {
	Vector2 mouse_pos = GetMousePosition();
	return (CGuiIVec) { (int) mouse_pos.x, (int) mouse_pos.y };
}

CGuiContext cgui_create_context() {
	return (CGuiContext) {
		.draw_line			= cgui_draw_line,
		.draw_rectangle		= cgui_draw_rectangle,
		.draw_text			= cgui_draw_text,
		.measure_text		= cgui_measure_text,
		.is_mouse_pressed	= cgui_is_mouse_pressed,
		.is_mouse_down		= cgui_is_mouse_down,
		.is_mouse_up		= cgui_is_mouse_up,
		.mouse_pos			= cgui_mouse_pos,
	};
}
