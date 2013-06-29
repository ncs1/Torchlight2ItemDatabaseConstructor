#ifndef AFFIXTABLE_H
#define AFFIXTABLE_H

#include <sqlitetable.h>
#include <QString>

struct Affix;

class AffixTable : public SqliteTable
{
public:
    AffixTable(QString databaseName);

    void DatabasePath(QString databaseName);

    qint64 AddAffix(const Affix& affix);

private:
    static const QString AffixTableName;
    static const QString AffixTableId;
    static const QString Name;
    static const QString Duration;
    static const QString EffectName;
    static const QString EffectDuration;
    static const QString EffectDamageType;
    static const QString EffectMin;
    static const QString EffectMax;

};

struct Affix
{
    Affix()
    {

    }

    void Clear()
    {
        id = 0;
        name = "";
        duration = 0;
        effectName = "";
        effectDuration = "";
        effectDamageType = "";
        effectMin = 0;
        effectMax = 0;
    }

    qint64 id;
    QString name;
    double duration;
    QString effectName;
    QString effectDuration;
    QString effectDamageType;
    double effectMin;
    double effectMax;
};

#endif // AFFIXTABLE_H
