#include <cgui.h>

void cgui_label__draw(CGuiContext *ctx, CGuiTreeNode *node, void *user_data) {
	CGuiLabel *label = (CGuiLabel *) node->value;
	CGuiContainerParams *params = (CGuiContainerParams *) user_data;

	cgui_draw_styled_text(ctx, &label->text_object, params);
}

CGuiComponentProps *cgui_label__get_props(CGuiTreeNode *node, void *user_data) {
	(void) user_data;
	
	CGuiLabel *label = (CGuiLabel *) node->value;
	return &label->props;
}