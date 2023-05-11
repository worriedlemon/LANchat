#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include "connection/abstracttypes.h"
#include "exceptions.h"
using namespace Exceptions;

#include <QString>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

class FileManager
{
private:
    FileManager() { };
    FileManager(FileManager& other) { };

public:
    template <class ILoadableType>
    static void SaveToXML(ILoadableType& obj, QString& path)
    {
        QFile xmlFile(path);
        xmlFile.open(QIODevice::WriteOnly);

        QXmlStreamWriter writer(&xmlFile);
        writer.setAutoFormatting(true);
        writer.setAutoFormattingIndent(4);

        QMap<QString, QString> varValueMap = ((Connection::ILoadable*)&obj)->Serialize();
        writer.writeStartElement(varValueMap["ClassName"]);
        varValueMap.remove("ClassName");

        for (const QString& k : varValueMap.keys())
        {
            writer.writeTextElement(k, varValueMap[k]);
        }
        writer.writeEndElement();
    }

    template <class ILoadableType>
    static ILoadableType& LoadFromXML(QString& path)
    {
        QFile xmlFile(path);
        if (!xmlFile.open(QIODevice::ReadOnly)) throw CommonException("File doesn't exist");
        QXmlStreamReader reader(&xmlFile);

        reader.readNextStartElement();
        QString className = reader.name().toString();
        QMap<QString, QString> varValueMap { { "ClassName", className } };
        Connection::ILoadable* loaded = INITIALIZE_EMPTY(className);

        while (reader.readNextStartElement())
        {
            QString key = reader.name().toString(), value = reader.readElementText();
            varValueMap.insert(key, value);
        }

        loaded->Deserialize(varValueMap);

        return *(ILoadableType*)loaded;
    }
};

#endif // FILEMANAGER_H
