#ifndef ITEMSTABLE_H
#define ITEMSTABLE_H

#include <sqlitetable.h>
#include <QString>

struct ItemDetails
{
    ItemDetails()
    {
        Clear();
    }

    void Clear()
    {
        itemId = 0;
        filePath = "";
        guid = 0;
        name = "";
        displayName = "";
        setName = "";
        itemType = "";
        level = 0;
        levelRequired = 0;
        is2Handed = false;
        defenseRequired = 0;
        dexterityRequired = 0;
        strengthRequired = 0;
        magicRequired = 0;
        damagePhysical = 0;
        damageElectric = 0;
        damageFire = 0;
        damageIce = 0;
        damagePoison = 0;
        minimumDamage = 0;
        maximumDamage = 0;
        deviationFrom30FPS = 1;
        speed = 0;
        speedDamageMod = 0;
        rarityDamageMod = 0;
        description = "";
        requiredClass = "";
    }

    qint64 itemId;
    QString filePath;
    qint64 guid;
    QString name;
    QString displayName;
    QString setName;
    QString itemType;
    qint32 level;
    qint32 levelRequired;
    bool is2Handed;
    qint32 defenseRequired;
    qint32 dexterityRequired;
    qint32 strengthRequired;
    qint32 magicRequired;
    qint32 damagePhysical;
    qint32 damageElectric;
    qint32 damageFire;
    qint32 damageIce;
    qint32 damagePoison;
    qint32 minimumDamage;
    qint32 maximumDamage;
    double deviationFrom30FPS;
    qint32 speed;
    qint32 speedDamageMod;
    qint32 rarityDamageMod;
    QString description;
    QString requiredClass;
};

class ItemDetailsTable : public SqliteTable
{
public:
    ItemDetailsTable(QString databaseName);

    void DatabasePath(QString databaseName);

    qint64 AddItem(const ItemDetails& inItem);

private:
    static const QString ItemDetailsTableName;
    static const QString ItemsTableId;
    static const QString ItemFilePath;
    static const QString Guid;
    static const QString Name;
    static const QString DisplayName;
    static const QString SetName;
    static const QString ItemType;
    static const QString Level;
    static const QString LevelRequired;
    static const QString Is2Handed;
    static const QString DefenseRequired;
    static const QString DexterityRequired;
    static const QString StrengthRequired;
    static const QString MagicRequired;
    static const QString DamagePhysical;
    static const QString DamageElectric;
    static const QString DamageFire;
    static const QString DamageIce;
    static const QString DamagePoison;
    static const QString MinimumDamage;
    static const QString MaximumDamage;
    static const QString DeviationFrom30FPS;
    static const QString Speed;
    static const QString SpeedDamageMod;
    static const QString RarityDamageMod;
    static const QString Description;
    static const QString RequiredClass;

};

#endif // ITEMSTABLE_H
