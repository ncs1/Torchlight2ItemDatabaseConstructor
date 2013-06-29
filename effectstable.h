#ifndef EFFECTSTABLE_H
#define EFFECTSTABLE_H

#include <sqlitetable.h>

struct Effect;

class EffectsTable : public SqliteTable
{
public:
    EffectsTable(QString databaseName);

    void DatabasePath(QString databaseName);

    qint64 AddEffect(const Effect& inEffect);

private:
    static const QString EffectsTableName;
    static const QString EffectId;
    static const QString EffectNumber;
    static const QString EffectRawName;
    static const QString Name;
    static const QString GoodDescription;
    static const QString GoodDescriptionOverTime;
    static const QString BadDescription;
    static const QString BadDescriptionOverTime;
    static const QString DisplayPrecision;
    static const QString DisplayPrecisionMaxVal;
};

struct Effect
{
    Effect()
    {
        Clear();
    }

    void Clear()
    {
        effectId = 0;
        effectNumber = 0;
        rawName = "";
        name = "";
        goodDescription = "";
        goodDescriptionOverTime = "";
        badDescription = "";
        badDescriptionOverTime = "";
        displayPrecision = 0;
        displayPrecisionMaxVal = 0;
    }

    qint64 effectId;
    qint32 effectNumber;
    QString rawName;
    QString name;
    QString goodDescription;
    QString goodDescriptionOverTime;
    QString badDescription;
    QString badDescriptionOverTime;
    qint32 displayPrecision;
    qint32 displayPrecisionMaxVal;
};

#endif // EFFECTSTABLE_H
