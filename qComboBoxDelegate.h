#include <QWidget>
#include <QStyledItemDelegate>
#include <QComboBox>
#include <QObject>

class qComboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    qComboBoxDelegate(QObject* pobj = 0);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                const QModelIndex &index) const;

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                                             const QModelIndex &index);

    void setEditorData(QWidget *editor, const QModelIndex &index);
private:
    void commitAndCloseEditor();
};
