from ctypes import POINTER, CFUNCTYPE, c_void_p, c_float, c_uint8, c_char_p, c_int, c_bool, c_size_t, byref
import ctypes

import pyray

cgui = ctypes.CDLL("./build/Debug/CGui.dll")

class CGuiColor(ctypes.Structure):
	_fields_ = [
		("r", c_uint8),
		("g", c_uint8),
		("b", c_uint8),
		("a", c_uint8),
	]

class CGuiIVec(ctypes.Structure):
	_fields_ = [
		("x", c_int),
		("y", c_int),
	]

class CGuiTreeNode(ctypes.Structure):
	_fields_ = [
		("id", 				c_int),
		("value", 			c_void_p),
		("children", 		POINTER(c_void_p)),
		("children_count", 	c_size_t),
		("children_cap", 	c_size_t),
	]

class CGuiNodeValueVTable(ctypes.Structure):
	_fields = [("draw", CFUNCTYPE(None, c_void_p, POINTER(CGuiTreeNode), c_void_p))]

class CGuiPanel(ctypes.Structure):
	_fields_ = [
		("vptr",		POINTER(CGuiNodeValueVTable)),
		("layout_mode",	c_int),
	]

class CGuiPanelParams(ctypes.Structure):
	_fields_ = [
		("x",				c_int),
		("y",				c_int),
		("width",			c_int),
		("height",			c_int),
		("padding_top",		c_int),
		("padding_bottom",	c_int),
		("padding_left",	c_int),
		("padding_right",	c_int),
		("background",		CGuiColor),
		("border",			CGuiColor),
		("has_focus",		c_bool),
	]

ffi_draw_line			= CFUNCTYPE(None, c_int, c_int, c_int, c_int, c_int, CGuiColor)
ffi_draw_rectangle		= CFUNCTYPE(None, c_int, c_int, c_int, c_int, c_int, CGuiColor)
ffi_is_mouse_pressed	= CFUNCTYPE(c_bool, c_int)
ffi_is_mouse_down		= CFUNCTYPE(c_bool, c_int)
ffi_is_mouse_up			= CFUNCTYPE(c_bool, c_int)
ffi_mouse_pos			= CFUNCTYPE(CGuiIVec)

class CGuiContext(ctypes.Structure):
	_fields_ = [
		("draw_line",			ffi_draw_line),
		("draw_rectangle",		ffi_draw_rectangle),
		("is_mouse_pressed",	ffi_is_mouse_pressed),
		("is_mouse_down",		ffi_is_mouse_down),
		("is_mouse_up",			ffi_is_mouse_up),
		("mouse_pos",			ffi_mouse_pos),
	]

cgui.cgui_panel__create.argtypes = []
cgui.cgui_panel__create.restype = CGuiPanel

cgui.cgui_node_value__draw.argtypes = [POINTER(CGuiContext), POINTER(CGuiTreeNode), c_void_p]
cgui.cgui_node_value__draw.restype = None

cgui.cgui_tree_node__create.argtypes = [c_void_p]
cgui.cgui_tree_node__create.restype = POINTER(CGuiTreeNode)

cgui.cgui_panel_params__default.argtypes = []
cgui.cgui_panel_params__default.restype = CGuiPanelParams

class Context:
	def __init__(self):
		self._draw_line			= ffi_draw_line(self.draw_line)
		self._draw_rectangle	= ffi_draw_rectangle(self.draw_rectangle)
		self._is_mouse_pressed	= ffi_is_mouse_pressed(self.is_mouse_pressed)
		self._is_mouse_down		= ffi_is_mouse_down(self.is_mouse_down)
		self._is_mouse_up		= ffi_is_mouse_up(self.is_mouse_up)
		# self._mouse_pos			= ffi_mouse_pos(self.mouse_pos)
		self.struct = CGuiContext(
			self._draw_line,
			self._draw_rectangle,
			self._is_mouse_pressed,
			self._is_mouse_down,
			self._is_mouse_up,
			# self._mouse_pos,
		)

	def draw_line(x1, y1, x2, y2, thickness, color):
		pyray.draw_line_ex(pyray.vector2(x1, y1), pyray.vector2(x2, y2), thickness, color)

	def draw_rectangle(x, y, width, height, thickness, color):
		if thickness > 0:
			pyray.draw_rectangle_lines_ex(pyray.Rectangle(x, y, width, height), thickness, color)
		else:
			pyray.draw_rectangle(x, y, width, height, color)

	def is_mouse_pressed(button):
		return pyray.is_mouse_button_pressed(button)

	def is_mouse_down(button):
		return pyray.is_mouse_button_down(button)

	def is_mouse_up(button):
		return pyray.is_mouse_button_up(button)

	def mouse_pos():
		pos = pyray.get_mouse_position()
		return CGuiIVec(int(pos.x), int(pos.y))

if __name__ == "__main__":
	pyray.set_trace_log_level(pyray.LOG_ERROR)
	pyray.init_window(800, 600, "Test")

	ctx = Context()
	panel = cgui.cgui_panel__create()
	root = cgui.cgui_tree_node__create(byref(panel))
	params = cgui.cgui_panel_params__default()

	while not pyray.window_should_close():
		pyray.begin_drawing()
		pyray.clear_background(pyray.BLACK)

		cgui.cgui_node_value__draw(byref(ctx.struct), root, byref(params))

		pyray.end_drawing()