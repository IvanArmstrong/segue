#include <stdio.h>
#include <raylib.h>

#define CGUI_STRIP_PREFIX
#include <cgui.h>

void button_cb(void *user_data) {
	(void) user_data;
	printf("BUTTON PRESSED SUKA!!!\n");
}

int main(int argc, char const *argv[]) {
	Panel root_value = panel__create();
	TreeNode *root = tree_node__create(&root_value);

	Panel child1_value = panel__create();
	child1_value.layout_mode = Horizontal;
	TreeNode *child1 = tree_node__create(&child1_value);

	Button child2_value = button__create(button_cb);
	TreeNode *child2 = tree_node__create(&child2_value);

	Panel child3_value = panel__create();
	TreeNode *child3 = tree_node__create(&child3_value);

	Panel child4_value = panel__create();
	TreeNode *child4 = tree_node__create(&child4_value);

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
		node_value__draw(root, &params);

		EndDrawing();
	}

	tree_node__free(root, NULL);

	return 0;
}