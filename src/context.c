#include <cgui.h>

#include <stdarg.h>

void cgui_context__init(CGuiContext *ctx) {
	ctx->components			= NULL;
	ctx->components_count	= 0;
	ctx->components_cap		= 0;

	ctx->string_store		= NULL;
	ctx->string_count		= 0;
	ctx->string_cap			= 0;
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
		char *text, 
		void (*callback)(CGuiContext *ctx, CGuiTreeNode *button), 
		void *user_data) {
	static CGuiNodeValueVTable vtable = {
		.draw = cgui_button__draw,
		.get_props = cgui_button__get_props,
		.expanded = _cgui_node_value__expanded,
		.get_size = _cgui_node_value__get_size,
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

CGuiContainer *cgui_context__create_container(CGuiContext *ctx) {
	static CGuiNodeValueVTable vtable = {
		.draw = cgui_container__draw,
		.get_props = cgui_container__get_props,
		.expanded = _cgui_node_value__expanded,
		.get_size = _cgui_node_value__get_size,
	};
	CGuiComponent *component = malloc(sizeof *component);
	component->kind = CGUI_COMP_CONTAINER;
	component->value.container = (CGuiContainer) {
		.vptr				= &vtable,
		.props				= cgui_create_component_props(),
		.layout_mode		= MODE_FLEX_VERT,
		.reversed			= false,
		.style				= CONT_STYLE_INVISIBLE,

		.overflow_scroll	= 0,
		.can_select			= false,
		.selection_index	= -1,
	};
	cgui_context__add_component(ctx, component);
	return &component->value.container;
}

CGuiLabel *cgui_context__create_label(CGuiContext *ctx, char *text) {
	static CGuiNodeValueVTable vtable = {
		.draw = cgui_label__draw,
		.get_props = cgui_label__get_props,
		.expanded = _cgui_node_value__expanded,
		.get_size = _cgui_node_value__get_size,
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

char *cgui_context__create_str(CGuiContext *ctx, char *source) {
	if (ctx->string_count >= ctx->string_cap) {
		size_t new_cap = ctx->string_cap ? ctx->string_cap * 2 : 4;
		char **temp = realloc(ctx->string_store, new_cap * sizeof *temp);
		if (!temp)
			return NULL;
		ctx->string_store = temp;
		ctx->string_cap = new_cap;
	}
	ctx->string_count++;
	memcpy(ctx->string_store[ctx->string_count], source);
	return ctx->string_store[ctx->string_count];
}

void cgui_context__free_str(CGuiContext *ctx, size_t index) {
	if (index >= ctx->string_count)
		return;

	free(ctx->string_store[index]);

	memmove(&ctx->string_store[index],
			&ctx->string_store[index + 1],
			(ctx->string_count - index - 1) * sizeof *ctx->string_store);

	ctx->string_store[--ctx->string_count] = NULL;
}