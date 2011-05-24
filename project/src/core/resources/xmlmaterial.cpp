#include "core/resources/xmlmaterial.h"
#include <QtXml/QtXml>

#include "core/resources/managers.h"

XmlMaterial::XmlMaterial(const QString& name, const QString& path, IResourceFactory* factory) :
	MaterialData(name, path, factory),
	m_diffuse(0.7,0.7,0.7,1.0),
	m_specular(0.7,0.7,0.7,1.0),
	m_ambient(0.7,0.7,0.7,1.0),
	m_emission(0.0,0.0,0.0,0.0),
	m_shininess(0.7)
{}

XmlMaterial::~XmlMaterial()
{
	for(int i=0 ; i<m_uniforms.length() ; i++)
	{
		delete m_uniforms[i];
	}
}

void XmlMaterial::apply()
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  m_diffuse.coords);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specular.coords);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  m_ambient.coords);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, m_emission.coords);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_shininess);

	/*
	for(int i=0 ; i<m_textures.size() ; i++) {
		if(m_textures[i].isValid())
			m_textures[i]->bind(i);
	}
	*/

	if(m_textures.length() > 0)
	{
		glEnable(GL_TEXTURE_2D);
		for(int i = 0 ; i< m_textures.length() ; i++)
		{
			if(m_textures[i].isValid())
				m_textures[i]->bind(i);
		}
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}

	if(m_program.isValid())
	{
		m_program->use();
		int nb_uniforms = m_program->nbUniforms();
		for(int i=0 ; i<nb_uniforms ; i++)
		{
			m_program->setUniform(m_program->uniform(i));
		}
	}
}

void XmlMaterial::unset()
{
	if(m_program.isValid())
		m_program->unset();

	for(int i = 0 ; i< m_textures.length() ; i++)
	{
		if(m_textures[i].isValid())
			m_textures[i]->release(i);
	}
}

bool XmlMaterial::unload()
{
	return false;
}

void XmlMaterialFactory::load(ResourceData* resource)
{
	XmlMaterial* xmlresource = static_cast<XmlMaterial*>(resource);

	QFile file(xmlresource->m_path);

	if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		logWarn("XmlMaterial : Can't open file" << xmlresource->m_path);
		return;
	}

	QDomDocument doc;
	if(!doc.setContent(&file))
	{
		logWarn("XmlMaterial : Can't parse file" << xmlresource->m_path);
		return;
	}

	QDomNodeList materials = doc.elementsByTagName("material");

	if(materials.length() < 1)
	{
		logWarn("XmlMaterial : No material tag in file" << xmlresource->m_path);
		return;
	}

	QDomNodeList nodes = materials.at(0).childNodes();

	for(int i=0 ; i < nodes.length() ; i++)
	{
		QString tag = nodes.at(i).nodeName();

		if(tag == "texture")
		{
			QDomNodeList content = nodes.at(i).childNodes();
			Texture tex = TEXTURE_MANAGER.get(content.at(0).nodeValue());
			if(tex.isValid())
			{
				xmlresource->m_textures.push_back(tex);
			}
			else
			{
				logError("Check texture tag" << nodes.at(i).lineNumber());
			}
		}
		else if(tag == "diffuse")
		{
			QDomNodeList content = nodes.at(i).childNodes();
			QStringList txtlist = content.at(0).nodeValue().split(" ",QString::SkipEmptyParts);
			for(int i=0 ; i<4 && i<txtlist.length() ; i++)
			{
				bool ok;
				float val = txtlist.at(i).toFloat(&ok);
				if(ok)
					xmlresource->m_diffuse[i] = val;
				else
					logWarn("format is 1.2 3.4 5.6 7.8 while reading" << content.at(0).nodeValue() << "in file" << xmlresource->m_path);
			}
		}
		else if(tag == "specular")
		{
			QDomNodeList content = nodes.at(i).childNodes();
			QStringList txtlist = content.at(0).nodeValue().split(" ",QString::SkipEmptyParts);
			for(int i=0 ; i<4 && i<txtlist.length() ; i++)
			{
				bool ok;
				float val = txtlist.at(i).toFloat(&ok);
				if(ok)
					xmlresource->m_specular[i] = val;
				else
					logWarn("format is 1.2 3.4 5.6 7.8 while reading" << content.at(0).nodeValue() << "in file" << xmlresource->m_path);
			}
		}
		else if(tag == "ambient")
		{
			QDomNodeList content = nodes.at(i).childNodes();
			QStringList txtlist = content.at(0).nodeValue().split(" ",QString::SkipEmptyParts);
			for(int i=0 ; i<4 && i<txtlist.length() ; i++)
			{
				bool ok;
				float val = txtlist.at(i).toFloat(&ok);
				if(ok)
					xmlresource->m_ambient[i] = val;
				else
					logWarn("format is 1.2 3.4 5.6 7.8 while reading" << content.at(0).nodeValue() << "in file" << xmlresource->m_path);
			}
		}
		else if(tag == "emission")
		{
			QDomNodeList content = nodes.at(i).childNodes();
			QStringList txtlist = content.at(0).nodeValue().split(" ",QString::SkipEmptyParts);
			for(int i=0 ; i<4 && i<txtlist.length() ; i++)
			{
				bool ok;
				float val = txtlist.at(i).toFloat(&ok);
				if(ok)
					xmlresource->m_emission[i] = val;
				else
					logWarn("format is 1.2 3.4 5.6 7.8 while reading" << content.at(0).nodeValue() << "in file" << xmlresource->m_path);
			}
		}
		else if(tag == "shininess")
		{
			QDomNodeList content = nodes.at(i).childNodes();
			bool ok;
			float val = content.at(0).nodeValue().toFloat(&ok);
			if(ok)
				xmlresource->m_shininess= val;
			else
				logWarn("expected float for shininess" << "in file" << xmlresource->m_path);
		}
		else if(tag == "program")
		{
			QDomNodeList content = nodes.at(i).childNodes();
			QString program_name = content.at(0).nodeValue();

			ShaderProgram prog = SHADER_PROGRAM_MANAGER.get(program_name);

			if(prog.isValid())
			{
				xmlresource->m_program = prog;
			}
			else
			{
				logError("In file" << xmlresource->m_path << "require program" << program_name << "which can't be found");
			}
		}
		else if(tag == "uniform")
		{

		}
		else
		{
			logWarn("In file" << xmlresource->m_path << "Unknown tag" << tag);
		}
	}

	xmlresource->m_state = XmlMaterial::STATE_LOADED;

	logInfo("XmlMaterial loaded" << xmlresource->name());
}

void XmlMaterialFactory::parseFile(const QString &path, QList<ResourceData *> &content, const QHash<QString,QString>&)
{
	QDir dir(path);
	QString name = dir.dirName();

	// Chop extension
	int id = name.lastIndexOf(".");
	if(id > 1)
	{
		name.chop(name.length() - id);
	}

	dir.cdUp();
	if(dir.exists())
	{
		debug( "RESOURCE PARSING" , "XmlMaterial found " << name);
		XmlMaterial* material = new XmlMaterial(name,path,this);
		content.push_back(material);
	}
	else
	{
		debug( "RESOURCE PARSING" , path << " : " << dir << " does not exist");
	}
}