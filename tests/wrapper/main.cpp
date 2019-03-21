#include <QCoreApplication>
#include <QDebug>
#include <QDateTime>
#include <QTimer>

#include <QUaServer>

#include "mynewobjecttype.h"
#include "mynewvariabletype.h"

//void printChildren(QUaNode * child, const QString strSpacer = "")
//{
//	Q_CHECK_PTR(child);
//	qDebug() << strSpacer + "-->" << child->displayName()/* << QString("(%1)").arg(child->objectName())*/;
//	for (int i = 0; i < child->children().count(); i++)
//	{
//		printChildren(static_cast<QUaNode*>(child->children().at(i)), strSpacer + "--");
//	}
//}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

	QUaServer server;
	auto objsFolder = server.objectsFolder();

	auto var1 = objsFolder->addBaseDataVariable();
	var1->setDisplayName("var1");
	var1->setDataTypeEnum<MyNewObjectType::TestEnum>();
	var1->setValue(MyNewObjectType::TestEnum::FOUR);
	var1->setIsWritable(true);
	QObject::connect(var1, &QUaBaseVariable::valueChanged, [](const QVariant &value) {
		qDebug() << "var1 value changed to" << value;
	});

/*
	// instances

	auto varBaseData = objsFolder->addBaseDataVariable();
	varBaseData->setBrowseName("QUaBaseDataVariable");
	varBaseData->setDisplayName("QUaBaseDataVariable");

	auto varProp = objsFolder->addProperty();
	varProp->setBrowseName("QUaProperty");
	varProp->setDisplayName("QUaProperty");

	auto objBase = objsFolder->addBaseObject();
	objBase->setBrowseName("QUaBaseObject");
	objBase->setDisplayName("QUaBaseObject");

	auto objFolder = objsFolder->addFolderObject();
	objFolder->setBrowseName("QUaFolderObject");
	objFolder->setDisplayName("QUaFolderObject");

	// methods

	objsFolder->addMethod("method1", []() {
		qDebug() << "method1";
	});
	objsFolder->addMethod("method2", []() {
		qDebug() << "method2";
		return "method2";
	});
	objsFolder->addMethod("method3", [](int x, int y) {
		qDebug() << "method3";
		return (double)x / (double)y;
	});
	objsFolder->addMethod("method4", [](int x, double y, QString str) {
		qDebug() << "method4";
		return QString("%1, %2, %3").arg(x).arg(y).arg(str);
	});	

	// custom types

	auto newobjTypeInstance = objsFolder->addChild<MyNewObjectSubType>();
	newobjTypeInstance->setDisplayName("Test");

	auto otherNewobjTypeInstance = objsFolder->addChild<MyOtherNewObjectType>();
	otherNewobjTypeInstance->setBrowseName("MyOtherNewObjectType");
	otherNewobjTypeInstance->setDisplayName("MyOtherNewObjectType");

	auto newVarTypeInstance = objsFolder->addChild<MyNewVariableType>();
	newVarTypeInstance->setBrowseName("MyNewVariableType");
	newVarTypeInstance->setDisplayName("MyNewVariableType");
	newVarTypeInstance->setValue(1.2345);

	newVarTypeInstance->myOtherTwo()->myVarTwo()->setValue("foo");
	newVarTypeInstance->myOtherVar()->myVarTwo()->setValue("bar");

	auto newVarTypeInstance2 = objsFolder->addChild<MyNewVariableType>();
	newVarTypeInstance2->setBrowseName("MyNewVariableType2");
	newVarTypeInstance2->setDisplayName("MyNewVariableType2");
	newVarTypeInstance2->setValue("xxx");

	auto newVarSubTypeInstance = objsFolder->addChild<MyNewVariableSubType>();
	newVarSubTypeInstance->setBrowseName("MyNewVariableSubType");
	newVarSubTypeInstance->setDisplayName("MyNewVariableSubType");
	newVarSubTypeInstance->setValue(666);

	auto newVarSubSubTypeInstance = objsFolder->addChild<MyNewVariableSubSubType>();
	newVarSubSubTypeInstance->setBrowseName("MyNewVariableSubSubType");
	newVarSubSubTypeInstance->setDisplayName("MyNewVariableSubSubType");
	newVarSubSubTypeInstance->setValue(QDateTime::currentDateTime());	

	// delete

	objsFolder->addMethod("delete_node", [&newVarTypeInstance]() {
		if (newVarTypeInstance)
		{
			delete newVarTypeInstance;
			newVarTypeInstance = nullptr;
			return "Deleted";
		}
		return "Already Deleted";
	});

	//// print tree
	//printChildren(objsFolder);
*/

	// NOTE : runs in main thread within Qt's event loop
	server.start();

    return a.exec();
}
