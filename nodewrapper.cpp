#include "nodewrapper.hpp"

#include "layer.hpp"
#include "space.hpp"
#include "utils.hpp"

namespace BigSpace
{

NodeWrapper::NodeWrapper(Layer* layer, Urho3D::Node* node) :
layer(layer),
node(node)
{
}

void NodeWrapper::setPosition(Urho3D::IntVector3 const& cubepos, Urho3D::Vector3 const& pos)
{
	this->cubepos = cubepos;
	this->pos = pos;
	updateActualPosition(layer->getCameraZoomedOrigin(), layer->getZoom(), layer->getSpace()->getCubeWidth());
}

void NodeWrapper::updateActualPosition(Urho3D::IntVector3 const& origin, unsigned zoom, float cube_width)
{
	// Calculate zoomed position
	Urho3D::IntVector3 zoomed_cubepos;
	Urho3D::Vector3 zoomed_pos;
	zoomDiv(zoomed_cubepos.x_, zoomed_pos.x_, cubepos.x_, pos.x_, zoom, cube_width);
	zoomDiv(zoomed_cubepos.y_, zoomed_pos.y_, cubepos.y_, pos.y_, zoom, cube_width);
	zoomDiv(zoomed_cubepos.z_, zoomed_pos.z_, cubepos.z_, pos.z_, zoom, cube_width);

	Urho3D::IntVector3 actual_cubepos = zoomed_cubepos - origin;

	Urho3D::Vector3 actual_pos = zoomed_pos;
	actual_pos.x_ += actual_cubepos.x_ * cube_width;
	actual_pos.y_ += actual_cubepos.y_ * cube_width;
	actual_pos.z_ += actual_cubepos.z_ * cube_width;

	node->SetPosition(actual_pos);
}

}
