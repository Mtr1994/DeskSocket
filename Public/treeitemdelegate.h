#ifndef TREEITEMDELEGATE_H
#define TREEITEMDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QPainter>
#include <QRect>

#include "Public/defines.h"

class TreeItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TreeItemDelegate(){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QRect rect(option.rect);
        rect.setLeft(0);

        if (option.state & QStyle::State_Selected )
        {
            painter->fillRect(rect, QBrush("#557cb3f1"));
        }
        else if (option.state & QStyle::State_MouseOver)
        {
            painter->fillRect(rect, QBrush("#337cb3f1"));
        }

        QRectF rectangle(rect.width() - rect.width() / 12, rect.height() / 2.0 + rect.y() - 4, 8, 8);
        QString mark = index.data(Qt::UserRole + 1).toString();
        painter->setRenderHint(QPainter::Antialiasing, true);
        if (mark.contains("SERVER"))
        {
            int status = index.data(Qt::UserRole + 2).toUInt();
            QColor color = (status == Server_Started) ? QColor(82, 196, 26, 255) : (status == Server_Shutdown) ? QColor(245, 34, 45, 255) : QColor(24, 144, 255, 255);

            painter->setPen(QPen(color));
            painter->setBrush(QBrush(color));
            painter->drawEllipse(rectangle);
        }
        else if (mark.contains("CLIENT"))
        {
            int status = index.data(Qt::UserRole + 2).toUInt();
            QColor color = (status == Client_Connected) ? QColor(82, 196, 26, 255) : (status == Client_Closed) ? QColor(245, 34, 45, 255) : QColor(24, 144, 255, 255);

            painter->setPen(QPen(color));
            painter->setBrush(QBrush(color));
            painter->drawEllipse(rectangle);
        }

        QStyledItemDelegate::paint(painter, option, index);
    }
};

#endif // TREEITEMDELEGATE_H
