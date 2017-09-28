#ifndef BIGSPACE_NODEWRAPPER_HPP
#define BIGSPACE_NODEWRAPPER_HPP

#include <Urho3D/Container/RefCounted.h>
#include <Urho3D/Scene/Node.h>

namespace BigSpace
{

class Layer;

class NodeWrapper : public Urho3D::RefCounted
{

public:

	NodeWrapper(Layer* layer, Urho3D::Node* node);

	inline Urho3D::Node* getNode() const { return node; }

	void setPosition(Urho3D::IntVector3 const& cubepos, Urho3D::Vector3 const& pos);

	void updateActualPosition(Urho3D::IntVector3 const& origin, unsigned zoom, float cube_width);

private:

	Layer* layer;
	Urho3D::Node* node;

	Urho3D::IntVector3 cubepos;
	Urho3D::Vector3 pos;
};

}

#endif

