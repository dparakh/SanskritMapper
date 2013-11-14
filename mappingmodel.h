#ifndef MAPPINGMODEL_H
#define MAPPINGMODEL_H

#include <map>
#include <QString>

namespace dp {

class MappingModel
{
public:
    void UpdateMapEntry (const char in_key, const QString& in_value, int in_location);
    bool FindMapEntry (const char in_key, QString& out_value, int& out_location);
    void ClearMap ();
    bool LoadFromFile (const QString& in_fileToLoadFrom);
    bool SaveToFile (const QString& in_fileToSaveTo);

    static const int kLocationPrefix = -1;
    static const int kLocationPostFix = 1;
    static const int kLocationInPlace = 0;

private:
    typedef std::pair<QString, int> MapValue;
    typedef std::map<char, MapValue> MyMap;

    MyMap m_TheMap;

};

};


#endif // MAPPINGMODEL_H
