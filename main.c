#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <raylib.h>

#include "graphics_imp.h"

void test_button_cb(Context *ctx, TreeNode *button) {
	static int next_panel = 0;
	Button *button_value = (Button *) button->value;
	TreeNode *node = (TreeNode *) button_value->user_data;
	Panel *new_panel = context__create_panel(ctx);
	Panel *grand_child_panel = context__create_panel(ctx);
	Label *label = context__create_label(ctx, "ASDSFASF");
	label->text_object.horiz_align = abs(rand() % 3);
	label->text_object.vert_align = abs(rand() % 3);
	new_panel->layout_mode = Horizontal;
	TreeNode *child = tree_node__create(new_panel);
	TreeNode *grand_child = tree_node__create(grand_child_panel);
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

	Panel *root_value = context__create_panel(&ctx);
	root_value->layout_mode = Vertical;
	TreeNode *root = tree_node__create(root_value);

	// Header
	Panel *header_panel = context__create_panel(&ctx);
	header_panel->layout_mode = Horizontal;
	header_panel->props.basis = 40;
	TreeNode *header = tree_node__create(header_panel);
	tree_node__add_child(root, header);

	for (int i = 0; i < 5; i++) {
		Button *button_value = context__create_button(&ctx, "BUTTON", NULL, NULL);
		button_value->props.basis = 100;
		TreeNode *button = tree_node__create(button_value);
		tree_node__add_child(header, button);
	}

	// Body
	Panel *body_panel = context__create_panel(&ctx);
	body_panel->props.basis = 0;
	TreeNode *body = tree_node__create(body_panel);
	tree_node__add_child(root, body);

	// Footer
	Panel *footer_panel = context__create_panel(&ctx);
	footer_panel->layout_mode = Horizontal;
	footer_panel->props.basis = 40;
	TreeNode *footer = tree_node__create(footer_panel);
	tree_node__add_child(root, footer);

	for (int i = 0; i < 3; i++) {
		Button *button_value = context__create_button(&ctx, "BUTTON", NULL, NULL);
		button_value->props.basis = 100;
		TreeNode *button = tree_node__create(button_value);
		tree_node__add_child(footer, button);
	}

	// Panel *child1_value = context__create_panel(&ctx);
	// child1_value->layout_mode = Horizontal;
	// TreeNode *child1 = tree_node__create(child1_value);

	// Button *button = context__create_button(&ctx, "TEST", test_button_cb, root);
	// TreeNode *child2 = tree_node__create(button);

	// Panel *child3_value = context__create_panel(&ctx);
	// TreeNode *child3 = tree_node__create(child3_value);

	// Panel *child4_value = context__create_panel(&ctx);
	// TreeNode *child4 = tree_node__create(child4_value);

	// tree_node__add_child(child1, child2);
	// tree_node__add_child(child1, child4);
	// tree_node__add_child(root, child1);
	// tree_node__add_child(root, child3);


	SetTraceLogLevel(LOG_ERROR);
	InitWindow(1152, 648, "C Gui!");
	SetWindowState(FLAG_WINDOW_RESIZABLE);

	PanelParams params = panel_params__default();

	bool a_held = false;
	bool b_held = false;

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);

		params.x = 0;
		params.y = 0;
		params.width = GetScreenWidth();
		params.height = GetScreenHeight();

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