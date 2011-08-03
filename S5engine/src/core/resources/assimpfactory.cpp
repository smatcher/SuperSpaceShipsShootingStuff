#include "core/resources/assimpfactory.h"
#include "core/resources/assimpmesh.h"
#include <aiPostProcess.h>
#include <aiScene.h>

#include "core/resources/managers.h"

#include "core/log/log.h"

#include "core/animation/skeleton.h"

#include <QVector>
#include <QMap>

#include <core/managers/physicsmanager.h>

void AssimpFactory::load(ResourceData* resource)
{
	AssimpMesh* meshresource = static_cast<AssimpMesh*>(resource);

	if(resource->state() == AssimpMesh::STATE_UNLOADED)
	{
		const aiScene* scene = m_importer.ReadFile(resource->path().toLocal8Bit().data(), aiProcessPreset_TargetRealtime_Quality);
		QMap<QString,aiNode*> bones;

		if(scene != NULL)
		{
			unsigned int numVertices = 0;
			for(unsigned i=0 ; i<scene->mNumMeshes ; i++)
			{
				const aiMesh* aimesh = scene->mMeshes[i];
				for(int j=0 ; j<aimesh->mNumBones ; j++) {
					QString bone_name = aimesh->mBones[j]->mName.data;
					if(!bones.contains(bone_name)) {
						aiNode* bone = findBone(bone_name, scene->mRootNode);
						if(bone != NULL) {
							bones.insert(bone_name, findBone(bone_name, bone));
						}
					}
				}
				numVertices += aimesh->mNumVertices;
				meshresource->m_submeshes.push_back(new AssimpMesh::Submesh(aimesh));
			}

			GLfloat* array = new GLfloat[3 * numVertices]();
			for(unsigned int i=0 ; i<meshresource->m_submeshes.size() ; i++) {
				for(unsigned int j=0 ; j < meshresource->m_submeshes[i]->m_mesh->mNumVertices ; j++) {
					memcpy(&(array[3*j]),&meshresource->m_submeshes[i]->m_mesh->mVertices[j].x,3 * sizeof(GLfloat));
				}
			}
			PHYSICS_MANAGER.buildConvexCollider(resource->name(),array,numVertices);

			if(bones.size() != 0) {
				meshresource->m_skeleton = buildSkeleton(scene->mRootNode);

				for(int i=0 ; i< scene->mNumAnimations ; i++) {
					Animation anim;
					anim.m_name = scene->mAnimations[i]->mName.data;
					anim.m_duration = scene->mAnimations[i]->mDuration;

					for(int j=0 ; j<scene->mAnimations[i]->mNumChannels ; j++) {
						aiNodeAnim* ai_channel = scene->mAnimations[i]->mChannels[j];
						Animation::AnimChannel chanel;
						chanel.m_name = ai_channel->mNodeName.data;
						for(int k=0 ; k<ai_channel->mNumPositionKeys ; k++) {
							Animation::AnimKey<Vector3f> key;
							key.time = ai_channel->mPositionKeys[k].mTime;
							key.value.x = ai_channel->mPositionKeys[k].mValue.x;
							key.value.y = ai_channel->mPositionKeys[k].mValue.y;
							key.value.z = ai_channel->mPositionKeys[k].mValue.z;
							chanel.m_translation_keys.push_back(key);
						}
						for(int k=0 ; k<ai_channel->mNumScalingKeys ; k++) {
							Animation::AnimKey<Vector3f> key;
							key.time = ai_channel->mScalingKeys[k].mTime;
							key.value.x = ai_channel->mScalingKeys[k].mValue.x;
							key.value.y = ai_channel->mScalingKeys[k].mValue.y;
							key.value.z = ai_channel->mScalingKeys[k].mValue.z;
							chanel.m_scaling_keys.push_back(key);
						}
						for(int k=0 ; k<ai_channel->mNumRotationKeys ; k++) {
							Animation::AnimKey<Vector4f> key;
							key.time = ai_channel->mRotationKeys[k].mTime;
							key.value.x = ai_channel->mRotationKeys[k].mValue.x;
							key.value.y = ai_channel->mRotationKeys[k].mValue.y;
							key.value.z = ai_channel->mRotationKeys[k].mValue.z;
							key.value.w = ai_channel->mRotationKeys[k].mValue.w;
							chanel.m_rotation_keys.push_back(key);
						}
						anim.m_channels.push_back(chanel);
					}
					meshresource->m_skeleton->TMP_animations.push_back(anim);
				}
			}

			meshresource->buildVBOs();
			meshresource->m_state = ResourceData::STATE_LOADED;
		}
		else
		{
			logError( "Assimp could not read " << resource->path());
		}
	}
}

