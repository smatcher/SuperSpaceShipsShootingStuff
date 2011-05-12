#include <QtOpenGL>
#include "core/resources/rawmesh.h"

RawMesh::RawMesh(const QString &name, const QString &path, IResourceFactory *factory) :
	MeshData(name,path,factory),
	m_vertices(),
	m_normals(),
	m_colors(),
	m_texcoords(),
	m_indices(QGLBuffer::IndexBuffer),
	m_nbFaces(0)
{
}

bool RawMesh::unload()
{
	m_vertices.destroy();
	m_colors.destroy();
	m_texcoords.destroy();
	m_normals.destroy();
	m_indices.destroy();

	m_nbFaces = 0;

	return true;
}

void RawMesh::draw()
{
	if(!m_vertices.isCreated() || !m_indices.isCreated())
		return;

	if(m_texcoords.isCreated())
	{
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		m_texcoords.bind();
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);
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
		glColorPointer(4, GL_FLOAT, 0, NULL);
	}
	else
	{
		glDisable(GL_COLOR_MATERIAL);
	}

	if(m_normals.isCreated())
	{
		glEnable(GL_LIGHTING);
		glShadeModel(GL_SMOOTH);
		glEnableClientState(GL_NORMAL_ARRAY);
		m_normals.bind();
		glNormalPointer(GL_FLOAT, 0, NULL);
	}
	else
	{
		glDisable(GL_LIGHTING);
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	m_vertices.bind();
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glEnableClientState(GL_INDEX_ARRAY);
	m_indices.bind();
	glIndexPointer(GL_SHORT, 0, NULL);

	glDrawElements(GL_TRIANGLES, 3*m_nbFaces, GL_UNSIGNED_SHORT, NULL);

	// Unbind buffers (binding to a non created buffer does the trick)
	//QGLBuffer().bind();
	//QGLBuffer(QGLBuffer::IndexBuffer).bind();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);

	debugGL("while rendering" << name());
};
