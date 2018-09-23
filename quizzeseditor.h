#ifndef QUIZZESEDITOR_H
#define QUIZZESEDITOR_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QMessageBox>
#include "quizzeseditorbackend.h"
#include "qComboBoxDelegate.h"

namespace Ui {
class QuizzesEditor;
}

class QuizzesEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit QuizzesEditor(QWidget *parent = 0);
    ~QuizzesEditor();

private slots:
    void on_NewButton_clicked();

    void on_RemoveButton_clicked();

    void openQuizz();

    void saveQuizz();

    void setHorizontalHeaderLabelsInModel();

    void saveQuizzAs();

    void newQuizz();

    void open(QString fileName);

    void cellModified();

    bool saveQuestion();

private:
    Ui::QuizzesEditor *ui;
    QStandardItemModel questionsTableViewModel;
    QuizzesEditorBackend backend;
    QList<QStandardItem*> row;
    QVector<QStandardItem*>rowItems;
    bool isModified;
    qComboBoxDelegate delegate;
};

#endif // QUIZZESEDITOR_H