void AssimpFactory::parseFile(const QString& path, QList<ResourceData*>& content, const QHash<QString,QString>&)
{
	QDir dir(path);
	QString name = dir.dirName();

	// Chop extension
	int id = name.lastIndexOf(".");
	if(id > 1)
	{
		QString extension = name.right(id).toLower();
		if(extension != "x"
		&& extension != "blend"
		&& extension != "dae") {
			name.chop(name.length() - id);
		} else {
			return; // Not an assimp model
		}
	} else {
		return; // No extension
	}

	dir.cdUp();
	if(dir.exists())
	{
		debug( "RESOURCE PARSING" , "AssimpMesh found " << name);
		AssimpMesh* mesh = new AssimpMesh(name,path,this);
		content.push_back(mesh);
	}
	else
	{
		debug( "RESOURCE PARSING" , path << " : " << dir << " does not exist");
	}
}

QString AssimpFactory::findMeshName(unsigned int meshNumber, aiNode* node)
{
	for(unsigned int i=0 ; i < node->mNumMeshes ; i++) {
		if(node->mMeshes[i] == meshNumber) {
			return QString(node->mName.data);
		}
	}

	for(unsigned int i=0 ; i < node->mNumChildren ; i++) {
		QString ret = findMeshName(meshNumber, node->mChildren[i]);
		if(ret.length() > 0) {
			return ret;
		}
	}

	return "";
}

aiNode* AssimpFactory::findBone(QString name, aiNode* node)
{
	if(node->mName.data == name) {
		return node;
	}

	for(unsigned int i=0 ; i < node->mNumChildren ; i++) {
		aiNode* ret = findBone(name, node->mChildren[i]);
		if(ret != NULL) {
			return ret;
		}
	}

	return NULL;
}

void buildBone(Skeleton::Bone* bone, aiNode* node)
{
	bone->m_name = node->mName.data;
	aiVector3D scaling;
	aiQuaternion rotation;
	aiVector3D position;
	node->mTransformation.Decompose(scaling, rotation, position);
	aiMatrix3x3 rotmat = rotation.GetMatrix();

	Matrix3f _rotation; // Assimp is row major, S5 is column major
	_rotation[0] = rotmat.a1;
	_rotation[1] = rotmat.b1;
	_rotation[2] = rotmat.c1;
	_rotation[3] = rotmat.a2;
	_rotation[4] = rotmat.b2;
	_rotation[5] = rotmat.c2;
	_rotation[6] = rotmat.a3;
	_rotation[7] = rotmat.b3;
	_rotation[8] = rotmat.c3;

	Vector3f _position(position.x, position.y, position.z);
	Vector3f _scaling(scaling.x, scaling.y, scaling.z);

	bone->m_bind_pose = Transformf(_rotation, _position, _scaling);

	for(int i=0 ; i< node->mNumChildren ; i++) {
		Skeleton::Bone b;
		buildBone(&b,node->mChildren[i]);
		bone->m_childrens.push_back(b);
	}
}

Skeleton* AssimpFactory::buildSkeleton(aiNode* node)
{
	Skeleton* ret = new Skeleton();
	buildBone(&ret->m_root_bone,node);
	return ret;
}
