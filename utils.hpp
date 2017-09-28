#ifndef BIGSPACE_UTILS_HPP
#define BIGSPACE_UTILS_HPP

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

}

#endif
