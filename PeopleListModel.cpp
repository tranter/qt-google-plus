#include "PeopleListModel.h"

#include <QStringList>
#include <QDebug>

PeopleListModel::PeopleListModel(QObject * p) :
    QAbstractItemModel(p)
{
}

void
PeopleListModel::
clear()
{
    if( m_list.size() )
    {
        emit beginRemoveRows(QModelIndex(), 0, m_list.size()-1);
        m_list.clear();
        emit endRemoveRows();
    }
}

void
PeopleListModel::
setItems(const QStringList & items)
{
    clear();

    int size = items.size();
    if( ! size ) return;

    emit beginInsertRows(QModelIndex(), 0, size-1 );

    for(int i(0); i < size; ++i)
        m_list.append( Pair(items[i], QPixmap()) );

    emit endInsertRows();
}

void
PeopleListModel::
setPixmap(int position, const QPixmap & pixmap)
{
    if( position < 0 || m_list.size() <= position )
        return;

    m_list[position].second = pixmap;
    emit dataChanged( index(position), index(position) );
}

QVariant
PeopleListModel::
data(const QModelIndex & idx, int role) const
{
    int cRow = idx.row();
    if( cRow < 0 || m_list.size() <= cRow )
        return QVariant();

    if( role == Qt::DisplayRole )
    {
        switch( idx.column() )
        {
        case 0: return m_list[cRow].first;
        case 1: return m_list[cRow].second;
        default: ;
        }
    } else if( role == PixmapRole )
        return m_list[cRow].second;

    return QVariant();
}

QModelIndex
PeopleListModel::
index(int row, int column, const QModelIndex & /*parent*/) const
{
    return createIndex(row, column);
}

QModelIndex
PeopleListModel::
parent(const QModelIndex & /*child*/) const
{
    return QModelIndex();
}
