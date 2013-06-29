#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <effectstable.h>
#include <effectsdatfileparser.h>
#include <itemdetailstable.h>
#include <itemdatfileparser.h>
#include <itembaserequirementstable.h>
#include <itemlevelrequirementsparser.h>
#include <baseweapondamageparser.h>
#include <skillsdatfileparser.h>
#include <affixdatfileparser.h>
#include <affixtable.h>
#include <QThread>
#include <QTimer>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_CreateDatabasePushButton_clicked();

    void on_InfiniteStashFolderPushButton_clicked();

    void on_Torchlight2DatFilesFolderPushButton_clicked();
    void OnUpdateEffectsUI();
    void OnUpdateItemsUI();
    void OnUpdateItemRequirementsUI();
    void OnUpdateBaseWeaponDamageUI();
    void OnUpdateSkillsUI();
    void OnUpdateAffixUI();
    void OnProcessingFinished();
    void OnFinishedProcessingEffects();
//    void OnItemsFinished();
    void OnFinishedProcessingItemRequirements();
    void OnFinishedProcessingItemDetails();
    void OnFinishedProcessingBaseWeaponDamage();
    void OnFinishedProcessingSkills();
    void OnFinishedProcessingAffixes();

private:
    Ui::MainWindow *ui;

    EffectsDatFileParser* mEffectsParser;
    ItemDatFileParser* mItemsParser;
    ItemLevelRequirementsParser* mItemLevelRequirementsParser;
    BaseWeaponDamageParser* mBaseWeaponDamageParser;
    SkillsDatFileParser* mSkillsParser;
    AffixDatFileParser* mAffixesParser;

    QTimer* mUpdateEffectsUITimer;
    QTimer* mUpdateItemsUITimer;
    QTimer* mUpdateItemRequirementsUITimer;
    QTimer* mUpdateBaseWeaponDamageUITimer;
    QTimer* mUpdateSkillsUITimer;
    QTimer* mUpdateAffixesUITimer;

    QThread* mThread;

    static const QString ItemDetailsDatabaseFileName;

};

#endif // MAINWINDOW_H
