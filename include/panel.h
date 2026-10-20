#ifndef CGUI_PANEL_H
#define CGUI_PANEL_H

#include <stdlib.h>
#include <stdbool.h>

#include <layout.h>
#include <tree.h>
#include <graphics.h>

typedef struct {
	node_vptr		vptr;
	CGuiLayoutMode	layout_mode;
} CGuiPanel;

void		cgui_panel__draw(TreeNode *node, void *user_data);
CGuiPanel	cgui_panel__create();

typedef struct {
	int x;
	int y;
	int width;
	int height;

	int padding_top;
	int padding_bottom;
	int padding_left;
	int padding_right;

	CGuiColor background;
	CGuiColor border;

	bool has_focus;
} CGuiPanelParams;

CGuiPanelParams	cgui_panel_params__default();

#endif // CGUI_PANEL_H