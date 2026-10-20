#ifndef CGUI_UTILS_H
#define CGUI_UTILS_H

#include <stdbool.h>

bool	cgui_check_collision_point_rec(int px, int py, int rx, int ry, int rwidth, int rheight);
bool	cgui_check_collision_point_circle(int px, int py, int cx, int cy, int radius);

#endif // CGUI_UTILS_H