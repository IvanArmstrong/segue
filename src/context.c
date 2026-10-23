#include <cgui.h>

#include <stdarg.h>

void cgui_context__init(CGuiContext *ctx) {
	ctx->components			= NULL;
	ctx->components_count	= 0;
	ctx->components_cap		= 0;
}

int cgui_context__add_component(CGuiContext *ctx, CGuiComponent *component) {
	if (ctx->components_count >= ctx->components_cap) {
		size_t new_cap = ctx->components_cap ? ctx->components_cap * 2 : 4;
		CGuiComponent **temp = realloc(ctx->components, new_cap * sizeof *temp);
		if (!temp)
			return -1;
		ctx->components = temp;
		ctx->components_cap = new_cap;
	}
	ctx->components[ctx->components_count++] = component;
	return 0;
}

CGuiButton *cgui_context__create_button(CGuiContext *ctx, 
		const char *text, 
		void (*callback)(CGuiContext *ctx, CGuiTreeNode *button), 
		void *user_data) {
	static CGuiNodeValueVTable vtable = {
		.draw = cgui_button__draw,
		.get_props = cgui_button__get_props,
		.expanded = _cgui_node_value__expanded,
	};
	CGuiComponent *component = malloc(sizeof *component);
	component->kind = CGUI_COMP_BUTTON;
	component->value.button = (CGuiButton) {
		.vptr				= &vtable,
		.props				= cgui_create_component_props(),
		.text_object		= cgui_create_text_object(text),
		.callback			= callback,
		.user_data			= user_data,
		.pressed			= false,
		.highlight_color	= CGUI_LIGHTGRAY,
		.press_color		= CGUI_DARKGRAY,
		.normal_color		= CGUI_GRAY,
	};
	cgui_context__add_component(ctx, component);
	return &component->value.button;
}

CGuiPanel *cgui_context__create_panel(CGuiContext *ctx) {
	static CGuiNodeValueVTable vtable = {
		.draw = cgui_panel__draw,
		.get_props = cgui_panel__get_props,
		.expanded = _cgui_node_value__expanded,
	};
	CGuiComponent *component = malloc(sizeof *component);
	component->kind = CGUI_COMP_PANEL;
	component->value.panel = (CGuiPanel) {
		.vptr			= &vtable,
		.props			= cgui_create_component_props(),
		.layout_mode	= Vertical,
	};
	cgui_context__add_component(ctx, component);
	return &component->value.panel;
}

CGuiLabel *cgui_context__create_label(CGuiContext *ctx, const char *text) {
	static CGuiNodeValueVTable vtable = {
		.draw = cgui_label__draw,
		.get_props = cgui_label__get_props,
		.expanded = _cgui_node_value__expanded,
	};
	CGuiComponent *component = malloc(sizeof *component);
	component->kind = CGUI_COMP_LABEL;
	component->value.label = (CGuiLabel) {
		.vptr			= &vtable,
		.props			= cgui_create_component_props(),
		.text_object	= cgui_create_text_object(text),
	};
	cgui_context__add_component(ctx, component);
	return &component->value.label;
}

void cgui_context__free(CGuiContext *ctx) {
	for (size_t i = 0; i < ctx->components_count; i++) {
		free(ctx->components[i]);
	}
	free(ctx->components);
}
