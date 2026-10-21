#include <cgui.h>

void cgui_draw_bevel_rect(CGuiContext *ctx, int x, int y, int width, int height, CGuiColor background) {
	ctx->draw_rectangle(x, y, width, height, -1, background);
	ctx->draw_line(x, y, x + width, y, 2, CGUI_LIGHTGRAY);
	ctx->draw_line(x, y, x, y + height, 2, CGUI_LIGHTGRAY);
	ctx->draw_line(x, y + height, x + width, y + height, 2, CGUI_DARKGRAY);
	ctx->draw_line(x + width, y, x + width, y + height, 2, CGUI_DARKGRAY);
}