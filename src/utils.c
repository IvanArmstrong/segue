#include <utils.h>

#include <math.h>

bool cgui_check_collision_point_rec(int px, int py, int rx, int ry, int rwidth, int rheight) {
	return px >= rx && py >= ry && px < rx + rwidth && py < ry + rheight;
}

bool cgui_check_collision_point_circle(int px, int py, int cx, int cy, int radius) {
	return sqrt(pow((float) cx - (float) px, 2) + pow((float) cy - (float) py, 2));
}
