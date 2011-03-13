#include <QtGui>

#include "debug/widgets/nodewidget.h"
#include "debug/widgets/propertiespanel.h"
#include "core/framework/window.h"
#include "core/framework/glwidget.h"
#include "core/framework/engine.h"

 AppWindow::AppWindow(Engine* engine)
 {
	m_engine = engine;
	m_glWidget = new GLWidget(this);
	m_treeWidget = engine->getScenegraph_TEMPORARY()->getDebugView();
	m_propertiesWidget = new PropertiesPanel();

	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addWidget(m_glWidget);
	mainLayout->addWidget(m_treeWidget);
	mainLayout->addWidget(m_propertiesWidget);
	setLayout(mainLayout);

	m_glWidget->setXRotation(15 * 16);
	m_glWidget->setYRotation(345 * 16);
	m_glWidget->setZRotation(0);

	QObject::connect(m_treeWidget->selectionModel(),
					 SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)),
					 m_propertiesWidget,
					 SLOT(selectionChanged(const QItemSelection&,const QItemSelection&)));

	setWindowTitle(tr("S5 Engine"));
 }

 void AppWindow::keyPressEvent(QKeyEvent *e)
 {
	 if (e->key() == Qt::Key_Escape)
		 close();
	 else
		 QWidget::keyPressEvent(e);
 }

 void AppWindow::closeEvent(QCloseEvent *evt)
 {
	 m_engine->stop();
	 QWidget::closeEvent(evt);
 }