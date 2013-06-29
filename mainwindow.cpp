#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDir>
#include <QFileDialog>


const QString MainWindow::ItemDetailsDatabaseFileName = "itemDatabase.sqlite";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    mEffectsTable = NULL;
    mEffectsParser = NULL;
//    mItemsTable = NULL;
    mItemsParser = NULL;
    mItemLevelRequirementsParser = NULL;
    mUpdateEffectsUITimer = NULL;
    mUpdateItemsUITimer = NULL;
    mUpdateItemRequirementsUITimer = NULL;
    mUpdateSkillsUITimer = NULL;
    mUpdateAffixesUITimer = NULL;
    mThread = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_CreateDatabasePushButton_clicked()
{
    QString datFilesFolderString = ui->Torchlight2DatFilesFolderLineEdit->text();
    QString infiniteStashFolderString = ui->InfiniteStashFolderLineEdit->text();

    if (!datFilesFolderString.isEmpty() && !infiniteStashFolderString.isEmpty())
    {
        ui->CreateDatabasePushButton->setEnabled(false);
        QDir datFilesFolder(datFilesFolderString);
        QDir infiniteStashFolder(infiniteStashFolderString);

        QString itemDetailsDatabaseFilePath = infiniteStashFolder.absoluteFilePath(ItemDetailsDatabaseFileName);

        mEffectsParser = new EffectsDatFileParser(itemDetailsDatabaseFilePath);
        mEffectsParser->DatFilesFolder(datFilesFolder);
        mItemsParser = new ItemDatFileParser(itemDetailsDatabaseFilePath);
        mItemsParser->DatFilesFolder(datFilesFolder);
        mItemLevelRequirementsParser = new ItemLevelRequirementsParser(itemDetailsDatabaseFilePath);
        mItemLevelRequirementsParser->DatFilesFolder(datFilesFolder);
        mBaseWeaponDamageParser = new BaseWeaponDamageParser(itemDetailsDatabaseFilePath);
        mBaseWeaponDamageParser->DatFilesFolder(datFilesFolder);
        mSkillsParser = new SkillsDatFileParser(itemDetailsDatabaseFilePath);
        mSkillsParser->DatFilesFolder(datFilesFolder);
        mAffixesParser = new AffixDatFileParser(itemDetailsDatabaseFilePath);
        mAffixesParser->DatFilesFolder(datFilesFolder);
        mUpdateEffectsUITimer = new QTimer(this);
        mUpdateItemsUITimer = new QTimer(this);
        mUpdateItemRequirementsUITimer = new QTimer(this);
        mUpdateBaseWeaponDamageUITimer = new QTimer(this);
        mUpdateSkillsUITimer = new QTimer(this);
        mUpdateAffixesUITimer = new QTimer(this);

        mThread = new QThread(this);

        connect(mThread, SIGNAL(started()), mEffectsParser, SLOT(ParseEffects()));
        connect(mThread, SIGNAL(started()), mItemLevelRequirementsParser, SLOT(ParseItemBaseRequirements()));
        connect(mThread, SIGNAL(started()), mItemsParser, SLOT(ParseItems()));
        connect(mThread, SIGNAL(started()), mBaseWeaponDamageParser, SLOT(ParseBaseWeaponDamage()));
        connect(mThread, SIGNAL(started()), mSkillsParser, SLOT(ParseSkills()));
        connect(mThread, SIGNAL(started()), mAffixesParser, SLOT(ParseAffixes()));
        connect(mThread, SIGNAL(finished()), this, SLOT(OnProcessingFinished()));
        connect(mThread, SIGNAL(finished()), mThread, SLOT(deleteLater()));

        connect(mEffectsParser, SIGNAL(finishedParsingEffects()), this, SLOT(OnFinishedProcessingEffects()));
        connect(mItemsParser, SIGNAL(finishedParsingItemsFiles()), this, SLOT(OnFinishedProcessingItemDetails()));
        connect(mItemLevelRequirementsParser, SIGNAL(finishedParsingItemRequirements()), this,
                SLOT(OnFinishedProcessingItemRequirements()));
        connect(mBaseWeaponDamageParser, SIGNAL(finishedParsingBaseWeaponDamage()), this,
                SLOT(OnFinishedProcessingBaseWeaponDamage()));
        connect(mSkillsParser, SIGNAL(finishedParsingSkillsFiles()), this,
                SLOT(OnFinishedProcessingSkills()));
        connect(mAffixesParser, SIGNAL(finishedParsingAffixFiles()), this,
                SLOT(OnFinishedProcessingAffixes()));


        connect(mUpdateEffectsUITimer, SIGNAL(timeout()), this, SLOT(OnUpdateEffectsUI()));
        connect(mUpdateItemsUITimer, SIGNAL(timeout()), this, SLOT(OnUpdateItemsUI()));
        connect(mUpdateItemRequirementsUITimer, SIGNAL(timeout()), this, SLOT(OnUpdateItemRequirementsUI()));
        connect(mUpdateBaseWeaponDamageUITimer, SIGNAL(timeout()), this, SLOT(OnUpdateBaseWeaponDamageUI()));
        connect(mUpdateSkillsUITimer, SIGNAL(timeout()), this, SLOT(OnUpdateSkillsUI()));
        connect(mUpdateAffixesUITimer, SIGNAL(timeout()), this, SLOT(OnUpdateAffixUI()));

        mEffectsParser->moveToThread(mThread);
        mItemsParser->moveToThread(mThread);
        mItemLevelRequirementsParser->moveToThread(mThread);
        mBaseWeaponDamageParser->moveToThread(mThread);
        mSkillsParser->moveToThread(mThread);
        mAffixesParser->moveToThread(mThread);

        mThread->start();
        mUpdateEffectsUITimer->start(100);
        mUpdateItemRequirementsUITimer->start(100);
        mUpdateBaseWeaponDamageUITimer->start(100);
        mUpdateItemsUITimer->start(100);
        mUpdateSkillsUITimer->start(100);
        mUpdateAffixesUITimer->start(100);
    }
}

