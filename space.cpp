#include "space.hpp"

#include <stdexcept>

#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Math/MathDefs.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/Graphics/RenderPath.h>

namespace BigSpace
{

Space::Space(Urho3D::Context* context, float cube_width) :
Urho3D::Object(context),
cube_width(cube_width)
{
}

Layer* Space::createLayer(Urho3D::RenderPath* renderpath, unsigned zoom, float near_clip, float far_clip)
{
	if (layers.find(zoom) != layers.end()) {
		throw std::runtime_error("Layer with that zoom level already exists!");
	}
	if (!Urho3D::IsPowerOfTwo(zoom)) {
		throw std::runtime_error("Layer zoom must be power of two!");
	}

	Urho3D::SharedPtr<Layer> new_layer(new Layer(this, renderpath, zoom, near_clip, far_clip));
	layers[zoom] = new_layer;

	return new_layer;
}

void Space::createViewports()
{
	Urho3D::Renderer* renderer = GetSubsystem<Urho3D::Renderer>();

	unsigned viewport_id = 0;
	for (Layers::reverse_iterator i = layers.rbegin(); i != layers.rend(); ++ i) {
		Layer* layer = i->second;
		Urho3D::Viewport* viewport = layer->getOrCreateViewport();
		viewport->SetRenderPath(layer->getRenderPath());
		renderer->SetViewport(viewport_id ++, viewport);
	}
}

void Space::destroyViewports()
{
	Urho3D::Renderer* renderer = GetSubsystem<Urho3D::Renderer>();

	for (unsigned viewport_id = 0; renderer->GetViewport(viewport_id); ++ viewport_id) {
		renderer->SetViewport(viewport_id, NULL);
	}
	for (Layers::iterator i = layers.begin(); i != layers.end(); ++ i) {
		Layer* layer = i->second;
		layer->destroyViewport();
	}
}

void Space::addRelativeMovementToCamera(Urho3D::Vector3 const& movement)
{
	Urho3D::Vector3 absolute_movement = camera_rot* movement;
	addAbsoluteMovementToCamera(absolute_movement);
}

void Space::addAbsoluteMovementToCamera(Urho3D::Vector3 const& movement)
{
	camera_pos += movement;

	// Check if camera has moved to another cube
	if (camera_pos.x_ < 0) {
		int movement_cubes = Urho3D::FloorToInt(-camera_pos.x_ / cube_width);
		camera_pos.x_ += cube_width * movement_cubes;
		camera_cubepos.x_ -= movement_cubes;
	} else if (camera_pos.x_ >= cube_width) {
		int movement_cubes = Urho3D::FloorToInt(camera_pos.x_ / cube_width) - 1;
		camera_pos.x_ -= cube_width * movement_cubes;
		camera_cubepos.x_ += movement_cubes;
	}
	if (camera_pos.y_ < 0) {
		int movement_cubes = Urho3D::FloorToInt(-camera_pos.y_ / cube_width);
		camera_pos.y_ += cube_width * movement_cubes;
		camera_cubepos.y_ -= movement_cubes;
	} else if (camera_pos.y_ >= cube_width) {
		int movement_cubes = Urho3D::FloorToInt(camera_pos.y_ / cube_width) - 1;
		camera_pos.y_ -= cube_width * movement_cubes;
		camera_cubepos.y_ += movement_cubes;
	}
	if (camera_pos.z_ < 0) {
		int movement_cubes = Urho3D::FloorToInt(-camera_pos.z_ / cube_width);
		camera_pos.z_ += cube_width * movement_cubes;
		camera_cubepos.z_ -= movement_cubes;
	} else if (camera_pos.z_ >= cube_width) {
		int movement_cubes = Urho3D::FloorToInt(camera_pos.z_ / cube_width) - 1;
		camera_pos.z_ -= cube_width * movement_cubes;
		camera_cubepos.z_ += movement_cubes;
	}
}

void Space::rotateCamera(Urho3D::Quaternion const& rot)
{
	camera_rot = camera_rot * rot;
}

void Space::updateCameras()
{
	for (Layers::iterator i = layers.begin(); i != layers.end(); ++ i) {
		Layer* layer = i->second;
		layer->updateCamera(camera_pos, camera_cubepos, camera_rot);
	}
}

}
