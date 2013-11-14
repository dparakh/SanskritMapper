#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDir>

using namespace std;

static const char kDefaultFile[] = ".SanskritMapper.map";
#define DEFAULT_FILE QDir::homePath() + "/" + kDefaultFile

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_CurrentMap.LoadFromFile(DEFAULT_FILE);
}

MainWindow::~MainWindow()
{
    m_CurrentMap.SaveToFile(DEFAULT_FILE);
    delete ui;
}


void MainWindow::on_mappingText_textEdited(const QString & arg1)
{
    QString mappedCode;
    foreach (QChar mappedChar, arg1)
    {
        QString unicodeHex;
        unicodeHex.sprintf("%04x", mappedChar.unicode());
        mappedCode += unicodeHex;
    }
    ui->mappingCode->setText(mappedCode);
    //We should also cause an update of the mapped chars list
    //as well as the mapped Text
    UpdateMappedViews();
}

void MainWindow::on_mappingCode_textEdited(const QString & arg1)
{
    //A copy to work with.
    QString workingCopy (arg1);
    QString resultingCode;

    while (workingCopy.size())
    {
        //take up to 4 hex encoded asii chars, which respresent one Unicode char.
        QString workingChar (workingCopy.left(4));
        //remove what we took from the working copy
        workingCopy.remove(0, workingChar.size());
        //convert to a QChar and append to the resulting code
        resultingCode += QChar (workingChar.toShort(false, 16));
    }

    //update the mappingText
    ui->mappingText->setText(resultingCode);

    //Update everything else.
    UpdateMappedViews();
}

//**********************************************************************************************
// MainWindow::on_sourceTextEdit_textChanged
//
//! Called when the source text is changed. In response this function
//!   fills the source chars list, and also based on the mapping table
//!   maps them to destination chars list, and updates the mapped text.
//!
//! \param  : None.
//!
//**********************************************************************************************
void MainWindow::on_sourceTextEdit_textChanged()
{
    //clear the list of characters
    ui->sourceCharsList->clear();

    //for each characeter in the new text
    QString sourceText = ui->sourceTextEdit->toPlainText();

    foreach (QChar sourceChar, sourceText)
    {
        QString charDescription;
        if (sourceChar.toLatin1() >= 32)
            charDescription.sprintf("%c [0x%02x]", sourceChar.toLatin1(), sourceChar.toLatin1());
        else
            charDescription.sprintf("?? : [0x%02x]", sourceChar.toLatin1());

        ui->sourceCharsList->addItem(charDescription);
    }
    UpdateMappedViews();
}

void MainWindow::on_sourceCharsList_currentRowChanged(int currentRow)
{
    ui->mappingText->setText("");
    ui->mappingCode->setText("");
    ui->mapAsPostFix->setChecked(false);
    if (0 <= currentRow)
    {
        QString mappedText;
        int mappedLocation(0);
        if (m_CurrentMap.FindMapEntry(ui->sourceTextEdit->toPlainText().at(currentRow).toLatin1(),
                                      mappedText, mappedLocation))
        {
            ui->mappingText->setText(mappedText);
            on_mappingText_textEdited(mappedText);
            SetMappingLocation(mappedLocation);
        }
    }
    //ToDo: Also change the current Row of the mapped Chars list!
    ui->mappedCharsList->setCurrentRow(currentRow);
}

void MainWindow::on_updateMapButton_clicked()
{
    const int currentRow (ui->sourceCharsList->currentRow());
    if (0 <= currentRow)
    {
       m_CurrentMap.UpdateMapEntry(ui->sourceTextEdit->toPlainText().at(currentRow).toLatin1(),
                                   ui->mappingText->text(), GetMappingLocation());
    }
    UpdateMappedViews();
}

void MainWindow::UpdateMappedViews()
{
    ui->mappedCharsList->clear();
    //for each characeter in the source text
    QString sourceText = ui->sourceTextEdit->toPlainText();
    QString fullMappedText;
    QString toPostFix;

    int charIndex(0);
    foreach (QChar sourceChar, sourceText)
    {
        QString mappedText;
        int mappedLocation(0);
        if (m_CurrentMap.FindMapEntry(sourceChar.toLatin1(),
                                      mappedText, mappedLocation))
        {
            QString mappedCode;
            foreach (QChar mappedChar, mappedText)
            {
                QString unicodeHex;
                unicodeHex.sprintf("%04x", mappedChar.unicode());
                mappedCode += unicodeHex;
            }
            QString charDescription;
            charDescription += mappedText + " [" + mappedCode + "]";
            ui->mappedCharsList->addItem(charDescription);
            if (mappedLocation == dp::MappingModel::kLocationPostFix)
            {
                toPostFix += mappedText;
            }
            else if (mappedLocation == dp::MappingModel::kLocationPrefix)
            {
                fullMappedText = fullMappedText.left(fullMappedText.size()-1) + mappedText + fullMappedText.right(1);
            }
            else
            {
                fullMappedText += mappedText + toPostFix;
                toPostFix.clear();
            }
        }
        else
        {
            ui->mappedCharsList->addItem(ui->sourceCharsList->item(charIndex)->text());
            ui->mappedCharsList->item(charIndex)->setBackgroundColor(Qt::red);
            fullMappedText += sourceChar;
        }
        ++charIndex;
    }

    ui->mappedText->setPlainText(fullMappedText);
}

int MainWindow::GetMappingLocation()
{
    int retVal(0);

    if (ui->mapAsPreFix->isChecked())
        retVal = dp::MappingModel::kLocationPrefix;
    else if (ui->mapAsPostFix->isChecked())
        retVal = dp::MappingModel::kLocationPostFix;
    else if (ui->mapAsInPlace->isChecked())
        retVal = dp::MappingModel::kLocationInPlace;

    return retVal;
}

void MainWindow::SetMappingLocation(int in_newLocation)
{
    switch (in_newLocation)
    {
    case dp::MappingModel::kLocationPrefix:
        ui->mapAsPreFix->setChecked(true);
        break;
    case dp::MappingModel::kLocationPostFix:
        ui->mapAsPostFix->setChecked(true);
        break;
    case dp::MappingModel::kLocationInPlace:
    default:
        ui->mapAsInPlace->setChecked(true);
    }
}

void MainWindow::on_actionSave_triggered()
{
    //m_CurrentMap.SaveToFile("/tmp/test.json");
}
