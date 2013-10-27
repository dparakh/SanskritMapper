#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
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
    //ToDo: We should also cause an update of the mapped chars list
    //as well as the mapped Text
}

void MainWindow::on_mappingCode_textEdited(const QString & /*arg1*/)
{

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
    
}

void MainWindow::on_sourceCharsList_currentRowChanged(int currentRow)
{
    ui->mappingText->setText("");
    ui->mappingCode->setText("");
    ui->mapAsPostFix->setChecked(false);
    if (0 <= currentRow)
    {
        QString mappedText;
        bool mappedPostFix(false);
        if (m_CurrentMap.FindMapEntry(ui->sourceTextEdit->toPlainText().at(currentRow).toLatin1(),
                                      mappedText, mappedPostFix))
        {
            ui->mappingText->setText(mappedText);
            on_mappingText_textEdited(mappedText);
            ui->mapAsPostFix->setChecked(mappedPostFix);
        }
    }
    //ToDo: Also change the current Row of the mapped Chars list!
}

void MainWindow::on_updateMapButton_clicked()
{
    const int currentRow (ui->sourceCharsList->currentRow());
    if (0 <= currentRow)
    {
       m_CurrentMap.UpdateMapEntry(ui->sourceTextEdit->toPlainText().at(currentRow).toLatin1(),
                                   ui->mappingText->text(), ui->mapAsPostFix->isChecked());
    }
}
