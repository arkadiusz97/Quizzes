#include "qComboBoxDelegate.h"

qComboBoxDelegate::qComboBoxDelegate(QObject* pobj) : QStyledItemDelegate(pobj)
{
}
QWidget* qComboBoxDelegate::createEditor(QWidget *parent,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const
{

    QStringList values;
    values << "A" << "B" << "C" << "D";
    QComboBox* comboBox = new QComboBox(parent);
    comboBox->addItems(values);
    connect(comboBox,  QOverload<int>::of(&QComboBox::activated),
             this, &qComboBoxDelegate::commitAndCloseEditor);
    return comboBox;

}

void qComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                         const QModelIndex &index)
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
    QString value = comboBox->currentText();
    model->setData(index, value, Qt::EditRole);
}

void qComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index)
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
    comboBox->setCurrentIndex(comboBox->findText(value));
}

void qComboBoxDelegate::commitAndCloseEditor(){
    QComboBox *editor = qobject_cast<QComboBox *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
