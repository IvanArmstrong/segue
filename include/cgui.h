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

static const CGuiColor CGUI_LIGHTGRAY	= { 200, 200, 200, 255 };
static const CGuiColor CGUI_GRAY		= { 155, 155, 155, 255 };
static const CGuiColor CGUI_DARKGRAY	= { 100, 100, 100, 255 };
static const CGuiColor CGUI_BLACK		= { 0, 0, 0, 255 };

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
	int	basis;
} CGuiComponentProps;

typedef struct {
	void				(*draw)(CGuiContext *ctx, CGuiTreeNode *node, void *user_data);
	bool				(*expanded)(CGuiTreeNode *node, void *user_data);
	CGuiComponentProps	*(*get_props)(CGuiTreeNode *node, void *user_data);
} CGuiNodeValueVTable;

typedef CGuiNodeValueVTable *node_vptr;

typedef struct {
	node_vptr vptr;

	bool expanded;
} CGuiNodeValue;

typedef enum {
	ALIGN_VERT_CENTER,
	ALIGN_VERT_TOP,
	ALIGN_VERT_BOTTOM,
} CGuiVertAlign;

typedef enum {
	ALIGN_HORIZ_CENTER,
	ALIGN_HORIZ_LEFT,
	ALIGN_HORIZ_RIGHT,
} CGuiHorizAlign;

typedef struct {
	const char		*text;
	int				font_size;
	CGuiHorizAlign	horiz_align;
	CGuiVertAlign	vert_align;
	CGuiColor		font_color;
} CGuiText;

typedef struct {
	node_vptr			vptr;

	CGuiComponentProps	props;

	CGuiText			text_object;
	
	void				(*callback)(CGuiContext *ctx, CGuiTreeNode *button);
	void				*user_data;
	bool				pressed;

	CGuiColor			highlight_color;
	CGuiColor			press_color;
	CGuiColor			normal_color;
} CGuiButton;

typedef struct {
	node_vptr			vptr;
	CGuiComponentProps	props;
	CGuiText			text_object;
} CGuiLabel;

typedef enum {
	Horizontal,
	Vertical,
} CGuiLayoutMode;

