#ifndef MAPPINGMODEL_H
#define MAPPINGMODEL_H

#include <map>
#include <QString>

namespace dp {

class MappingModel
{
public:
    void UpdateMapEntry (const char in_key, const QString& in_value, bool in_isPostFix);
    bool FindMapEntry (const char in_key, QString& out_value, bool& out_isPostFix);
    void ClearMap ();
    bool LoadFromFile (const QString& in_fileToLoadFrom);
    bool SaveToFile (const QString& in_fileToSaveTo);


private:
    typedef std::pair<QString, bool> MapValue;
    typedef std::map<char, MapValue> MyMap;

    MyMap m_TheMap;

};

};


#endif // MAPPINGMODEL_H
