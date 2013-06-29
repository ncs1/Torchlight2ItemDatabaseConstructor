#ifndef ITEMDATFILEPARSER_H
#define ITEMDATFILEPARSER_H

#include <QObject>
#include <itemdetailstable.h>
#include <QDir>
#include <QHash>


class ItemDatFileParser : public QObject
{
    Q_OBJECT
public:
    explicit ItemDatFileParser(QString itemDetailsDatabaseFilePath, QObject *parent = 0);
    ~ItemDatFileParser();
    
    void DatFilesFolder(QDir inDatFilesFolder) { mDatFilesFolder = inDatFilesFolder; }

    qint32 NumFilesProcessed() { return mNumFilesProcessed; }

signals:
    void finishedParsingItemsFiles();
    
public slots:
    void ParseItems();

private:
    void ParseItemFileDir(QDir directoryToParse);
    void ParseItemFile(QString filePath, ItemDetails& itemDetails);

    ItemDetailsTable* mItemsTable;
    QDir mDatFilesFolder;

    qint32 mNumFilesProcessed;
    
};

#endif // ITEMDATFILEPARSER_H
