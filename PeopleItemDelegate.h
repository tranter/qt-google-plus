#ifndef PEOPLEITEMDELEGATE_H
#define PEOPLEITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QPixmap>

class PeopleItemDelegate : public QStyledItemDelegate
{
public:
    PeopleItemDelegate( QObject * parent = 0 );

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    static QPixmap getPixmap(const QModelIndex & index);

private:
    int m_margin;
};

#endif // PEOPLEITEMDELEGATE_H
