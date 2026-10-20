#ifndef CGUI_GRAPHICS_H
#define CGUI_GRAPHICS_H

// For external implementation

#include <stdlib.h>
#include <stdbool.h>

typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} CGuiColor;

static const CGuiColor CGUI_LIGHTGRAY = { 200, 200, 200, 255 };
static const CGuiColor CGUI_GRAY = { 155, 155, 155, 255 };
static const CGuiColor CGUI_DARKGRAY = { 100, 100, 100, 255 };

typedef struct {
	int x;
	int y;
} CGuiIVec;

typedef enum {
	CGUI_MOUSE_LEFT,
	CGUI_MOUSE_RIGHT,
	CGUI_MOUSE_MIDDLE,
} CGuiMouseButton;

void		cgui_draw_line(int x1, int y1, int x2, int y2, int thickness, CGuiColor color);
void		cgui_draw_rectangle(int x, int y, int width, int height, int thickness, CGuiColor color);
bool		cgui_is_mouse_pressed(CGuiMouseButton button);
bool		cgui_is_mouse_down(CGuiMouseButton button);
bool		cgui_is_mouse_up(CGuiMouseButton button);
CGuiIVec	cgui_mouse_pos();

#endif // CGUI_GRAPHICS_H