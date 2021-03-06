#ifndef QUASQLITESERIALIZER_H
#define QUASQLITESERIALIZER_H

#include <QUaServer>
#include <QSqlDatabase>
#include <QSqlQuery>

class QUaSqliteSerializer
{
public:
    QUaSqliteSerializer();

	// set sqlite database to read from or write to
	QString sqliteDbName() const;
	bool setSqliteDbName(
		const QString& strSqliteDbName,
		QQueue<QUaLog>& logOut
	);

	// optional API for QUaNode::serialize
	bool serializeStart(QQueue<QUaLog>& logOut);

	// optional API for QUaNode::serialize
	bool serializeEnd(QQueue<QUaLog>& logOut);

	// required API for QUaNode::serialize
	bool writeInstance(
		const QUaNodeId& nodeId,
		const QString& typeName,
		const QMap<QString, QVariant>& attrs,
		const QList<QUaForwardReference>& forwardRefs,
		QQueue<QUaLog>& logOut
	);

	// optional API for QUaNode::deserialize
	bool deserializeStart(QQueue<QUaLog>& logOut);

	// optional API for QUaNode::deserialize
	bool deserializeEnd(QQueue<QUaLog>& logOut);

	// required API for QUaNode::deserialize
	bool readInstance(
		const QUaNodeId& nodeId,
		const QString& typeName,
		QMap<QString, QVariant>& attrs,
		QList<QUaForwardReference>& forwardRefs,
		QQueue<QUaLog>& logOut
	);

private:
	QString m_strSqliteDbName;
	QHash<QString, QSqlQuery> m_prepStmts;
	// get database handle, creates it if not already
	bool getOpenedDatabase(
		QSqlDatabase &db, 
		QQueue<QUaLog>& logOut
	);
	// check table exists
	bool tableExists(
		QSqlDatabase& db,
		const QString& strTableName, 
		bool& tableExists,
		QQueue<QUaLog>& logOut
	);
	// create nodes table
	bool createNodesTable(
		QSqlDatabase& db, 
		QQueue<QUaLog>& logOut
	);
	// create references table
	bool createReferencesTable(
		QSqlDatabase& db, 
		QQueue<QUaLog>& logOut
	);
	// create type table
	bool createTypeTable(
		QSqlDatabase& db,
		const QString& typeName, 
		const QMap<QString, QVariant>& attrs,
		QQueue<QUaLog>& logOut
	);
	// check if node already exists in type table
	bool nodeIdInTypeTable(
		QSqlDatabase& db,
		const QString& typeName, 
		const QUaNodeId& nodeId,
		bool& nodeExists,
		qint32& nodeKey,
		QQueue<QUaLog>& logOut
	);
	// insert new node, return unique key
	bool insertNewNode(
		QSqlDatabase& db, 
		const QUaNodeId& nodeId,
		qint32& nodeKey, 
		QQueue<QUaLog>& logOut
	);
	// insert new instance, return unique key
	bool insertNewInstance(
		QSqlDatabase& db,
		const QString& typeName, 
		const qint32& nodeKey,
		const QMap<QString, QVariant>& attrs,
		QQueue<QUaLog>& logOut
	);
	// add references to db
	bool addReferences(
		QSqlDatabase& db,
		const qint32& nodeKey,
		const QList<QUaForwardReference>& forwardRefs,
		QQueue<QUaLog>& logOut
	);
	// remove references from db
	bool removeReferences(
		QSqlDatabase& db,
		const qint32& nodeKey,
		const QList<QUaForwardReference>& forwardRefs,
		QQueue<QUaLog>& logOut
	);
	// update attributes of existing instance
	bool updateInstance(
		QSqlDatabase& db,
		const QString& typeName,
		const qint32& nodeKey,
		const QMap<QString, QVariant>& attrs,
		QQueue<QUaLog>& logOut
	);
	// get attributes by type name and node key
	bool nodeAttributes(
		QSqlDatabase& db,
		const QString& typeName,
		const QUaNodeId& nodeId,
		QMap<QString, QVariant>& attrs,
		QQueue<QUaLog>& logOut
	);
	// get references by node key
	bool nodeReferences(
		QSqlDatabase& db,
		const QUaNodeId& nodeId,
		QList<QUaForwardReference>& forwardRefs,
		QQueue<QUaLog>& logOut
	);

	// return SQL type in string form, for given Qt type (only QUaServer supported types)
	static QHash<int, QString> m_hashTypes;
	static const QString QtTypeToSqlType(const QMetaType::Type& qtType);
};

#endif // QUASQLITESERIALIZER_H
