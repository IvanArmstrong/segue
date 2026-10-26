#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <raylib.h>

#include "graphics_imp.h"

void test_button_cb(Context *ctx, TreeNode *button) {
	static int next_container = 0;
	Button *button_value = (Button *) button->value;
	TreeNode *node = (TreeNode *) button_value->user_data;
	Container *new_container = context__create_container(ctx);
	Container *grand_child_container = context__create_container(ctx);
	Label *label = context__create_label(ctx, "ASDSFASF");
	label->text_object.horiz_align = abs(rand() % 3);
	label->text_object.vert_align = abs(rand() % 3);
	new_container->layout_mode = MODE_FLEX_HORIZ;
	TreeNode *child = tree_node__create(new_container);
	TreeNode *grand_child = tree_node__create(grand_child_container);
	TreeNode *great_grand_child = tree_node__create(label);
	tree_node__add_child(grand_child, great_grand_child);
	tree_node__add_child(child, grand_child);
	tree_node__add_child(node, child);
	button_value->user_data = child;
}

int main(int argc, char const *argv[]) {
	srand(time(NULL));

	CGuiContext ctx = cgui_create_context();
	cgui_context__init(&ctx);

	Container *root_value = context__create_container(&ctx);
	root_value->layout_mode = MODE_FLEX_VERT;
	TreeNode *root = tree_node__create(root_value);

	// Header
	Container *header_container = context__create_container(&ctx);
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
	body_container->layout_mode = MODE_COORD;
	body_container->props.basis = 0;
	body_container->props.flex_mode = FLEX_MODE_AUTO;
	TreeNode *body = tree_node__create(body_container);
	tree_node__add_child(root, body);
	// Body Coordinate children
	{
		Button *button_value = context__create_button(&ctx, "BUTTON", NULL, NULL);
		button_value->props.x = 25;
		button_value->props.y = 25;
		button_value->props.width = 100;
		button_value->props.height = 40;
		TreeNode *button = tree_node__create(button_value);
		tree_node__add_child(body, button);
	}

	// Footer
	Container *footer_container = context__create_container(&ctx);
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
	params.x = 100;
	params.y = 100;
	params.width = 600;
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