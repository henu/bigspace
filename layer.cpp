#include "layer.hpp"

#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Skybox.h>
#include <Urho3D/Resource/ResourceCache.h>

namespace BigSpace
{

Layer::Layer(Urho3D::Context* context, float near_clip, float far_clip) :
Urho3D::Object(context)
{
	scene = new Urho3D::Scene(context);
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

}
