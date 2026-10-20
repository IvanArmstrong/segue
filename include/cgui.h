#ifndef CGUI_H
#define CGUI_H

#ifdef CGUI_STRIP_PREFIX
#	define Button					CGuiButton
#	define LayoutMode				CGuiLayoutMode
#	define Panel					CGuiPanel
#	define PanelParams				CGuiPanelParams

#	define button__draw				cgui_button__draw
#	define button__create			cgui_button__create
#	define draw_bevel_rect			cgui_draw_bevel_rect
#	define panel__draw				cgui_panel__draw
#	define panel__create			cgui_panel__create
#	define panel_params__default	cgui_panel_params__default
#endif // CGUI_STRIP_PREFIX

#include <button.h>
#include <drawing.h>
#include <layout.h>
#include <panel.h>
#include <tree.h>

#endif CGUI_H