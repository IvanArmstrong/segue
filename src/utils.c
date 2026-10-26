#include <math.h>

#include <cgui.h>

CGuiText cgui_create_text_object(const char *text) {
	return (CGuiText) {
		.text			= text,
		.font_size		= 16,
		.horiz_align	= ALIGN_HORIZ_CENTER,
		.vert_align		= ALIGN_VERT_CENTER,
		.font_color		= CGUI_BLACK,
	};
}

CGuiComponentProps cgui_create_component_props() {
	return (CGuiComponentProps) {
		.basis		= 0,
		.flex_mode	= FLEX_MODE_MANUAL,
	};
}

bool cgui_check_collision_point_rec(int px, int py, int rx, int ry, int rwidth, int rheight) {
	return px >= rx && py >= ry && px < rx + rwidth && py < ry + rheight;
}

bool cgui_check_collision_point_circle(int px, int py, int cx, int cy, int radius) {
	return sqrt(pow((float) cx - (float) px, 2) + pow((float) cy - (float) py, 2));
}
