#include "mappingmodel.h"
#include <iostream>

using namespace std;



void dp::MappingModel::UpdateMapEntry(const char in_key, const QString &in_value, bool in_isPostFix)
{
    m_TheMap[in_key] = make_pair(in_value, in_isPostFix);
    cout << "k = " << in_key << ", v = " << in_value.toUtf8().data() << endl;
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

bool dp::MappingModel::LoadFromFile(const QString & /*in_fileToLoadFrom*/ )
{
    bool retVal = false;
    return retVal;
}

bool dp::MappingModel::SaveToFile(const QString & /*in_fileToSaveTo*/ )
{
    bool retVal = false;
    return retVal;
}
