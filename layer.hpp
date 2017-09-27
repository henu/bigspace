#ifndef BIGSPACE_LAYER_HPP
#define BIGSPACE_LAYER_HPP

#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Scene/Scene.h>

namespace BigSpace
{

class Space;

class Layer : public Urho3D::Object
{
	URHO3D_OBJECT(Layer, Urho3D::Object)

public:

	Layer(Space* space, unsigned zoom, float near_clip, float far_clip);

	// Easy method to create sky. This should only
	// be called to Layer that has the biggest zoom.
	void createSkybox(Urho3D::Material* skybox_mat);

	// Called by Space
	Urho3D::Viewport* getOrCreateViewport();
	void destroyViewport();
	void updateCamera(Urho3D::Vector3 const& camera_pos, Urho3D::IntVector3 const& camera_cubepos, Urho3D::Quaternion const& camera_rot);

private:

	Space* space;

	Urho3D::SharedPtr<Urho3D::Scene> scene;

	unsigned zoom;

	Urho3D::Node* camera_node;
	Urho3D::SharedPtr<Urho3D::Viewport> camera_viewport;
	Urho3D::IntVector3 camera_zoomed_origin;
};

}

#endif

