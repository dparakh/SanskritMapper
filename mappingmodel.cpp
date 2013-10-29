#include "mappingmodel.h"
#include <iostream>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>

using namespace std;

//The keys for each JSon object
static const char kSourceChar[] = "SourceChar";
static const char kMappedText[] = "MappedText";
static const char kIsPostFix[] = "IsPostFix";


void dp::MappingModel::UpdateMapEntry(const char in_key, const QString &in_value, bool in_isPostFix)
{
    m_TheMap[in_key] = make_pair(in_value, in_isPostFix);
    //cout << "k = " << in_key << ", v = " << in_value.toUtf8().data() << endl;
}

bool dp::MappingModel::FindMapEntry(const char in_key, QString &out_value, bool &out_isPostFix)
{
    bool retVal = false;
    MyMap::const_iterator mapIter = m_TheMap.find(in_key);
    if (mapIter != m_TheMap.end())
    {
        out_value = mapIter->second.first;
        out_isPostFix = mapIter->second.second;
        retVal = true;
    }

    return retVal;
}

void dp::MappingModel::ClearMap()
{
    m_TheMap.clear();
}

bool dp::MappingModel::LoadFromFile(const QString & in_fileToLoadFrom)
{
    bool retVal = false;

    QFile file(in_fileToLoadFrom);
    if (file.open(QIODevice::ReadOnly))
    {
        QJsonParseError parseError;
        QJsonDocument theDoc (QJsonDocument::fromJson(file.readAll(), &parseError));
        if (parseError.error == QJsonParseError::NoError)
        {
            m_TheMap.clear();
            const QJsonArray arrayOfEncodings (theDoc.array());
            for (QJsonArray::const_iterator arrayIter(arrayOfEncodings.begin()); arrayIter != arrayOfEncodings.end(); ++arrayIter)
            {
                if ((*arrayIter).isObject())
                {
                    const QJsonObject mappingObject((*arrayIter).toObject());
                    m_TheMap[mappingObject.value(kSourceChar).toDouble()] = make_pair(mappingObject.value(kMappedText).toString(),
                                                                                      mappingObject.value(kIsPostFix).toBool());
                }
            }

        }
        file.close();
    }

    return retVal;
}


bool dp::MappingModel::SaveToFile(const QString & in_fileToSaveTo)
{
    bool retVal = false;
    QJsonArray arrayOfEncodings;
    for (MyMap::const_iterator mapIter (m_TheMap.begin()); mapIter != m_TheMap.end(); ++mapIter)
    {
        QJsonObject mappingObject;
        mappingObject.insert(kSourceChar, QJsonValue(mapIter->first));
        mappingObject.insert(kMappedText, QJsonValue(mapIter->second.first));
        mappingObject.insert(kIsPostFix, QJsonValue(mapIter->second.second));
        arrayOfEncodings.append(mappingObject);
    }

    QJsonDocument theDoc;
    theDoc.setArray(arrayOfEncodings);
    QByteArray jsonData (theDoc.toJson());
    QFile file(in_fileToSaveTo);
    file.open(QIODevice::WriteOnly);
    file.write(jsonData);
    file.close();

    return retVal;
}