void MainWindow::OnUpdateEffectsUI()
{
    ui->EffectsProgressLabel->setText("Processed Effects: " +
                                      QString::number(mEffectsParser->NumEffectsProcessed()));
}

void MainWindow::OnUpdateItemsUI()
{
    ui->ItemsProgressLabel->setText("Number of Items Files Processed: " +
                                    QString::number(mItemsParser->NumFilesProcessed()));
}

void MainWindow::OnUpdateItemRequirementsUI()
{
    ui->ItemRequirementsProgressLabel->setText("Number of Item Requirements Processed: " +
                                               QString::number(mItemLevelRequirementsParser->NumItemRequirementsProcessed()));
}


void MainWindow::OnUpdateBaseWeaponDamageUI()
{
    ui->BaseWeaponDamageProgressLabel->setText("Number of Base Weapon Damage Levels Processed: " +
                                               QString::number(mBaseWeaponDamageParser->NumBaseWeaponDamageProcessed()));
}

void MainWindow::OnUpdateSkillsUI()
{
    ui->SkillsProgressLabel->setText("Number of Skills Processed: " +
                                     QString::number(mSkillsParser->NumFilesProcessed()));
}

void MainWindow::OnUpdateAffixUI()
{
    ui->AffixesProgressLabel->setText("Number of Affixes Processed: " +
                                      QString::number(mAffixesParser->NumFilesProcessed()));
}

void MainWindow::OnFinishedProcessingEffects()
{
    mUpdateEffectsUITimer->stop();
    ui->UserInformationLabel->setText("Done Processing Effects!");
}

void MainWindow::OnFinishedProcessingItemRequirements()
{
    mUpdateItemRequirementsUITimer->stop();
    ui->UserInformationLabel->setText("Done Processing Item Requirements!");
}

void MainWindow::OnFinishedProcessingBaseWeaponDamage()
{
    mUpdateBaseWeaponDamageUITimer->stop();
    ui->UserInformationLabel->setText("Done Processing Base Weapon Damage!");
}


void MainWindow::OnFinishedProcessingItemDetails()
{
    mUpdateItemsUITimer->stop();
    ui->UserInformationLabel->setText("Done Processing Items!");
}

void MainWindow::OnFinishedProcessingSkills()
{
    mUpdateSkillsUITimer->stop();
    ui->UserInformationLabel->setText("Done Processing Skills!");
}

void MainWindow::OnFinishedProcessingAffixes()
{
    mUpdateAffixesUITimer->stop();
    ui->UserInformationLabel->setText("Done Processing Affixes!");
    mThread->exit();
}

void MainWindow::OnProcessingFinished()
{

    delete mEffectsParser;
    mEffectsParser = NULL;

    delete mUpdateEffectsUITimer;
    mUpdateEffectsUITimer = NULL;

    delete mItemLevelRequirementsParser;
    mItemLevelRequirementsParser = NULL;

    delete mUpdateItemRequirementsUITimer;
    mUpdateItemRequirementsUITimer = NULL;

    delete mBaseWeaponDamageParser;
    mBaseWeaponDamageParser = NULL;

    delete mUpdateBaseWeaponDamageUITimer;
    mUpdateBaseWeaponDamageUITimer = NULL;

    delete mItemsParser;
    mItemsParser = NULL;

    delete mUpdateItemsUITimer;
    mUpdateItemsUITimer = NULL;

    delete mSkillsParser;
    mSkillsParser = NULL;

    delete mUpdateSkillsUITimer;
    mUpdateSkillsUITimer = NULL;

    delete mAffixesParser;
    mAffixesParser = NULL;

    delete mUpdateAffixesUITimer;
    mUpdateAffixesUITimer = NULL;

    ui->CreateDatabasePushButton->setEnabled(true);
}


void MainWindow::on_InfiniteStashFolderPushButton_clicked()
{
    QString folder = QFileDialog::getExistingDirectory(this, "Choose Infinite Stash Folder");

    if (!folder.isNull() && !folder.isEmpty())
    {
        QDir infiniteStashFolder(folder);

        if (infiniteStashFolder.exists())
        {
            ui->InfiniteStashFolderLineEdit->setText(infiniteStashFolder.absolutePath());
            ui->InfiniteStashFolderLineEdit->setToolTip(infiniteStashFolder.absolutePath());
        }
    }
}

void MainWindow::on_Torchlight2DatFilesFolderPushButton_clicked()
{
    QString folder = QFileDialog::getExistingDirectory(this, "Choose Torchlight 2 Dat Files Media folder");

    if (!folder.isNull() && !folder.isEmpty())
    {
        QDir torchlight2DatFilesFolder(folder);

        if (torchlight2DatFilesFolder.exists())
        {
            ui->Torchlight2DatFilesFolderLineEdit->setText(torchlight2DatFilesFolder.absolutePath());
            ui->Torchlight2DatFilesFolderLineEdit->setToolTip(torchlight2DatFilesFolder.absolutePath());
        }
    }
}
