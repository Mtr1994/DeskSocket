#ifndef TREEITEMDELEGATE_H
#define TREEITEMDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QPainter>
#include <QRect>

class TreeItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TreeItemDelegate(){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QRect rect(option.rect);
        rect.setLeft(0);
        rect.adjust(2, 2, -2, -2);

        // 绘制底色
        if (option.state & QStyle::State_Selected)
        {
            painter->setPen(Qt::NoPen);
            painter->setBrush(QBrush(QColor(124, 179, 241, 85)));
            painter->drawRoundedRect(rect, 2, 2);
        }
        else if (option.state & QStyle::State_MouseOver)
        {
            painter->setPen(Qt::NoPen);
            painter->setBrush(QBrush(QColor(124, 179, 241, 51)));
            painter->drawRoundedRect(rect, 2, 2);
        }

        // 判断节点类型
        int node = index.data(Qt::UserRole + 3).toUInt();
        int leftMargin = node * rect.height() * 0.6 + rect.height() * 0.2;

        // 绘制图标
        painter->setRenderHint(QPainter::SmoothPixmapTransform);
        QIcon icon = index.data(Qt::DecorationRole).value<QIcon>();
        QRect rectIcon = QRect(rect.height() * 0.5 + leftMargin, rect.y() + rect.height() * 0.25, rect.height() * 0.5, rect.height() * 0.5);
        painter->drawPixmap(rectIcon, icon.pixmap(icon.availableSizes().at(0)));

        // 绘制文本
        QRect rectText = QRect(rect.height() * 1.2 + leftMargin, rect.y(), rect.width() - rect.height() * 1.5, rect.height());
        painter->setRenderHint(QPainter::TextAntialiasing);
        painter->setPen(QColor(23, 23, 23));
        painter->drawText(rectText, Qt::AlignVCenter, index.data(Qt::DisplayRole).toString());

        // 绘制状态
        QRectF rectangle(rect.width() - rect.width() / 12, rect.height() / 2.0 + rect.y() - 4, 8, 8);
        painter->setRenderHint(QPainter::Antialiasing, true);
        int status = index.data(Qt::UserRole + 2).toUInt();
        QColor color = getColor(status);
        painter->setPen(color);
        painter->setBrush(QBrush(color));
        painter->drawEllipse(rectangle);
    }

private:
    QColor getColor(int status) const
    {
        switch (status)
        {
            case 0:
            {
                return QColor(24, 144, 255, 255);
            }
            case 1:
            {
                return QColor(82, 196, 26, 255);
            }
            case 2:
            case 3:
            {
                return QColor(245, 34, 45, 255);
            }
            default:
            {
                return QColor(24, 144, 255, 255);
            }
        }
    }
};

#endif // TREEITEMDELEGATE_H
