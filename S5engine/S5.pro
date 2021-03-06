#-------------------------------------------------
#
# Project created by QtCreator 2011-02-15T23:51:01
#
#-------------------------------------------------

QT       += core gui opengl xml

TARGET = S5
TEMPLATE = app

SOURCES += \
	src/main.cpp\
	src/core/properties/qtlogo.cpp \
	src/core/scenegraph/node.cpp \
	src/core/properties/propertyset.cpp \
	src/core/scenegraph/scenegraph.cpp \
	src/core/properties/iproperty.cpp \
	src/core/maths/trigo.cpp \
	src/core/maths/frustum.cpp \
	src/core/maths/boundingsphere.cpp \
	src/core/maths/aabb.cpp \
	src/core/properties/iupdatable.cpp \
	src/core/properties/dummyupdatable.cpp \
	src/core/managers/updatemanager.cpp \
	src/core/framework/engine.cpp \
	src/core/framework/glwidget.cpp \
	src/core/framework/window.cpp \
	src/core/managers/rendermanager.cpp \
	src/core/properties/irenderable.cpp \
	src/core/properties/camera.cpp \
	src/core/managers/cameramanager.cpp \
	src/core/properties/meshrenderer.cpp \
	src/core/utils/stb_image.cpp \
	src/core/resources/assimpmesh.cpp \
	src/core/resources/assimpfactory.cpp \
	src/core/resources/iresourcefactory.cpp \
	src/core/resources/stbimage.cpp \
	src/core/inputs/qtkeytranslator.cpp \
	src/core/inputs/inputmanager.cpp \
	src/core/properties/dummycontrolable.cpp \
	src/debug/tools/debugcamera.cpp \
	src/core/properties/soundemitter.cpp \
	src/core/resources/samplefactory.cpp \
	src/core/managers/soundmanager.cpp \
	src/core/properties/grid.cpp \
	src/debug/log/log.cpp \
	src/core/resources/rawmesh.cpp \
	src/core/utils/primitives.cpp \
	src/core/resources/xmlmaterial.cpp \
    src/core/resources/glslshader.cpp \
    src/core/properties/terrainrenderer.cpp \
    src/tools/widgets/soundemitterwidget.cpp \
    src/tools/widgets/terrainwidget.cpp

HEADERS  += \
	include/core/properties/qtlogo.h \
	include/core/utils/parenting.h \
	include/core/utils/parenting.hpp \
	include/core/scenegraph/node.h \
	include/core/scenegraph/scenegraph.h \
	include/core/properties/propertyset.h \
	include/core/properties/iproperty.h \
	include/core/maths/vector4.hpp \
	include/core/maths/vector4.h \
	include/core/maths/vector3.hpp \
	include/core/maths/vector3.h \
	include/core/maths/vector2.hpp \
	include/core/maths/vector2.h \
	include/core/maths/trigo.h \
	include/core/maths/transform.hpp \
	include/core/maths/transform.h \
	include/core/maths/plane.h \
	include/core/maths/matrix4.hpp \
	include/core/maths/matrix4.h \
	include/core/maths/matrix3.hpp \
	include/core/maths/matrix3.h \
	include/core/maths/frustum.h \
	include/core/maths/constants.h \
	include/core/maths/boundingvolume.h \
	include/core/maths/boundingsphere.h \
	include/core/maths/aabb.h \
	include/core/properties/iupdatable.h \
	include/core/properties/dummyupdatable.h \
	include/core/managers/manager.h \
	include/core/managers/manager.hpp \
	include/core/managers/updatemanager.h \
	include/core/managers/managee.h \
	include/core/managers/managee.hpp \
	include/core/framework/engine.h \
	include/core/framework/glwidget.h \
	include/core/framework/window.h \
	include/core/managers/rendermanager.h \
	include/core/properties/irenderable.h \
	include/core/scenegraph/parentofnode.h \
	include/core/properties/camera.h \
	include/core/utils/singleton.h \
	include/core/managers/cameramanager.h \
	include/core/utils/customevents.h \
	include/core/graphics/mesh.h \
	include/core/properties/meshrenderer.h \
	include/core/graphics/texture.h \
	include/core/graphics/material.h \
	include/core/graphics/shader.h \
	include/core/utils/stb_image.h \
	include/core/resources/resource.h \
	include/core/resources/resource.hpp \
	include/core/resources/resourcemanager.h \
	include/core/resources/resourcesmanager.hpp \
	include/core/resources/assimpmesh.h \
	include/core/resources/iresourcefactory.h \
	include/core/resources/stbimage.h \
	include/core/resources/managers.h \
	include/core/resources/assimpfactory.h \
	include/core/inputs/inputmanager.h \
	include/core/inputs/qtkeytranslator.h \
	include/core/properties/dummycontrolable.h \
	include/debug/tools/debugcamera.h \
	include/core/properties/soundemitter.h \
	include/core/resources/samplefactory.h \
	include/core/resources/sample.h \
	include/core/managers/soundmanager.h \
	include/core/properties/grid.h \
	include/debug/log/log.h \
	include/core/resources/rawmesh.h \
	include/core/utils/primitives.h \
	include/core/resources/xmlmaterial.h \
    include/core/resources/glslshader.h \
    include/core/properties/terrainrenderer.h \
    include/tools/widgets/soundemitterwidget.h \
    include/tools/widgets/terrainwidget.h \
    include/core/utils/quadtree.h \
    include/core/utils/quadtree.hpp

INCLUDEPATH += \
	include \
	../dep/assimp/include

macx {
LIBS += \
	-L../dep/assimp/lib -lassimp \
	-lopenal -lalut
}
unix:!macx{
LIBS += \
	-L../dep/assimp/lib -lassimp \
	-lopenal -lalut
}
win32 {
LIBS += \
	../dep/assimp/lib-win32/libassimp.a \
	../dep/openal/lib-win32/alut.lib \
	../dep/openal/lib-win32/OpenAL32.lib

INCLUDEPATH += \
	../dep/openal/include
}


s5debug {
	DEFINES += _DEBUG
}

tools {
	DEFINES += WITH_TOOLS

	SOURCES += \
		src/debug/scenegraphview.cpp \
		src/debug/scenegraphmodel.cpp \
		src/debug/widgets/nodewidget.cpp \
		src/debug/widgets/transformwidget.cpp \
		src/debug/widgets/propertiespanel.cpp \
		src/debug/widgets/propertywidget.cpp \
		src/debug/widgets/foldablegroup.cpp \
		src/debug/widgets/renderwidget.cpp \
		src/debug/widgets/meshrendererwidget.cpp \
		src/debug/widgets/soundemitterwidget.cpp \
		src/debug/debugwindow.cpp

	HEADERS += \
		include/debug/scenegraphmodel.h \
		include/debug/scenegraphview.h \
		include/debug/widgets/nodewidget.h \
		include/debug/widgets/transformwidget.h \
		include/debug/widgets/propertiespanel.h \
		include/debug/widgets/propertywidget.h \
		include/debug/widgets/foldablegroup.h \
		include/debug/widgets/renderwidget.h \
		include/debug/widgets/cameraradiobutton.h \
		include/debug/widgets/meshrendererwidget.h \
		include/debug/widgets/soundemitterwidget.h \
		include/debug/debugwindow.h
}
