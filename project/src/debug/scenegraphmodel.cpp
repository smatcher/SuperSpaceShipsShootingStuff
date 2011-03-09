#include "debug/scenegraphmodel.h"

SceneGraphModel::SceneGraphModel(SceneGraph* item)
{
	m_item = item;
}

SceneGraphModel::~SceneGraphModel()
{
	m_item->clearDebug();
}

Node* SceneGraphModel::getNode(const QModelIndex &index) const
{
	if (index.isValid() && index.internalPointer() != m_item)
	{
		Node *item = static_cast<Node*>(index.internalPointer());
		if (item) return item;
	}
	return NULL;
}

QModelIndex SceneGraphModel::index(int row, int column, const QModelIndex& parent) const
{
	if (parent.isValid() && parent.column() != 0)
		return QModelIndex();

	Node *parentItem = getNode(parent);

	if(parentItem != NULL)
	{
		Node *childItem = parentItem->child(row);
		if (childItem)
			return createIndex(row, column, childItem);
		else
			return QModelIndex();
	}
	else
	{
		Node *childItem = m_item->child(row);
		if (childItem)
			return createIndex(row, column, childItem);
		else
			return QModelIndex();
	}
}

QModelIndex SceneGraphModel::parent(const QModelIndex& index) const
{
	if (!index.isValid())
		return QModelIndex();

	Node *childItem = getNode(index);
	ParentOf<Node> *parentItem = childItem->parent();

	if (parentItem == static_cast<ParentOf<Node>* >(m_item))
		return QModelIndex();

	Node* parentNode = static_cast<Node*>(parentItem);

	return createIndex(((Node*)parentItem)->childNumber(), 0, parentNode);
}

int SceneGraphModel::rowCount(const QModelIndex& parent) const
{
	Node *parentItem = getNode(parent);

	if(parentItem != NULL)
		return parentItem->childCount();
	else
		return m_item->childCount();
}

int SceneGraphModel::columnCount(const QModelIndex&) const
{
	return 1;
}

QVariant SceneGraphModel::data(const QModelIndex& parent, int role) const
{
	if(role == Qt::DisplayRole)
	{
		Node* n = getNode(parent);
		if(n != NULL) return QVariant(n->getName());
	}
	return QVariant();
}
