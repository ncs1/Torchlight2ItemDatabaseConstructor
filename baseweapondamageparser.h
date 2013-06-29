#ifndef BASEWEAPONDAMAGEPARSER_H
#define BASEWEAPONDAMAGEPARSER_H

#include <QObject>
#include <baseweapondamagetable.h>
#include <graphfileparser.h>
#include <QDir>

class BaseWeaponDamageParser : public GraphFileParser
{
    Q_OBJECT
public:
    explicit BaseWeaponDamageParser(QString databaseFilePath, QObject *parent = 0);
    ~BaseWeaponDamageParser();

    void DatFilesFolder(QDir datFilesFolder)
    {
        QDir statsFolder = datFilesFolder;

        if (statsFolder.cd("GRAPHS/STATS"))
            mDatFilesFolder = statsFolder;
    }

    quint32 NumBaseWeaponDamageProcessed() { return mNumBaseWeaponDamageProcessed; }
    
signals:
    void finishedParsingBaseWeaponDamage();
    
public slots:
    void ParseBaseWeaponDamage();

private:
    BaseWeaponDamageTable* mBaseWeaponDamageTable;

    QDir mDatFilesFolder;

    quint32 mNumBaseWeaponDamageProcessed;

    static const QString BaseWeaponDamageFileName;
};

#endif // BASEWEAPONDAMAGEPARSER_H
