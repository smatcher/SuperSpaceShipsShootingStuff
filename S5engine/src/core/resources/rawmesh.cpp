#include <QtOpenGL>
#include <GL/glu.h>
#include "core/resources/rawmesh.h"
#include "core/managers/rendermanager.h"

//#ifdef _WIN32
	void __ClientActiveTexture(int texid);
//#endif

RawMesh::RawMesh(const QString &name, const QString &path, IResourceFactory *factory) :
	MeshData(name,path,factory),
	m_vertices(),
	m_normals(),
	m_colors(),
	m_texcoords(),
	m_tangents(),
	m_bitangents(),
	m_indices(QGLBuffer::IndexBuffer),
	m_nbFaces(0),
	m_boundingVolume(0)
{
}

bool RawMesh::unload()
{
	m_vertices.destroy();
	m_colors.destroy();
	m_texcoords.destroy();
	m_normals.destroy();
	m_tangents.destroy();
	m_bitangents.destroy();
	m_indices.destroy();

	m_nbFaces = 0;

	if(m_boundingVolume)
	{
		delete m_boundingVolume;
		m_boundingVolume = 0;
	}

	return true;
}

void RawMesh::draw(unsigned int, int flags)
{
	QGLShaderProgram* program = RENDER_MANAGER.getRenderPassInfo()->ubershader_used->program();

	bool wireframe = flags & WIREFRAME;

	if(wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if(!m_vertices.isCreated() || !m_indices.isCreated())
		return;

	if(m_texcoords.isCreated() && !wireframe)
	{
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		__ClientActiveTexture(GL_TEXTURE0);
		m_texcoords.bind();
		glTexCoordPointer(2, GL_FLOAT, 0, 0);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}

	if(m_colors.isCreated())
	{
		glEnable(GL_COLOR_MATERIAL);
		glEnableClientState(GL_COLOR_ARRAY);
		m_colors.bind();
		glColorPointer(4, GL_FLOAT, 0, 0);
	}
	else
	{
		glDisable(GL_COLOR_MATERIAL);
	}

	if(m_normals.isCreated() && !wireframe)
	{
		glEnable(GL_LIGHTING);
		glShadeModel(GL_SMOOTH);
		glEnableClientState(GL_NORMAL_ARRAY);
		m_normals.bind();
		glNormalPointer(GL_FLOAT, 0, 0);
	}
	else
	{
		glDisable(GL_LIGHTING);
	}

	if(program != 0 && !wireframe)
	{
		int location = program->attributeLocation("tangent");
		if(location != -1)
		{
			m_tangents.bind();
			program->enableAttributeArray(location);
			program->setAttributeBuffer(location,GL_FLOAT,0,3);
		}
		location = program->attributeLocation("bitangent");
		if(location != -1)
		{
			m_bitangents.bind();
			program->enableAttributeArray(location);
			program->setAttributeBuffer(location,GL_FLOAT,0,3);
		}
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	m_vertices.bind();
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glEnableClientState(GL_INDEX_ARRAY);
	m_indices.bind();
	glIndexPointer(GL_SHORT, 0, 0);

	glDrawElements(GL_TRIANGLES, 3*m_nbFaces, GL_UNSIGNED_SHORT, 0);

	m_indices.release();
	m_vertices.release();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);

	if(wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	debugGL("while rendering" << name());
}

void RawMesh::draw(unsigned int submesh, const QMap<QString, Matrix4f>& matrix_palette, int flags)
{
	draw(submesh, flags);
}

unsigned int RawMesh::nbSubmeshes()
{
	return 1;
}

int RawMesh::getNbVertices()
{
	return m_nbVertices;
}

int RawMesh::getNbFaces()
{
	return m_nbFaces;
}

const BoundingVolume* RawMesh::getBoundingVolume()
{
	return m_boundingVolume;
}

#ifdef WITH_TOOLS
void RawMesh::drawPreview()
{
	bool wireframe = false;//flags & WIREFRAME;

	if(wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if(!m_vertices.isCreated() || !m_indices.isCreated())
		return;

	if(m_normals.isCreated() && !wireframe)
	{
		glEnable(GL_LIGHTING);
		glShadeModel(GL_SMOOTH);
		glEnableClientState(GL_NORMAL_ARRAY);
		m_normals.bind();
		glNormalPointer(GL_FLOAT, 0, 0);
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	m_vertices.bind();
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glEnableClientState(GL_INDEX_ARRAY);
	m_indices.bind();
	glIndexPointer(GL_SHORT, 0, 0);

	glDrawElements(GL_TRIANGLES, 3*m_nbFaces, GL_UNSIGNED_SHORT, 0);

	m_indices.release();
	m_vertices.release();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);

	if(wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	debugGL("while rendering" << name() << "preview");
}
#endif
