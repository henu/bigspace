#include "space.hpp"

#include <stdexcept>

#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Math/MathDefs.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>

namespace BigSpace
{

Space::Space(Urho3D::Context* context, float cube_width) :
Urho3D::Object(context),
cube_width(cube_width)
{
}

Layer* Space::createLayer(unsigned zoom, float near_clip, float far_clip)
{
	if (layers.find(zoom) != layers.end()) {
		throw std::runtime_error("Layer with that zoom level already exists!");
	}
	if (!Urho3D::IsPowerOfTwo(zoom)) {
		throw std::runtime_error("Layer zoom must be power of two!");
	}

	Urho3D::SharedPtr<Layer> new_layer(new Layer(this, zoom, near_clip, far_clip));
	layers[zoom] = new_layer;

	return new_layer;
}

void Space::createViewports()
{
	Urho3D::Renderer* renderer = GetSubsystem<Urho3D::Renderer>();
	Urho3D::ResourceCache* resources = GetSubsystem<Urho3D::ResourceCache>();

	Urho3D::XMLFile* renderpath_file = resources->GetResource<Urho3D::XMLFile>("RenderPaths/ForwardNoColorClear.xml");

	unsigned viewport_id = 0;
	for (Layers::reverse_iterator i = layers.rbegin(); i != layers.rend(); ++ i) {
		Layer* layer = i->second;
		Urho3D::Viewport* viewport = layer->getOrCreateViewport();
		viewport->SetRenderPath(renderpath_file);
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

void Space::updateCameras()
{
	for (Layers::iterator i = layers.begin(); i != layers.end(); ++ i) {
		Layer* layer = i->second;
		layer->updateCamera(camera_pos, camera_cubepos, camera_rot);
	}
}

}
