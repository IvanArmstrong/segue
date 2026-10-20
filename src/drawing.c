#include <drawing.h>

void draw_bevel_rect(int x, int y, int width, int height, Color background) {
	DrawRectangle(x, y, width, height, background);
	Vector2 a = { x, y };
	Vector2 b = { x + width, y };
	Vector2 c = { x, y + height };
	Vector2 d = { x + width, y + height };
	DrawLineEx(a, b, 2, LIGHTGRAY);
	DrawLineEx(a, c, 2, LIGHTGRAY);
	DrawLineEx(c, d, 2, DARKGRAY);
	DrawLineEx(b, d, 2, DARKGRAY);
}