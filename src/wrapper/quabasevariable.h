#ifndef QUABASEVARIABLE_H
#define QUABASEVARIABLE_H

#include <QUaNode>

/*
typedef struct {                          // UA_VariableTypeAttributes_default
	// Node Attributes
	UA_UInt32        specifiedAttributes; // 0,
	UA_LocalizedText displayName;         // {{0, NULL}, {0, NULL}},
	UA_LocalizedText description;         // {{0, NULL}, {0, NULL}},
	UA_UInt32        writeMask;           // 0,
	UA_UInt32        userWriteMask;       // 0,
	// Variable Type Attributes
	UA_Variant       value;               // {NULL, UA_VARIANT_DATA, 0, NULL, 0, NULL},
	UA_NodeId        dataType;            // {0, UA_NODEIDTYPE_NUMERIC, {UA_NS0ID_BASEDATATYPE}},
	UA_Int32         valueRank;           // UA_VALUERANK_ANY,
	size_t           arrayDimensionsSize; // 0,
	UA_UInt32        *arrayDimensions;    // NULL,
	UA_Boolean       isAbstract;          // false
} UA_VariableTypeAttributes;

typedef struct {                              // UA_VariableAttributes_default
	// Node Attributes
	UA_UInt32        specifiedAttributes;     // 0,
	UA_LocalizedText displayName;             // {{0, NULL}, {0, NULL}},
	UA_LocalizedText description;             // {{0, NULL}, {0, NULL}},
	UA_UInt32        writeMask;               // 0,
	UA_UInt32        userWriteMask;           // 0,
	// Variable Attributes
	UA_Variant       value;                   // {NULL, UA_VARIANT_DATA, 0, NULL, 0, NULL},
	UA_NodeId        dataType;                // {0, UA_NODEIDTYPE_NUMERIC, {UA_NS0ID_BASEDATATYPE}},
	UA_Int32         valueRank;               // UA_VALUERANK_ANY,
	size_t           arrayDimensionsSize;     // 0,
	UA_UInt32        *arrayDimensions;        // NULL,
	UA_Byte          accessLevel;             // UA_ACCESSLEVELMASK_READ,
	UA_Byte          userAccessLevel;         // 0,
	UA_Double        minimumSamplingInterval; // 0.0,
	UA_Boolean       historizing;             // false
} UA_VariableAttributes;


 * Access Level Masks
 * ------------------
 * The access level to a node is given by the following constants that are ANDed
 * with the overall access level.

#define UA_ACCESSLEVELMASK_READ           (0x01<<0)
#define UA_ACCESSLEVELMASK_WRITE          (0x01<<1)
#define UA_ACCESSLEVELMASK_HISTORYREAD    (0x01<<2)
#define UA_ACCESSLEVELMASK_HISTORYWRITE   (0x01<<3)
#define UA_ACCESSLEVELMASK_SEMANTICCHANGE (0x01<<4)
#define UA_ACCESSLEVELMASK_STATUSWRITE    (0x01<<5)
#define UA_ACCESSLEVELMASK_TIMESTAMPWRITE (0x01<<6)

*/

// Part 5 - 7.2 : BaseVariableType
/*
The BaseVariableType is the abstract base type for all other VariableTypes. 
However, only the PropertyType and the BaseDataVariableType directly inherit from this type.
*/

union QUaAccessLevel
{
	QUaAccessLevel()
	{
		// read only by default
		bits.bRead           = true;
		bits.bWrite			 = false;
		bits.bHistoryRead	 = false;
		bits.bHistoryWrite	 = false;
		bits.bSemanticChange = false;
		bits.bStatusWrite	 = false;
		bits.bTimestampWrite = false;
	};
	struct bit_map {
		bool bRead           : 1; // UA_ACCESSLEVELMASK_READ
		bool bWrite          : 1; // UA_ACCESSLEVELMASK_WRITE
		bool bHistoryRead    : 1; // UA_ACCESSLEVELMASK_HISTORYREAD
		bool bHistoryWrite   : 1; // UA_ACCESSLEVELMASK_HISTORYWRITE
		bool bSemanticChange : 1; // UA_ACCESSLEVELMASK_SEMANTICCHANGE
		bool bStatusWrite    : 1; // UA_ACCESSLEVELMASK_STATUSWRITE
		bool bTimestampWrite : 1; // UA_ACCESSLEVELMASK_TIMESTAMPWRITE
	} bits;
	quint8 intValue;
};

class QUaBaseVariable : public QUaNode
{
	Q_OBJECT

	// Variable Attributes

	Q_PROPERTY(QVariant          value               READ value               WRITE setValue           NOTIFY valueChanged          )
	Q_PROPERTY(QMetaType::Type   dataType            READ dataType            WRITE setDataType        NOTIFY dataTypeChanged       )
	Q_PROPERTY(qint32            valueRank           READ valueRank          /*NOTE : Read-only      NOTIFY valueRankChanged      */)
	Q_PROPERTY(QVector<quint32>  arrayDimensions     READ arrayDimensions    /*NOTE : Read-only      NOTIFY arrayDimensionsChanged*/)
	Q_PROPERTY(quint8            accessLevel         READ accessLevel         WRITE setAccessLevel     NOTIFY accessLevelChanged    )

	// Cannot be written from the server, as they are specific to the different users and set by the access control callback :
	// Q_PROPERTY(quint32 userAccessLevel READ get_userAccessLevel)	

	Q_PROPERTY(double minimumSamplingInterval READ minimumSamplingInterval WRITE setMinimumSamplingInterval)

	// Historizing is currently unsupported
	Q_PROPERTY(bool historizing READ historizing)

public:
	explicit QUaBaseVariable(QUaServer *server);
	
	// If the new value is the same dataType or convertible to the old dataType, the old dataType is preserved
	// If the new value has a new type different and not convertible to the old dataType, the dataType is updated
	// Use QVariant::fromValue or use casting to force a dataType
	QVariant          value() const;
	void              setValue(const QVariant &value);
	// If there is no old value, a default value is assigned with the new dataType
	// If an old value exists and is convertible to the new dataType then the value is converted
	// If the old value is not convertible, then a default value is assigned with the new dataType and the old value is lost
	QMetaType::Type   dataType() const;
	void              setDataType(const QMetaType::Type &dataType);
	template<typename T>
	void              setDataTypeEnum();
	void              setDataTypeEnum(const QMetaEnum &metaEnum);
	// Read-only, values set automatically when calling setValue
	qint32            valueRank() const;
	QVector<quint32>  arrayDimensions() const; // includes arrayDimensionsSize
	// 
	quint8            accessLevel() const;
	void              setAccessLevel(const quint8 &accessLevel);
	// 
	double            minimumSamplingInterval() const;
	void              setMinimumSamplingInterval(const double &minimumSamplingInterval);
	// 
	bool              historizing() const;

	// helpers

	bool              isWritable() const;
	void              setIsWritable(const bool &isWritable);

	// static helpers

	static qint32            GetValueRankFromQVariant      (const QVariant &varValue);
	static QVector<quint32>  GetArrayDimensionsFromQVariant(const QVariant &varValue);
	
signals:
	void valueChanged          (const QVariant         &value          );
	void dataTypeChanged       (const QMetaType::Type  &dataType       );
	void accessLevelChanged    (const quint8           &accessLevel    );

private:
	static void onWrite(UA_Server             *server, 
		                const UA_NodeId       *sessionId,
		                void                  *sessionContext, 
		                const UA_NodeId       *nodeId,
		                void                  *nodeContext, 
		                const UA_NumericRange *range,
		                const UA_DataValue    *data);

};

#endif // QUABASEVARIABLE_H