typedef struct {
	node_vptr			vptr;
	CGuiComponentProps	props;
	CGuiLayoutMode		layout_mode;
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

typedef enum {
	CGUI_COMP_BUTTON,
	CGUI_COMP_PANEL,
	CGUI_COMP_LABEL,
} CGuiComponentKind;

typedef struct {
	CGuiComponentKind kind;
	union {
		CGuiButton	button;
		CGuiPanel	panel;
		CGuiLabel	label;
	} value;
} CGuiComponent;

struct CGuiContext {
	void		(*draw_line)(int x1, int y1, int x2, int y2, int thickness, CGuiColor color);
	void		(*draw_rectangle)(int x, int y, int width, int height, int thickness, CGuiColor color);
	void		(*draw_text)(const char *text, int x, int y, int font_size, CGuiColor color);
	int			(*measure_text)(const char *text, int font_size);
	bool		(*is_mouse_pressed)(CGuiMouseButton button);
	bool		(*is_mouse_down)(CGuiMouseButton button);
	bool		(*is_mouse_up)(CGuiMouseButton button);
	CGuiIVec	(*mouse_pos)();

	// This is kinda lazy and I know there's a better way to do this,
	// but I didn't want to implement a malloc for each freaking component. No way, man.
	// It's union time
	CGuiComponent	**components;
	size_t			components_count;
	size_t			components_cap;
};

void				_cgui_node_value__draw(CGuiContext *ctx, CGuiTreeNode *node, void *user_data);
bool				_cgui_node_value__expanded(CGuiTreeNode *node, void *user_data);
CGuiComponentProps	*_cgui_node_value__get_component_props(CGuiTreeNode *node, void *user_data);

CGUI_API CGuiTreeNode		*cgui_tree_node__create(void *value);
CGUI_API int				cgui_tree_node__add_child(CGuiTreeNode *parent, CGuiTreeNode *child);
CGUI_API void				cgui_tree_node__free(CGuiTreeNode *node, void (*free_value_cb)(void *));
CGUI_API void				cgui_tree_node__execute(CGuiTreeNode *node, void (*callback)(CGuiTreeNode *, void *), void *user_data);
CGUI_API void				cgui_tree_node__draw_tree(CGuiContext *ctx, CGuiTreeNode *node, int indent, void *user_data);

CGUI_API CGuiNodeValue		cgui_node_value__create();
CGUI_API void				cgui_node_value__draw(CGuiContext *ctx, CGuiTreeNode *node, void *user_data);
CGuiComponentProps			*cgui_node_value__get_component_props(CGuiTreeNode *node, void *user_data);

CGUI_API void				cgui_draw_styled_text(CGuiContext *ctx, CGuiText *text_object, CGuiPanelParams *params);
CGUI_API void				cgui_draw_bevel_rect(CGuiContext *ctx, int x, int y, int width, int height, CGuiColor background);

CGUI_API void				cgui_button__draw(CGuiContext *ctx, CGuiTreeNode *node, void *user_data);
CGUI_API void				cgui_panel__draw(CGuiContext *ctx, CGuiTreeNode *node, void *user_data);
CGUI_API void				cgui_label__draw(CGuiContext *ctx, CGuiTreeNode *node, void *user_data);
CGUI_API CGuiComponentProps *cgui_button__get_props(CGuiTreeNode *node, void *user_data);
CGUI_API CGuiComponentProps *cgui_panel__get_props(CGuiTreeNode *node, void *user_data);
CGUI_API CGuiComponentProps *cgui_label__get_props(CGuiTreeNode *node, void *user_data);

CGUI_API CGuiPanelParams	cgui_panel_params__default();
CGUI_API CGuiText			cgui_create_text_object(const char *text);
CGUI_API CGuiComponentProps	cgui_create_component_props();
CGUI_API bool				cgui_check_collision_point_rec(int px, int py, int rx, int ry, int rwidth, int rheight);
CGUI_API bool				cgui_check_collision_point_circle(int px, int py, int cx, int cy, int radius);

CGUI_API void				*cgui_component__get(CGuiComponent *component);

CGUI_API void				cgui_context__init(CGuiContext *ctx);
CGUI_API int				cgui_context__add_component(CGuiContext *ctx, CGuiComponent *component);
CGUI_API CGuiButton			*cgui_context__create_button(CGuiContext *ctx, const char *text, void (*callback)(CGuiContext *ctx, CGuiTreeNode *button), void *user_data);;
CGUI_API CGuiPanel			*cgui_context__create_panel(CGuiContext *ctx);
CGUI_API CGuiLabel			*cgui_context__create_label(CGuiContext *ctx, const char *text);
CGUI_API void				cgui_context__free(CGuiContext *ctx);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif CGUI_H

#ifndef CGUI_STRIP_PREFIX_GUARD_
#define CGUI_STRIP_PREFIX_GUARD_
#	ifdef CGUI_STRIP_PREFIX
#		define LayoutMode				CGuiLayoutMode
#		define Button					CGuiButton
#		define Panel					CGuiPanel
#		define PanelParams				CGuiPanelParams
#		define Label					CGuiLabel
#		define TreeNode					CGuiTreeNode
#		define NodeValue				CGuiNodeValue
#		define Context					CGuiContext
#		define Component				CGuiComponent

#		define button__draw				cgui_button__draw
#		define draw_bevel_rect			cgui_draw_bevel_rect
#		define panel__draw				cgui_panel__draw
#		define panel_params__default	cgui_panel_params__default
#		define tree_node__create		cgui_tree_node__create
#		define tree_node__add_child		cgui_tree_node__add_child
#		define node_value__draw			cgui_node_value__draw
#		define tree_node__free			cgui_tree_node__free
#		define tree_node__free			cgui_tree_node__free
#		define tree_node__execute		cgui_tree_node__execute
#		define context__init			cgui_context__init
#		define context__add_component	cgui_context__add_component
#		define context__create_button	cgui_context__create_button
#		define context__create_panel	cgui_context__create_panel
#		define context__create_label	cgui_context__create_label
#		define context__free			cgui_context__free
#	endif // CGUI_STRIP_PREFIX
#endif // CGUI_STRIP_PREFIX_GUARD_