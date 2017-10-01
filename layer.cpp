#include "layer.hpp"

#include "space.hpp"
#include "utils.hpp"

#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Skybox.h>
#include <Urho3D/Resource/ResourceCache.h>

namespace BigSpace
{

Layer::Layer(Space* space, unsigned zoom, float near_clip, float far_clip) :
Urho3D::Object(space->GetContext()),
space(space),
zoom(zoom)
{
	scene = new Urho3D::Scene(context_);
	scene->CreateComponent<Urho3D::Octree>();

	camera_node = scene->CreateChild("Camera");
	Urho3D::Camera* camera = camera_node->CreateComponent<Urho3D::Camera>();
	camera->SetNearClip(near_clip);
	camera->SetFarClip(far_clip);
}

void Layer::createSkybox(Urho3D::Material* skybox_mat)
{
	Urho3D::ResourceCache* resources = GetSubsystem<Urho3D::ResourceCache>();

	Urho3D::Node* skybox_node = scene->CreateChild("Sky");
	Urho3D::Skybox* skybox = skybox_node->CreateComponent<Urho3D::Skybox>();
	skybox->SetModel(resources->GetResource<Urho3D::Model>("Models/Box.mdl"));
	skybox->SetMaterial(skybox_mat);
}

NodeWrapper* Layer::createNodeWrapper()
{
	Urho3D::SharedPtr<NodeWrapper> nodewrapper(new NodeWrapper(this, scene->CreateChild()));
	nodewrappers.Insert(nodewrapper);
	nodewrapper->updateActualPosition(camera_zoomed_origin, zoom, space->getCubeWidth());
	return nodewrapper;
}

void Layer::destroyNodeWrapper(NodeWrapper* nodewrapper)
{
	nodewrapper->getNode()->Remove();
	nodewrappers.Erase(Urho3D::SharedPtr<NodeWrapper>(nodewrapper));
}

Urho3D::Viewport* Layer::getOrCreateViewport()
{
	if (camera_viewport.Null()) {
		camera_viewport = new Urho3D::Viewport(context_, scene, camera_node->GetComponent<Urho3D::Camera>());
	}
	return camera_viewport;
}

void Layer::destroyViewport()
{
	camera_viewport = NULL;
}

void Layer::updateCamera(Urho3D::Vector3 const& camera_pos, Urho3D::IntVector3 const& camera_cubepos, Urho3D::Quaternion const& camera_rot)
{
	// Apply zooming
	Urho3D::IntVector3 zoomed_cubepos;
	Urho3D::Vector3 zoomed_pos;
	zoomDiv(zoomed_cubepos.x_, zoomed_pos.x_, camera_cubepos.x_, camera_pos.x_, zoom, space->getCubeWidth());
	zoomDiv(zoomed_cubepos.y_, zoomed_pos.y_, camera_cubepos.y_, camera_pos.y_, zoom, space->getCubeWidth());
	zoomDiv(zoomed_cubepos.z_, zoomed_pos.z_, camera_cubepos.z_, camera_pos.z_, zoom, space->getCubeWidth());

	if (zoomed_cubepos != camera_zoomed_origin) {
		camera_zoomed_origin = zoomed_cubepos;

		// Update actual positions of NodeWrappers
		for (NodeWrappers::Iterator i = nodewrappers.Begin(); i != nodewrappers.End(); ++ i) {
			NodeWrapper* nodewrapper = *i;
			nodewrapper->updateActualPosition(zoomed_cubepos, zoom, space->getCubeWidth());
		}
	}

	camera_node->SetPosition(zoomed_pos);
	camera_node->SetRotation(camera_rot);
}

}
