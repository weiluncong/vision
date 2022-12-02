#include "cpoint_item.h"

CPointItem::CPointItem(CVehicleBackGroundItem *backItem)
    : QGraphicsItem(), parentItem_(backItem)
{
    setParentItem(parentItem_);
}

QRectF CPointItem::boundingRect() const
{
    return parentItem_->boundingRect();
}
/** @brief 点云显示*/
void CPointItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    this->show();

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(color_, pen_width_));
    painter->setBrush(QBrush(color_));

    if (connect_swtich_ && name_.contains("_fs"))
    {
        for (cav::CPointData point : point_datas_)
        {
            QPointF itemPoint = parentItem_->PointItemInScene(QPointF(point.y_, point.x_));
            painter->drawEllipse(itemPoint, 1, 1);
        }
        painter->setPen(QPen(color_, 2));
        QuickSort(0, point_datas_.size() - 1, point_datas_);
        for (int i = 0; i < point_datas_.size() - 1; ++i)
        {
            QPointF startPoint = parentItem_->PointItemInScene(QPointF(point_datas_[i].y_, point_datas_[i].x_));
            QPointF endPoint = parentItem_->PointItemInScene(QPointF(point_datas_[i + 1].y_, point_datas_[i + 1].x_));
            painter->drawLine(startPoint, endPoint);
        }
    }
    else if (map_switch_)
    {
        Qt::PenStyle pen_style;
        switch (point_type_)
        {
        case 0:
        case 1:
        case 2:
        case 4:
            pen_style = Qt::SolidLine;
            break;

        default:
            pen_style = Qt::DashLine;
        }

        switch (line_type_)
        {
        case 0:
        case 1: //车道中心线1
        {
            painter->setPen(QPen(color_, pen_width_, pen_style));
            for (int i = 0; i < point_datas_.size() - 1; ++i)
            {
                QPointF startPoint = parentItem_->PointItemInScene(QPointF(point_datas_[i].y_, point_datas_[i].x_));
                QPointF endPoint = parentItem_->PointItemInScene(QPointF(point_datas_[i + 1].y_, point_datas_[i + 1].x_));

                painter->drawLine(startPoint, endPoint);
            }
            break;
        }
        case 2: //车道边线2
        {
            painter->setPen(QPen(color_, pen_width_, pen_style));
            for (int i = 0; i < point_datas_.size() - 1; ++i)
            {
                QPointF startPoint = parentItem_->PointItemInScene(QPointF(point_datas_[i].y_, point_datas_[i].x_));
                QPointF endPoint = parentItem_->PointItemInScene(QPointF(point_datas_[i + 1].y_, point_datas_[i + 1].x_));
                painter->drawLine(startPoint, endPoint);
            }
            break;
        }
        case 3: //边界线(护栏、路沿等)3
        {
            painter->setPen(QPen(color_, pen_width_, pen_style));
            for (int i = 0; i < point_datas_.size() - 1; ++i)
            {
                QPointF startPoint = parentItem_->PointItemInScene(QPointF(point_datas_[i].y_, point_datas_[i].x_));
                QPointF endPoint = parentItem_->PointItemInScene(QPointF(point_datas_[i + 1].y_, point_datas_[i + 1].x_));
                painter->drawLine(startPoint, endPoint);
            }
            break;
        }
        case 4: //边界线--护栏
        {
            painter->setPen(QPen(color_, 2));
            for (int i = 0; i < point_datas_.size() - 1; ++i)
            {
                QPointF startPoint = parentItem_->PointItemInScene(QPointF(point_datas_[i].y_, point_datas_[i].x_));
                QPointF endPoint = parentItem_->PointItemInScene(QPointF(point_datas_[i + 1].y_, point_datas_[i + 1].x_));
                painter->drawLine(startPoint, endPoint);
            }
            break;
        }
        case 5: //边界线--篱笆
        {
            painter->setPen(QPen(color_, pen_width_, pen_style));
            for (int i = 0; i < point_datas_.size() - 1; ++i)
            {
                QPointF startPoint = parentItem_->PointItemInScene(QPointF(point_datas_[i].y_, point_datas_[i].x_));
                QPointF endPoint = parentItem_->PointItemInScene(QPointF(point_datas_[i + 1].y_, point_datas_[i + 1].x_));
                painter->drawLine(startPoint, endPoint);
            }
            break;
        }
        case 6: //边界线--路缘
        {
            painter->setPen(QPen(color_, pen_width_, pen_style));
            for (int i = 0; i < point_datas_.size() - 1; ++i)
            {
                QPointF startPoint = parentItem_->PointItemInScene(QPointF(point_datas_[i].y_, point_datas_[i].x_));
                QPointF endPoint = parentItem_->PointItemInScene(QPointF(point_datas_[i + 1].y_, point_datas_[i + 1].x_));
                painter->drawLine(startPoint, endPoint);
            }
            break;
        }
        case 7: //边界线--墙
        default:
        {
            painter->setPen(QPen(color_, pen_width_, pen_style));
            for (int i = 0; i < point_datas_.size() - 1; ++i)
            {
                QPointF startPoint = parentItem_->PointItemInScene(QPointF(point_datas_[i].y_, point_datas_[i].x_));
                QPointF endPoint = parentItem_->PointItemInScene(QPointF(point_datas_[i + 1].y_, point_datas_[i + 1].x_));
                painter->drawLine(startPoint, endPoint);
            }
            break;
        }
        }

        if (is_draw_line_node_)
        {
            QPointF itemPoint = parentItem_->PointItemInScene(QPointF(point_datas_[0].y_, point_datas_[0].x_));
            painter->drawEllipse(itemPoint, pen_width_ + 1, pen_width_ + 1);

            itemPoint = parentItem_->PointItemInScene(QPointF(point_datas_[point_datas_.size() - 1].y_,
                                                              point_datas_[point_datas_.size() - 1].x_));
            painter->drawEllipse(itemPoint, pen_width_ + 1, pen_width_ + 1);
        }
    }
    else if (prediction_switch_)
    {
        for (auto line : prediction_lines_)
        {
            if (line.type_ == cav::CPredictionType::AlgorithmUnknow || 
                line.type_ == cav::CPredictionType::AlgorithmCar || 
                line.type_ == cav::CPredictionType::AlgorithmPerson)
            {
                for (int i = 0; i < line.predict_line_.size() - 1; i++)
                {
                    QPointF startPoint = parentItem_->PointItemInScene(QPointF(line.predict_line_[i].y_, line.predict_line_[i].x_));
                    QPointF endPoint = parentItem_->PointItemInScene(QPointF(line.predict_line_[i + 1].y_, line.predict_line_[i + 1].x_));
                    painter->drawEllipse(startPoint, 2, 2);
                    painter->drawLine(startPoint, endPoint);
                }
                QPointF point  = parentItem_->PointItemInScene(QPointF(line.predict_line_.last().y_, line.predict_line_.last().x_));
                painter->drawEllipse(point, 2, 2);
            }
            else if (line.type_ == cav::CPredictionType::AlgorithmCslstm)
            {
                for (int i = 0; i < line.predict_line_.size() - 1; i++)
                {
                        
                    QPointF startPoint = parentItem_->PointItemInScene(QPointF(line.predict_line_[i].y_, line.predict_line_[i].x_));
                    QPointF endPoint = parentItem_->PointItemInScene(QPointF(line.predict_line_[i + 1].y_, line.predict_line_[i + 1].x_));
                    painter->drawEllipse(startPoint, 2, 2);
                    if (i % 2 == 0)
                        painter->drawLine(startPoint, endPoint);
                }
                QPointF point  = parentItem_->PointItemInScene(QPointF(line.predict_line_.last().y_, line.predict_line_.last().x_));
                painter->drawEllipse(point, 2, 2);
            }
            else if (line.type_ == cav::CPredictionType::AlgorithmRaster)
            {
                for (int i = 0; i < line.predict_line_.size() - 1; i++)
                {

                    QPointF startPoint = parentItem_->PointItemInScene(QPointF(line.predict_line_[i].y_, line.predict_line_[i].x_));
                    QPointF endPoint = parentItem_->PointItemInScene(QPointF(line.predict_line_[i + 1].y_, line.predict_line_[i + 1].x_));
                    painter->drawEllipse(startPoint, 2, 2);
                    if (i % 3 == 0)
                        painter->drawLine(startPoint, endPoint);
                }
                QPointF point = parentItem_->PointItemInScene(QPointF(line.predict_line_.last().y_, line.predict_line_.last().x_));
                painter->drawEllipse(point, 2, 2);
            }
        }
    }
    else
    {
        for (cav::CPointData point : point_datas_)
        {
            QPointF itemPoint = parentItem_->PointItemInScene(QPointF(point.y_, point.x_));
            painter->drawEllipse(itemPoint, 2.5, 2.5);
        }
    }

    if (parentItem_->btn_press_)
    {
        cav::CPointData center;
        int counter = 0;
        for (cav::CPointData point : point_datas_)
        {
            center.x_ += point.x_;
            center.y_ += point.y_;
            counter++;
        }

        center.x_ = center.x_ / counter;
        center.y_ = center.y_ / counter;

        QFont font;
        font.setFamily("Microsoft YaHei");
        font.setPointSize(font_size);
        font.setBold(true);
        painter->setFont(font);
        QPointF itemPoint = parentItem_->PointItemInScene(QPointF(center.y_, center.x_));
        painter->drawText(itemPoint, name_);
    }
}

void CPointItem::QuickSort(int left, int right, QVector<cav::CPointData> &arr)
{
    if (left >= right)
        return;
    int i, j;
    cav::CPointData base, temp;
    i = left, j = right;
    base = arr[left];
    while (i < j)
    {
        while (arr[j].y_ >= base.y_ && i < j)
            j--;
        while (arr[i].y_ <= base.y_ && i < j)
            i++;
        if (i < j)
        {
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    arr[left] = arr[i];
    arr[i] = base;
    QuickSort(left, i - 1, arr);
    QuickSort(i + 1, right, arr);
}
