#include <drawing.h>

void cgui_draw_bevel_rect(int x, int y, int width, int height, CGuiColor background) {
	cgui_draw_rectangle(x, y, width, height, -1, background);
	cgui_draw_line(x, y, x + width, y, 2, CGUI_LIGHTGRAY);
	cgui_draw_line(x, y, x, y + height, 2, CGUI_LIGHTGRAY);
	cgui_draw_line(x, y + height, x + width, y + height, 2, CGUI_DARKGRAY);
	cgui_draw_line(x + width, y, x + width, y + height, 2, CGUI_DARKGRAY);
}