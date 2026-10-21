#ifndef CGUI_H
#define CGUI_H

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

typedef struct TreeNode {
	int				id;
	void			*value;
	struct TreeNode	**children;
	size_t			children_count;
	size_t			children_cap;
} TreeNode;

typedef struct {
	void	(*draw)(TreeNode *, void *);
} NodeValueVTable;

typedef NodeValueVTable *node_vptr;

typedef struct {
	node_vptr vptr;
} NodeValue;

typedef void (*button_cb_p)(void *);

typedef struct {
	node_vptr	vptr;
	
	button_cb_p	callback;
	bool		pressed;

	CGuiColor	highlight_color;
	CGuiColor	press_color;
	CGuiColor	normal_color;
} CGuiButton;

typedef enum {
	Horizontal,
	Vertical,
} CGuiLayoutMode;

typedef struct {
	node_vptr		vptr;
	CGuiLayoutMode	layout_mode;
} CGuiPanel;

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


void			cgui_draw_line(int x1, int y1, int x2, int y2, int thickness, CGuiColor color);
void			cgui_draw_rectangle(int x, int y, int width, int height, int thickness, CGuiColor color);
bool			cgui_is_mouse_pressed(CGuiMouseButton button);
bool			cgui_is_mouse_down(CGuiMouseButton button);
bool			cgui_is_mouse_up(CGuiMouseButton button);
CGuiIVec		cgui_mouse_pos();

TreeNode		*tree_node__create(void *value);
int				tree_node__add_child(TreeNode *parent, TreeNode *child);
void			tree_node__free(TreeNode *node, void (*free_value_cb)(void *));
void			tree_node__execute(TreeNode *node, void (*callback)(TreeNode *, void *), void *user_data);

void			_node_value__draw(TreeNode *node, void *user_data);
NodeValue		node_value__create();
void			node_value__draw(TreeNode *node, void *user_data);

void			cgui_button__draw(TreeNode *node, void *user_data);
CGuiButton		cgui_button__create(button_cb_p callback);

void			cgui_draw_bevel_rect(int x, int y, int width, int height, CGuiColor background);

void			cgui_panel__draw(TreeNode *node, void *user_data);
CGuiPanel		cgui_panel__create();


CGuiPanelParams	cgui_panel_params__default();
bool			cgui_check_collision_point_rec(int px, int py, int rx, int ry, int rwidth, int rheight);
bool			cgui_check_collision_point_circle(int px, int py, int cx, int cy, int radius);

#endif CGUI_H

#ifndef CGUI_STRIP_PREFIX_GUARD_
#define CGUI_STRIP_PREFIX_GUARD_
#	ifdef CGUI_STRIP_PREFIX
#		define Button					CGuiButton
#		define LayoutMode				CGuiLayoutMode
#		define Panel					CGuiPanel
#		define PanelParams				CGuiPanelParams

#		define button__draw				cgui_button__draw
#		define button__create			cgui_button__create
#		define draw_bevel_rect			cgui_draw_bevel_rect
#		define panel__draw				cgui_panel__draw
#		define panel__create			cgui_panel__create
#		define panel_params__default	cgui_panel_params__default
#	endif // CGUI_STRIP_PREFIX
#endif // CGUI_STRIP_PREFIX_GUARD_