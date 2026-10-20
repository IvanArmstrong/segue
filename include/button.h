#ifndef CGUI_BUTTON_H
#define CGUI_BUTTON_H

#include <tree.h>
#include <graphics.h>

typedef void (*button_cb_p)(void *);

typedef struct {
	node_vptr	vptr;
	
	button_cb_p	callback;
	bool		pressed;

	CGuiColor	highlight_color;
	CGuiColor	press_color;
	CGuiColor	normal_color;
} CGuiButton;

void		cgui_button__draw(TreeNode *node, void *user_data);
CGuiButton	cgui_button__create(button_cb_p callback);

#endif // CGUI_BUTTON_H