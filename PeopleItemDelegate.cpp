#include "PeopleItemDelegate.h"
#include "PeopleListModel.h"

#include <QPainter>
#include <QApplication>
#include <QPixmap>

#include <QDebug>

PeopleItemDelegate::
PeopleItemDelegate(QObject * p) :
    QStyledItemDelegate(p)
{
    m_margin = 3;
}

void
PeopleItemDelegate::
paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    painter->save();

    QRectF rect = option.rect;

    if( option.state & QStyle::State_Selected )
    {
        painter->save();
        painter->setBrush( QApplication::palette().highlight() );
        painter->setPen( Qt::NoPen );
        painter->drawRect(rect);
        painter->restore();
    }

    QPixmap pixmap = getPixmap(index);

    if( pixmap.isNull() )
    {
        painter->drawText( rect, index.data().toString() );
    } else {

        QRectF target( rect.topLeft() + QPoint(m_margin, m_margin), pixmap.size() );

        painter->drawPixmap( target, pixmap, pixmap.rect() );

        int textHeight = painter->fontMetrics().height();
        int left = pixmap.width() + m_margin + 10;
        int top = (rect.height() - textHeight) * 0.5;

        target.setTopLeft( QPoint( left, top ) + target.topLeft() );
        target.setSize( QSizeF( rect.width()-left, textHeight ) );

        painter->drawText(target, index.data().toString() );
    }

    painter->restore();
}

QSize
PeopleItemDelegate::
sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    QSize originSizeHint = QStyledItemDelegate::sizeHint(option, index);

    QPixmap pixmap = getPixmap(index);
    return pixmap.isNull() ? originSizeHint : QSize( originSizeHint.width(), pixmap.height() + m_margin * 2 );
}

QPixmap
PeopleItemDelegate::
getPixmap(const QModelIndex & index)
{
    QVariant pixValue = index.data(PeopleListModel::PixmapRole);
    return pixValue.isValid() ? pixValue.value<QPixmap>() : QPixmap();
}
