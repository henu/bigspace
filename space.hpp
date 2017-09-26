#ifndef BIGSPACE_SPACE_HPP
#define BIGSPACE_SPACE_HPP

#include "layer.hpp"

#include <Urho3D/Container/Ptr.h>
#include <Urho3D/Math/Quaternion.h>
#include <Urho3D/Math/Vector3.h>

#include <map>

namespace BigSpace
{

class Space : public Urho3D::Object
{
	URHO3D_OBJECT(Space, Urho3D::Object)

public:

	Space(Urho3D::Context* context, float cube_width);

	Layer* createLayer(unsigned zoom, float near_clip, float far_clip);

	// TODO: Call these automatically!
	void createViewports();
	void destroyViewports();

private:

	typedef std::map<unsigned, Urho3D::SharedPtr<Layer> > Layers;

	// Options
	float const cube_width;

	Layers layers;

	// Camera properties
	Urho3D::Vector3 camera_pos;
	Urho3D::IntVector3 camera_cubepos;
	Urho3D::Quaternion camera_rot;
};

}

#endif
