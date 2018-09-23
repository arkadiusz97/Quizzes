#include "quizzeseditor.h"
#include "ui_quizzeseditor.h"

QuizzesEditor::QuizzesEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QuizzesEditor),
    isModified(false)
{
    ui->setupUi(this);
    ui->questionsTableView->setModel(&questionsTableViewModel);
    ui->questionsTableView->setItemDelegateForColumn(2, &delegate);
    ui->questionsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->questionsTableView->resizeColumnsToContents();
    ui->questionsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    setHorizontalHeaderLabelsInModel();
    connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(openQuizz()));
    connect(ui->actionSave, SIGNAL(triggered(bool)), this, SLOT(saveQuizz()));
    connect(ui->actionSave_as, SIGNAL(triggered(bool)), this, SLOT(saveQuizzAs()));
    connect(ui->actionNew, SIGNAL(triggered(bool)), this, SLOT(newQuizz()));
    connect(&questionsTableViewModel, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(cellModified()));
    ui->questionsTableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
}

QuizzesEditor::~QuizzesEditor()
{
    delete ui;
}

void QuizzesEditor::on_NewButton_clicked()
{
    questionsTableViewModel.insertRow(0);
    ui->questionsTableView->selectRow(0);
}

void QuizzesEditor::on_RemoveButton_clicked()
{
    QModelIndexList selection = ui->questionsTableView->selectionModel()->selectedRows();
    for(int i=0; i< selection.count(); i++)
    {
        questionsTableViewModel.removeRow(selection.at(i).row());
    }
}

void QuizzesEditor::openQuizz()
{
    if(!saveQuestion())
        return;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open quiz"), "", tr("Quiz Files (*.qz)"));
    open(fileName);
}

void QuizzesEditor::setHorizontalHeaderLabelsInModel()
{
    questionsTableViewModel.setHorizontalHeaderLabels(
    {
          "image",
          "question",
          "valid answer",
          "answer a",
          "answer b",
          "answer c(optional)",
          "answer d(optional)"
    });
    for(int i = 0; i < ui->questionsTableView->horizontalHeader()->count(); ++i)
    {
        ui->questionsTableView->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    }
}

void QuizzesEditor::saveQuizz()
{
    backend.saveQuiz(questionsTableViewModel);
    isModified = false;
}

void QuizzesEditor::saveQuizzAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save quiz"), "", tr("Quiz Files (*.qz)"));
    backend.saveQuizAs(questionsTableViewModel, fileName);
}

void QuizzesEditor::newQuizz()
{
    if(!saveQuestion())
        return;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save quiz"), "", tr("Quiz Files (*.qz)"));
    QFile file(fileName);
    file.open(QIODevice::ReadWrite);
    file.close();
    open(fileName);
}

void QuizzesEditor::open(QString fileName)
{
    QJsonArray quizToEdit;
    try
    {
        quizToEdit = backend.openQuiz(fileName);
    }
    catch(int)
    {
        return;
    }
    /*for(auto i : rowItems) ---PROBLEM---
    {
        delete [] i;
    }*/
    ui->questionsTableView->setEnabled(true);
    setWindowTitle("Quizzes Editor - " + fileName);
    isModified = false;
    questionsTableViewModel.clear();
    setHorizontalHeaderLabelsInModel();
    rowItems.clear();
    QString columnsNames[] = {"image", "question", "Valid answer", "answer A", "answer B", "answer C", "answer D"};
    for(auto i : quizToEdit)
    {
        rowItems.push_back(new QStandardItem[7]);//To do.
        for(int j=0; j<7; ++j)
        {
            rowItems[rowItems.size()-1][j].setText(i.toObject()[columnsNames[j]].toString());
            row.push_back(&rowItems[rowItems.size()-1][j]);
        }
        questionsTableViewModel.appendRow(row);
        row.clear();
    }
}

void QuizzesEditor::cellModified()
{
    isModified = true;
}

bool QuizzesEditor::saveQuestion()
{
    if(isModified)
    {
        auto userReply = QMessageBox::question(this, "Test", "Do you save changes?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if(userReply == QMessageBox::Yes)
        {
            backend.saveQuiz(questionsTableViewModel);
            return true;
        }
        else if(userReply == QMessageBox::Cancel)
        {
            return false;
        }
        else if(userReply == QMessageBox::No)
        {
            return true;
        }
    }
    return true;
}
