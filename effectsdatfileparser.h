#ifndef EFFECTSDATFILEPARSER_H
#define EFFECTSDATFILEPARSER_H

#include <effectstable.h>

#include <QObject>
#include <QDir>

class EffectsDatFileParser : public QObject
{
    Q_OBJECT
public:
    explicit EffectsDatFileParser(QString itemDetailsDatabaseFilePath, QObject* parent = 0);
    ~EffectsDatFileParser();

    void DatFilesFolder(QDir inDatFilesFolder) { mDatFilesFolder = inDatFilesFolder; }

    qint32 NumEffectsProcessed() { return mNumEffectsProcessed; }

signals:
    void effectProcessed(qint32);
    void finishedParsingEffects();

public slots:
    void ParseEffects();


private:
    EffectsTable* mEffectsTable;
    QDir mDatFilesFolder;

    qint32 mNumEffectsProcessed;

    static const QString EffectsFileName;

};

#endif // EFFECTSDATFILEPARSER_H
