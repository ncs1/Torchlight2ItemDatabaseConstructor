#ifndef WEAPONDAMAGETABLE_H
#define WEAPONDAMAGETABLE_H

#include <sqlitetable.h>

struct BaseWeaponDamage
{
    BaseWeaponDamage()
    {
        Clear();
    }

    void Clear()
    {
        level = 0;
        baseDamage = 0;
    }

    qint32 level;
    float baseDamage;
};


class BaseWeaponDamageTable : public SqliteTable
{
public:
    BaseWeaponDamageTable(QString databaseName);

    void DatabasePath(QString databaseName);

    qint64 AddBaseWeaponDamage(BaseWeaponDamage baseDamage);

private:
    static const QString BaseWeaponDamageTableName;
    static const QString BaseWeaponDamageId;
    static const QString Level;
    static const QString BaseDamage;
};

#endif // WEAPONDAMAGETABLE_H
