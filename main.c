#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <raylib.h>

#include "graphics_imp.h"

void add_button(Context *ctx, TreeNode *parent) {
	static int button_id = 0;
	static char str[20][20] = { 0 };
	Container *value = context__create_container(ctx);
	value->layout_mode = MODE_FLEX_HORIZ;
	value->props.basis = 40;
	TreeNode *container = tree_node__create(value);
	tree_node__add_child(parent, container);

	sprintf(str[button_id], "Button %d", button_id);
	Button *button_value = context__create_button(ctx, str[button_id], NULL, NULL);
	button_value->props.basis = 100;
	TreeNode *button = tree_node__create(button_value);
	tree_node__add_child(container, button);

	button_id++;
}

void add_button_cb(Context *ctx, TreeNode *button) {
	Button *value = button->value;
	TreeNode *node = value->user_data;
	add_button(ctx, node);
}

void remove_node(Context *ctx, TreeNode *button) {
	(void) ctx;
	Button *value = (Button *) button->value;
	TreeNode *parent = (TreeNode *) value->user_data;
	Container *container = parent->value;
	if (container->selection_index < 0)
		return;
	tree_node__remove_child(parent, container->selection_index);
}

int main(int argc, char const *argv[]) {
	srand((unsigned int) time(NULL));

	CGuiContext ctx = cgui_create_context();
	cgui_context__init(&ctx);

	Container *root_value = context__create_container(&ctx);
	root_value->style = CONT_STYLE_RAISED;
	root_value->layout_mode = MODE_FLEX_VERT;
	TreeNode *root = tree_node__create(root_value);

	// Header
	Container *header_container = context__create_container(&ctx);
	// header_container->style = CONT_STYLE_RAISED;
	header_container->layout_mode = MODE_FLEX_HORIZ;
	header_container->props.basis = 40;
	TreeNode *header = tree_node__create(header_container);
	tree_node__add_child(root, header);

	for (int i = 0; i < 5; i++) {
		Button *button_value = context__create_button(&ctx, "BUTTON", NULL, NULL);
		button_value->props.basis = 100;
		TreeNode *button = tree_node__create(button_value);
		tree_node__add_child(header, button);
	}
	Label *label_value = context__create_label(&ctx, NULL);
	label_value->props.flex_mode = FLEX_MODE_AUTO;
	TreeNode *label = tree_node__create(label_value);
	tree_node__add_child(header, label);
	{
		Button *button_value = context__create_button(&ctx, "BUTTON", NULL, NULL);
		button_value->props.basis = 100;
		TreeNode *button = tree_node__create(button_value);
		tree_node__add_child(header, button);
	}

	// Body
	Container *body_container = context__create_container(&ctx);
	// body_container->style = CONT_STYLE_RAISED;
	body_container->layout_mode = MODE_FLEX_HORIZ;
	body_container->props.flex_mode = FLEX_MODE_AUTO;
	TreeNode *body = tree_node__create(body_container);
	tree_node__add_child(root, body);

	// Body button flex container
	Container *body_button_container_value = context__create_container(&ctx);
	body_button_container_value->style = CONT_STYLE_INSET;
	body_button_container_value->layout_mode = MODE_FLEX_VERT;
	body_button_container_value->props.flex_mode = FLEX_MODE_AUTO;
	body_button_container_value->can_select = true;
	body_button_container_value->props.padding = (Padding) { 0 };
	TreeNode *body_button_container = tree_node__create(body_button_container_value);
	tree_node__add_child(body, body_button_container);

	for (int i = 0; i < 20; i++) {
		add_button(&ctx, body_button_container);
	}

	{
		// Add button
		Button *button_value = context__create_button(&ctx, "Add", add_button_cb, body_button_container);
		button_value->props.basis = 100;
		TreeNode *button = tree_node__create(button_value);
		tree_node__add_child(header, button);
	}

	// Body Coordinate Container
	Container *body_coord_container_value = context__create_container(&ctx);
	body_coord_container_value->style = CONT_STYLE_INSET;
	body_coord_container_value->layout_mode = MODE_COORD;
	body_coord_container_value->props.flex_mode = FLEX_MODE_AUTO;
	TreeNode *body_coord_container = tree_node__create(body_coord_container_value);
	tree_node__add_child(body, body_coord_container);
	// Body Coordinate children
	{
		Button *button_value = context__create_button(&ctx, "Remove", remove_node, body_button_container);
		button_value->props.x = 25;
		button_value->props.y = 25;
		button_value->props.width = 100;
		button_value->props.height = 40;
		TreeNode *button = tree_node__create(button_value);
		tree_node__add_child(body_coord_container, button);
	}

	// Footer
	Container *footer_container = context__create_container(&ctx);
	// footer_container->style = CONT_STYLE_RAISED;
	footer_container->reversed = true;
	footer_container->layout_mode = MODE_FLEX_HORIZ;
	footer_container->props.basis = 40;
	TreeNode *footer = tree_node__create(footer_container);
	tree_node__add_child(root, footer);

	for (int i = 0; i < 3; i++) {
		Button *button_value = context__create_button(&ctx, "BUTTON", NULL, NULL);
		button_value->props.basis = 100;
		TreeNode *button = tree_node__create(button_value);
		tree_node__add_child(footer, button);
	}

	SetTraceLogLevel(LOG_ERROR);
	InitWindow(1152, 648, "C Gui!");
	SetWindowState(FLAG_WINDOW_RESIZABLE);

	ContainerParams params = container_params__default();
	params.x = 25;
	params.y = 25;
	params.width = 800;
	params.height = 600;

	bool a_held = false;
	bool b_held = false;

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);

		// params.x = 0;
		// params.y = 0;
		// params.width = GetScreenWidth();
		// params.height = GetScreenHeight();

		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			Vector2 mouse_pos = GetMousePosition();
			Vector2 a = { params.x, params.y };
			Vector2 b = { params.x + params.width, params.y + params.height };
			if (CheckCollisionPointCircle(mouse_pos, a, 3) || a_held) {
				params.x = (int) mouse_pos.x;
				params.y = (int) mouse_pos.y;
				a_held = true;
				b_held = false;
			} else if (CheckCollisionPointCircle(mouse_pos, b, 3) || b_held) {
				params.width = (int) mouse_pos.x - params.x;
				params.height = (int) mouse_pos.y - params.y;
				a_held = false;
				b_held = true;
			}
		} else {
			a_held = false;
			b_held = false;
		}
		node_value__draw(&ctx, root, &params);

		DrawFPS(1, 1);

		EndDrawing();
	}

	tree_node__free(root, NULL);
	context__free(&ctx);

	return 0;
}