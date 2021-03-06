#include "tools/widgets/propertiespanel.h"
#include <QHBoxLayout>

PropertiesPanel::PropertiesPanel()
{
	m_widget = 0;

	setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
}

PropertiesPanel::~PropertiesPanel()
{
}

void PropertiesPanel::selectionChanged(const QItemSelection& selected, const QItemSelection&)
{
	QModelIndexList sel = selected.indexes();

	Node* n = static_cast<Node*>(sel.first().internalPointer());

	NodeWidget* widget = n->getWidget();

	if(m_widget != widget)
	{
		if(m_widget != 0)
		{
			delete m_widget;
		}

		m_widget = widget;
		setWidget(m_widget);
	}
}

QSize PropertiesPanel::sizeHint() const
{
	return QSize(240, 300);
}

void PropertiesPanel::updateData()
{
	if(m_widget != 0)
		m_widget->updateData();
}
