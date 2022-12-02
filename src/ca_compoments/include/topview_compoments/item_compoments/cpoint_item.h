#ifndef KPOINT_ITEM_H
#define KPOINT_ITEM_H

#include <QObject>
#include <QWidget>
#include <QDebug>

#include "cvehicle_background_item.h"
#include "cpoint_struct.h"
#include "cprediction_struct.h"


class CPointItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit CPointItem(CVehicleBackGroundItem *backItem);

    ~CPointItem(){}

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void SetPointName(const QString &name){ name_ = name; }
    void SetColor(const QColor &color){ color_ = color; }
    void SetData(const QVector<cav::CPointData> &data) { point_datas_ = data; }
    void SetPrediction(const QVector<cav::CPredictLine> &data) { prediction_lines_ = data;}
    void clear(){ this->hide();}

    bool connect_swtich_ = false;
    bool map_switch_ = false;
    bool prediction_switch_ = false;
    bool is_draw_name_ = false;
    int pen_width_ = 2;
    int font_size = 5;
    bool is_draw_line_node_ = false;


    inline void SetPointType(const int &PointType){ point_type_ = PointType; }
    inline void SetLineType(const int &LineType){ line_type_ = LineType; }

private:
    CVehicleBackGroundItem *parentItem_;
    QColor color_;
    QString name_;
    QVector<cav::CPointData> point_datas_;
    QVector<cav::CPredictLine> prediction_lines_;

    //<无车道线0;
    //<未知1;
    //<单实线2;
    //<单虚线3;
    //<双实线4;
    //<双虚线5;
    //<左实右虚6;
    //<左虚右实7;
    //<短虚线8;
    //<导流线9;
    //<物理分隔10;
    int point_type_ = 0;
    int line_type_ = 0;
    
    void QuickSort(int left, int right, QVector<cav::CPointData> &arr);
};

#endif // KPOINT_ITEM_H
