#ifndef BIGSPACE_UTILS_HPP
#define BIGSPACE_UTILS_HPP

#include <Urho3D/Math/Vector3.h>

namespace BigSpace
{

inline void zoomDiv(int& result_i, float& result_f, int i, float f, int zoom, float cube_width)
{
	if (i >= 0) {
		result_i = i / zoom;
		int i_left = i % zoom;
		result_f = f / zoom + i_left * cube_width / zoom;
	} else {
		int i_flipped = -i - 1;
		result_i = -(i_flipped / zoom + 1);
		int i_left = i_flipped % zoom;
		result_f = f / zoom + cube_width - (i_left + 1) * cube_width / zoom;
	}
}

inline void addToPosition(Urho3D::IntVector3& cubepos, Urho3D::Vector3& pos, double x, double y, double z, float const cube_width)
{
	int x_i = Urho3D::FloorToInt(x / cube_width);
	int y_i = Urho3D::FloorToInt(y / cube_width);
	int z_i = Urho3D::FloorToInt(z / cube_width);
	float x_f = double(x) - double(x_i * cube_width);
	float y_f = double(y) - double(y_i * cube_width);
	float z_f = double(z) - double(z_i * cube_width);

	cubepos += Urho3D::IntVector3(x_i, y_i, z_i);
	pos += Urho3D::Vector3(x_f, y_f, z_f);
	if (pos.x_ >= cube_width) {
		++ cubepos.x_;
		pos.x_ -= cube_width;
	}
	if (pos.y_ >= cube_width) {
		++ cubepos.y_;
		pos.y_ -= cube_width;
	}
	if (pos.z_ >= cube_width) {
		++ cubepos.z_;
		pos.z_ -= cube_width;
	}
}

}

#endif
