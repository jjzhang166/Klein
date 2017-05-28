#include "Core/Mesh.h"
#include "Core/Logger.h"
#include "Core/ResourceManager.h"
#include "Core/Util.h"

#include <Euclid/Geometry/Polyhedron_3.h>
#include <algorithm>
#include <cmath>

Mesh::Mesh(const std::vector<QVector3D>& rawVertices,
	const std::vector<QVector3D>& rawFNormals,
	const std::vector<unsigned>& rawIndices,
	const std::string& positionBuffer,
	const std::string& normalBuffer)
	: indices(rawIndices), positionBufferID(positionBuffer), normalBufferID(normalBuffer), meshID(count++)
{
	vertices.resize(rawVertices.size());
	std::transform(rawVertices.begin(), rawVertices.end(), vertices.begin(),
		[](const QVector3D& v) {
			return qtToEigen<float>(v);
		}
	);

	fNormals.resize(rawFNormals.size());
	std::transform(rawFNormals.begin(), rawFNormals.end(), fNormals.begin(),
		[](const QVector3D& v) {
			return qtToEigen<float>(v);
		}
	);

	cMesh = std::make_unique<CMesh>();
	Euclid::TriMeshBuilder<CMesh> meshBuilder(vertices, indices);
	try {
		cMesh->delegate(meshBuilder);

		size_t i = 0;
		for (auto v = cMesh->vertices_begin(); v != cMesh->vertices_end(); ++v) {
			v->id() = i++;
		}
		i = 0;
		for (auto f = cMesh->facets_begin(); f != cMesh->facets_end(); ++f) {
			f->id() = i++;
		}
	}
	catch (...) {
		KLEIN_LOG_WARNING("The provided mesh is non-manifold, thus cgal-mesh will be unavailable");
		cMesh = nullptr;
	}
}

Mesh::~Mesh() = default;

void Mesh::updateGLBuffer() const
{
	std::vector<QVector3D> positions(indices.size());
	std::transform(indices.begin(), indices.end(), positions.begin(),
		[&vertices = vertices](unsigned idx) {
			return eigenToQt(vertices[idx]);
		}
	);

	std::vector<QVector3D> normals;
	normals.reserve(indices.size());
	for (const auto& fn : fNormals) {
		normals.push_back(eigenToQt(fn));
		normals.push_back(eigenToQt(fn));
		normals.push_back(eigenToQt(fn));
	}

	ResourceManager::instance().addGLBuffer(positionBufferID, positions);
	ResourceManager::instance().addGLBuffer(normalBufferID, normals);
}

bool Mesh::attachTo(GraphicsComponent* graphics)
{
	if (_graphics == nullptr) {
		_graphics = graphics;
		return true;
	}
	else {
		KLEIN_LOG_CRITICAL("This mesh has already been attached to another GraphicsComponent");
		return false;
	}
}

GraphicsComponent* Mesh::attachedGraphics()
{
	return _graphics;
}

unsigned Mesh::count = 0;
