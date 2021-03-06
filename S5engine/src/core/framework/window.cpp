#include <QtGui>
#include <QPixmap>
#include <QtConcurrentRun>

#include "core/framework/window.h"
#include "core/framework/glwidget.h"
#include "core/framework/engine.h"

#include "core/log/log.h"

#include <iostream>

AppWindow::AppWindow(Engine* engine)
{
	m_engine = engine;
	m_glWidget = new GLWidget(this);

	setCentralWidget(m_glWidget);

	setWindowTitle(tr("S5 Engine"));
 }

AppWindow::~AppWindow()
{
}

void AppWindow::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Escape) {
		close();
	} else if(e->key() == Qt::Key_F12) {
		m_glWidget->takeScreenshot();
	}
}

void AppWindow::closeEvent(QCloseEvent *evt)
{
	m_engine->stop();
	evt->accept();
}
