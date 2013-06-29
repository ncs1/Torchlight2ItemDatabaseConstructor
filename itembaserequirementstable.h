#ifndef ITEMBASEREQUIREMENTSTABLE_H
#define ITEMBASEREQUIREMENTSTABLE_H

#include <sqlitetable.h>
#include <QString>

struct ItemBaseRequirements
{
    ItemBaseRequirements()
    {
        Clear();
    }

    void Clear()
    {
        itemBaseRequirementsId = 0;
        level = 0;
        levelRequired = 0;
        levelRequiredNormal = 0;
        levelRequiredSocketable = 0;
        dexterityRequired = 0;
        defenseRequired = 0;
        magicRequired = 0;
        strengthRequired = 0;
    }

    qint64 itemBaseRequirementsId;
    qint32 level;
    qint32 levelRequired;
    qint32 levelRequiredNormal;
    qint32 levelRequiredSocketable;
    qint32 defenseRequired;
    qint32 dexterityRequired;
    qint32 magicRequired;
    qint32 strengthRequired;
};

class ItemBaseRequirementsTable : public SqliteTable
{
public:
    ItemBaseRequirementsTable(QString databaseName);

    void DatabasePath(QString databaseName);

    qint64 AddItemBaseRequirements(ItemBaseRequirements itemRequirements);


private:
    static const QString TableName;
    static const QString ItemBaseRequirementsId;
    static const QString Level;
    static const QString LevelRequired;
    static const QString LevelRequiredNormal;
    static const QString LevelRequiredSocketable;
    static const QString DefenseRequired;
    static const QString DexterityRequired;
    static const QString MagicRequired;
    static const QString StrengthRequired;
};

#endif // ITEMBASEREQUIREMENTSTABLE_H
