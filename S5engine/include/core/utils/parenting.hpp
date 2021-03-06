// This include should fail since parenting.h includes parenting.hpp
// However QtCreator needs this include in order to provide code assistance.
#include "parenting.h"
#include "core/log/log.h"
/*
 *  ParentOf
 */

template<class Child>
ParentOf<Child>::ParentOf()
{
}

template<class Child>
ParentOf<Child>::~ParentOf()
{
	// Free all sons still linked to the parent
	// The parent destroyer should have unlinked them.
	QMutableMapIterator<QString, Child*> i(m_sons);
	 while (i.hasNext()) {
		 i.next();
		 Child* save = i.value();
		 save->unlinkFromParent(true);	// Unlink the son
		 m_sons.remove(i.key());		// Forget the son
		 delete save;					// Destroy the son
	 }
}

template<class Child>
Child* ParentOf<Child>::child(const QString& childName) const
{
	return m_sons.value(childName,0);
}

template<class Child>
Child* ParentOf<Child>::child(int nb) const
{
	if(m_sons.size() == 0)
		return 0;

	QList<QString> keys = m_sons.keys();
	return child(keys[nb]);
}

template<class Child>
int ParentOf<Child>::childCount() const
{
	return m_sons.count();
}

template<class Child>
void ParentOf<Child>::link(Child* son)
{
	#ifdef DEBUG
		assert(son != 0);
	#endif

	if(son != 0)
	{
		QString name = son->getName();

		if(m_sons.contains(name))
		{
			int cpt=2;
			while(m_sons.contains(name+QString().setNum(cpt))) {cpt++;}
			name = name+QString().setNum(cpt);
			debug("NAME_COLLISIONS","Son named " << son->getName() << " renamed to " << name);
			son->setName(name);
		}
		m_sons.insert(name,son);
		son->setParent((typename Child::ParentPtrType)this);
		onLinked(son);
	}
	else
	{
		logWarn("Trying to add 0 as a son");
	}
}

template<class Child>
Child* ParentOf<Child>::unlink(const QString& childName)
{
	Child* ret = 0;
	typename QMap<QString, Child*>::iterator i = m_sons.find(childName);
	if(i != m_sons.end() && i.key() == childName)
	{
		ret = i.value();
		ret->unlinkFromParent(true);
		m_sons.erase(i);
		onUnlinked(ret);
	}
	return ret;
}

template<class Child>
void ParentOf<Child>::unlinkAll()
{
	QMutableMapIterator<QString, Child*> i(m_sons);
	 while (i.hasNext()) {
		 i.next();
		 Child* save = i.value();
		 save->unlinkFromParent(true);	// Unlink the son
		 m_sons.remove(i.key());		// Forget the son
	 }
}

/*
 *  ChildOf
 */

template<class Parent>
ChildOf<Parent>::ChildOf(const QString& name, Parent* parent)
{
	m_parent = parent;
	m_name = name;

	if(parent != 0)
		parent->link((typename Parent::ChildPtrType)this);
}

template<class Parent>
ChildOf<Parent>::~ChildOf()
{
	// Unlink from the parent to avoid dangling pointers
	unlinkFromParent();
}

template<class Parent>
const QString& ChildOf<Parent>::getName()
{
	return m_name;
}

template<class Parent>
void ChildOf<Parent>::setName(const QString& name)
{
	// Save parent
	Parent* parent = m_parent;
	// Unlink
	if(parent != 0)
		unlinkFromParent();
	// Change name
	m_name = name;
	// Relink
	if(parent != 0)
		parent->link((typename Parent::ChildPtrType)this);
}

template<class Parent>
Parent* ChildOf<Parent>::parent()
{
	return m_parent;
}

template<class Parent>
const Parent* ChildOf<Parent>::parent() const
{
	return m_parent;
}

template<class Parent>
int ChildOf<Parent>::childNumber()
{
	int ret = 0;

	if(m_parent != 0)
	{
		for(int i=0 ; i<m_parent->childCount() ; i++)
		{
			if(m_parent->child(i) == this)
			{
				ret = i;
				break;
			}
		}
	}

	return ret;
}

template<class Parent>
void ChildOf<Parent>::unlinkFromParent(bool justForgetParent)
{
	if(m_parent != 0 && !justForgetParent)
	{
		m_parent->unlink(m_name);
	}
	Parent* oldParent = m_parent;
	m_parent = 0;
	onUnlinked(oldParent);
}

template<class Parent>
void ChildOf<Parent>::setParent(Parent* parent)
{
	if(m_parent != 0) {
		unlinkFromParent();
	}

	m_parent = static_cast<Parent*>(parent);
	onLinked(m_parent);
}
