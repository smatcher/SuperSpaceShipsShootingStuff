#include "core/animation/skeletonanimator.h"
#include "core/scenegraph/node.h"

#include "core/log/log.h"

SkeletonAnimator::SkeletonAnimator(Skeleton* skeleton) : IUpdatable("SkeletonAnimator")
{
	m_animationtime = 0.0;
	m_skeleton = skeleton;
}

void SkeletonAnimator::update(double elapsed)
{
	m_animationtime += 10*elapsed;

	Animation* anim = m_skeleton->getAnimation();
	while(m_animationtime > anim->m_duration) {
		m_animationtime -= anim->m_duration;
	}

	for(int i=0 ; i<anim->m_channels.count() ; i++) {
		QMap<QString, QPair<Skeleton::Bone*, Node*> >::iterator link;
		link = m_links.find(anim->m_channels[i].m_name);
		Skeleton::Bone* bone = link.value().first;
		Node* node = link.value().second;

		int j=0;
		while(
			(j+1)<anim->m_channels[i].m_rotation_keys.count() &&
			m_animationtime > anim->m_channels[i].m_rotation_keys[j].time
			) {
			j++;
		}
		Vector4f quat;
		double factor;
		if(j > 0) {
			double old_time = anim->m_channels[i].m_rotation_keys[j-1].time;
			double new_time = anim->m_channels[i].m_rotation_keys[j].time;
			factor = (m_animationtime - old_time) / (new_time - old_time);
		}
		if(j==0) {
			quat = anim->m_channels[i].m_rotation_keys[j].value;
		} else if(j==anim->m_channels[i].m_rotation_keys.count()-1) {
			Vector4f quat1 = anim->m_channels[i].m_rotation_keys[j-1].value;
			Vector4f quat2 = anim->m_channels[i].m_rotation_keys[j].value;
			quat = Vector4f::lerp(factor,quat1,quat2);
		} else {
			Vector4f quat1 = anim->m_channels[i].m_rotation_keys[j-1].value;
			Vector4f quat2 = anim->m_channels[i].m_rotation_keys[j].value;
			quat = Vector4f::lerp(factor,quat1,quat2);
		}
		Matrix3f rot(quat);
		j=0;
		while(
			(j+1)<anim->m_channels[i].m_scaling_keys.count() &&
			m_animationtime > anim->m_channels[i].m_scaling_keys[j].time
			) {
			j++;
		}
		if(j > 0) {
			double old_time = anim->m_channels[i].m_scaling_keys[j-1].time;
			double new_time = anim->m_channels[i].m_scaling_keys[j].time;
			factor = (m_animationtime - old_time) / (new_time - old_time);
		}
		Vector3f scale;
		if(j==0) {
			scale = anim->m_channels[i].m_scaling_keys[j].value;
		} else if(j==anim->m_channels[i].m_scaling_keys.count()-1) {
			Vector3f scale1 = anim->m_channels[i].m_scaling_keys[j-1].value;
			Vector3f scale2 = anim->m_channels[i].m_scaling_keys[j].value;
			scale = Vector3f::lerp(factor,scale1,scale2);
		} else {
			Vector3f scale1 = anim->m_channels[i].m_scaling_keys[j-1].value;
			Vector3f scale2 = anim->m_channels[i].m_scaling_keys[j].value;
			scale = Vector3f::lerp(factor,scale1,scale2);
		}
		j=0;
		while(
			(j+1)<anim->m_channels[i].m_translation_keys.count() &&
			m_animationtime > anim->m_channels[i].m_translation_keys[j].time
			) {
			j++;
		}
		if(j > 0) {
			double old_time = anim->m_channels[i].m_translation_keys[j-1].time;
			double new_time = anim->m_channels[i].m_translation_keys[j].time;
			factor = (m_animationtime - old_time) / (new_time - old_time);
		}
		Vector3f pos;
		if(j==0) {
			pos = anim->m_channels[i].m_translation_keys[j].value;
		} else if(j==anim->m_channels[i].m_translation_keys.count()-1) {
			Vector3f pos1 = anim->m_channels[i].m_translation_keys[j-1].value;
			Vector3f pos2 = anim->m_channels[i].m_translation_keys[j].value;
			pos = Vector3f::lerp(factor,pos1,pos2);
		} else {
			Vector3f pos1 = anim->m_channels[i].m_translation_keys[j-1].value;
			Vector3f pos2 = anim->m_channels[i].m_translation_keys[j].value;
			pos = Vector3f::lerp(factor,pos1,pos2);
		}

		Transformf trans(rot,pos,scale);
		node->copy(trans);
	}
}

void SkeletonAnimator::createLinks()
{
	linkBoneToNode(m_skeleton->getRootBone(), node());
}

void SkeletonAnimator::linkBoneToNode(Skeleton::Bone* bone, Node* node)
{
	for(int i=0 ; i<node->childCount() ; i++) {
		Node* child = node->child(i);

		if(child->getName() == bone->m_name) {
			QPair<Skeleton::Bone*, Node*> pair(bone, child);
			m_links.insert(child->getName(), pair);

			for(int j=0 ; j<bone->m_childrens.count() ; j++) {
				linkBoneToNode(&bone->m_childrens[j], child);
			}
		}
	}
}
