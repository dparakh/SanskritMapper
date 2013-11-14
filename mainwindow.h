#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mappingmodel.h"

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

    void on_mappingText_textEdited(const QString &arg1);

    void on_mappingCode_textEdited(const QString &arg1);

    void on_sourceTextEdit_textChanged();

    void on_sourceCharsList_currentRowChanged(int currentRow);

    void on_updateMapButton_clicked();

    void on_actionSave_triggered();

private:
    Ui::MainWindow *ui;
    dp::MappingModel m_CurrentMap;

    void UpdateMappedViews ();
};

#endif // MAINWINDOW_H
