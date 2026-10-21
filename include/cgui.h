#ifndef CGUI_H
#define CGUI_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifdef _WIN32
#	ifdef CGUI_BUILD_DLL
#		define CGUI_API __declspec(dllexport)
#	else
#		define CGUI_API __declspec(dllimport)
#	endif
#else
#	define CGUI_API __attribute__((visibility("default")))
#endif

#include <stdlib.h>
#include <stdbool.h>

struct CGuiContext;
typedef struct CGuiContext CGuiContext;

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

typedef struct CGuiTreeNode {
	int					id;
	void				*value;
	struct CGuiTreeNode	**children;
	size_t				children_count;
	size_t				children_cap;
} CGuiTreeNode;

typedef struct {
	void	(*draw)(CGuiContext *ctx, CGuiTreeNode *node, void *user_data);
} CGuiNodeValueVTable;

typedef CGuiNodeValueVTable *node_vptr;

typedef struct {
	node_vptr vptr;
} CGuiNodeValue;

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

struct CGuiContext {
	void		(*draw_line)(int x1, int y1, int x2, int y2, int thickness, CGuiColor color);
	void		(*draw_rectangle)(int x, int y, int width, int height, int thickness, CGuiColor color);
	bool		(*is_mouse_pressed)(CGuiMouseButton button);
	bool		(*is_mouse_down)(CGuiMouseButton button);
	bool		(*is_mouse_up)(CGuiMouseButton button);
	CGuiIVec	(*mouse_pos)();
};

CGUI_API CGuiTreeNode		*cgui_tree_node__create(void *value);
CGUI_API int				cgui_tree_node__add_child(CGuiTreeNode *parent, CGuiTreeNode *child);
CGUI_API void				cgui_tree_node__free(CGuiTreeNode *node, void (*free_value_cb)(void *));
CGUI_API void				cgui_tree_node__execute(CGuiTreeNode *node, void (*callback)(CGuiTreeNode *, void *), void *user_data);

CGUI_API void				_cgui_node_value__draw(CGuiContext *ctx, CGuiTreeNode *node, void *user_data);
CGUI_API CGuiNodeValue		cgui_node_value__create();
CGUI_API void				cgui_node_value__draw(CGuiContext *ctx, CGuiTreeNode *node, void *user_data);

CGUI_API void				cgui_button__draw(CGuiContext *ctx, CGuiTreeNode *node, void *user_data);
CGUI_API CGuiButton			cgui_button__create(button_cb_p callback);

CGUI_API void				cgui_draw_bevel_rect(CGuiContext *ctx, int x, int y, int width, int height, CGuiColor background);

CGUI_API void				cgui_panel__draw(CGuiContext *ctx, CGuiTreeNode *node, void *user_data);
CGUI_API CGuiPanel			cgui_panel__create();

CGUI_API CGuiPanelParams	cgui_panel_params__default();
CGUI_API bool				cgui_check_collision_point_rec(int px, int py, int rx, int ry, int rwidth, int rheight);
CGUI_API bool				cgui_check_collision_point_circle(int px, int py, int cx, int cy, int radius);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif CGUI_H

#ifndef CGUI_STRIP_PREFIX_GUARD_
#define CGUI_STRIP_PREFIX_GUARD_
#	ifdef CGUI_STRIP_PREFIX
#		define Button					CGuiButton
#		define LayoutMode				CGuiLayoutMode
#		define Panel					CGuiPanel
#		define PanelParams				CGuiPanelParams
#		define TreeNode					CGuiTreeNode
#		define NodeValue				CGuiNodeValue

#		define button__draw				cgui_button__draw
#		define button__create			cgui_button__create
#		define draw_bevel_rect			cgui_draw_bevel_rect
#		define panel__draw				cgui_panel__draw
#		define panel__create			cgui_panel__create
#		define panel_params__default	cgui_panel_params__default
#		define tree_node__create		cgui_tree_node__create
#		define tree_node__add_child		cgui_tree_node__add_child
#		define node_value__draw			cgui_node_value__draw
#		define tree_node__free			cgui_tree_node__free
#		define tree_node__free			cgui_tree_node__free
#		define tree_node__execute		cgui_tree_node__execute
#	endif // CGUI_STRIP_PREFIX
#endif // CGUI_STRIP_PREFIX_GUARD_