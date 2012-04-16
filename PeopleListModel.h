#ifndef PEOPLELISTMODEL_H
#define PEOPLELISTMODEL_H

#include <QAbstractListModel>
#include <QPixmap>

class PeopleListModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit PeopleListModel(QObject * parent = 0);
    enum {
        PixmapRole = Qt::UserRole + 1
    };

    int rowCount(const QModelIndex &) const { return m_list.size(); }
    int columnCount(const QModelIndex &) const { return 2; }

    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex index(int row, int column = 0, const QModelIndex & parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex & child) const;

public slots:
    void clear();
    void setItems(const QStringList & items);
    void setPixmap(int position, const QPixmap & pixmap);

private:
    typedef QPair<QString, QPixmap> Pair;
    QList<Pair> m_list;
};

#endif // PEOPLELISTMODEL_H
