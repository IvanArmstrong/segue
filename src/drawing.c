#include <cgui.h>

void cgui_draw_styled_text(CGuiContext *ctx, CGuiText *text_object, CGuiPanelParams *params) {
	if (!text_object->text)
		return;

	int text_x = params->x;
	int text_y = params->y;

	switch (text_object->horiz_align) {
		case ALIGN_HORIZ_CENTER: {
			int text_width = ctx->measure_text(text_object->text, text_object->font_size);
			text_x = params->x + ((params->width / 2) - (text_width / 2));
		} break;
		case ALIGN_HORIZ_RIGHT: {
			int text_width = ctx->measure_text(text_object->text, text_object->font_size);
			text_x = params->x + params->width - text_width;
		} break;
		default: break;
	}

	switch (text_object->vert_align) {
		case ALIGN_VERT_CENTER: {
			text_y = params->y + ((params->height / 2) - (text_object->font_size / 2));
		} break;
		case ALIGN_VERT_BOTTOM: {
			text_y = params->y + params->height - text_object->font_size;
		} break;
		default: break;
	}

	ctx->draw_text(text_object->text, text_x, text_y, text_object->font_size, CGUI_BLACK);
}

void cgui_draw_bevel_rect(CGuiContext *ctx, int x, int y, int width, int height, CGuiColor background) {
	ctx->draw_rectangle(x, y, width, height, -1, background);
	ctx->draw_line(x, y, x + width, y, 2, CGUI_LIGHTGRAY);
	ctx->draw_line(x, y, x, y + height, 2, CGUI_LIGHTGRAY);
	ctx->draw_line(x, y + height, x + width, y + height, 2, CGUI_DARKGRAY);
	ctx->draw_line(x + width, y, x + width, y + height, 2, CGUI_DARKGRAY);
}