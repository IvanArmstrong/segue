#include <stdio.h>
#include <raylib.h>

#include "graphics_imp.h"

void test_button_cb(Context *ctx, TreeNode *button) {
	static int next_panel = 0;
	Button *button_value = (Button *) button->value;
	TreeNode *node = (TreeNode *) button_value->user_data;
	Panel *new_panel = context__create_panel(ctx);
	Panel *grand_child_panel = context__create_panel(ctx);
	Label *label = context__create_label(ctx, "ASDSFASF");
	label->text_object.horiz_align = ALIGN_HORIZ_CENTER;
	label->text_object.vert_align = ALIGN_VERT_BOTTOM;
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
	CGuiContext ctx = cgui_create_context();
	cgui_context__init(&ctx);

	Panel *root_value = context__create_panel(&ctx);
	TreeNode *root = tree_node__create(root_value);

	Panel *child1_value = context__create_panel(&ctx);
	child1_value->layout_mode = Horizontal;
	TreeNode *child1 = tree_node__create(child1_value);

	Button *button = context__create_button(&ctx, "TEST", test_button_cb, root);
	TreeNode *child2 = tree_node__create(button);

	Panel *child3_value = context__create_panel(&ctx);
	TreeNode *child3 = tree_node__create(child3_value);

	Panel *child4_value = context__create_panel(&ctx);
	TreeNode *child4 = tree_node__create(child4_value);

	tree_node__add_child(child1, child2);
	tree_node__add_child(child1, child4);
	tree_node__add_child(root, child1);
	tree_node__add_child(root, child3);

	SetTraceLogLevel(LOG_ERROR);
	InitWindow(1152, 648, "C Gui!");

	PanelParams params = panel_params__default();
	params.x = 200;
	params.y = 200;
	params.width = 500;
	params.height = 400;

	bool a_held = false;
	bool b_held = false;

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);

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

		EndDrawing();
	}

	tree_node__free(root, NULL);
	context__free(&ctx);

	return 0;
}