#ifndef CVISION_WIDGET_H
#define CVISION_WIDGET_H

#include <QtWidgets>
#include <QCloseEvent>
#include "opencv2/opencv.hpp"

#include "cvision_obj_struct.h"
#include "ccamera_struct.h"
#include "cobj_struct.h"
#include "cvision_rle_struct.h"
#include "ccamera_struct.h"
#include "cobj_struct.h"
#include "cglobal_param.h"
#include "cslider.h"
#include "ccamera.h"
#include "ccamera_widget.h"

/**
 * @brief SDA视觉可视化窗口，用于显示视觉视频数据，
 */
class CVisionCameraWidget : public QWidget
{
    Q_OBJECT
protected:
    void resizeEvent(QResizeEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

public:
    CVisionCameraWidget(QWidget *parent = nullptr);
    ~CVisionCameraWidget();

    QString Name() const { return name_; }
    void SetName(const QString &name)
    {
        name_ = name;
        setWindowTitle(name_);
        UpdateView(last_time_str_, last_time_);
    }
    void SetCameraList(const QStringList &camera_list);
    void SetSliderRange(double max)
    {
        if (!FLAGS_v_online)
            slider_->setRange(0, max * 100);
    }
    void SetSliderStep(int value)
    {
        if (!FLAGS_v_online)
            slider_->setSingleStep(value);
    }

    /**
     * @brief 更新当前视觉视图视频数据
     */
    void UpdateView(const QString &time_str, double time);
    
    /**
     * @brief 更新原始视频数据
     */
    void SetRawData(const cav::CImageData &raw_data)
    {
        raw_data_ = raw_data;
    }

    /**
     * @brief 更新全局语义分割数据
     */
    void SetSematicData(const cav::CImageData &semantic_data)
    {
        semantic_data_ = semantic_data;
    }

    /**
     * @brief 更新车道线语义分割数据
     */
    void SetLaneSematicData(const cav::CImageData &lane_semantic_data)
    {
        lane_semantic_data_ = lane_semantic_data;
    }

    /**
     * @brief 更新当前视图目标框信息
     */
    void SetObjectView(const QVector<cav::CObjBoxCV> &object_data)
    {
        object_data_ = object_data;
    }

    /**
     * @brief 更新当前视图车道线点集坐标数据
     */
    void SetLaneCVView(const QVector<cav::CSDALineCVBlocks> &lane_cv_blocks)
    {
        lane_cv_blocks_ = lane_cv_blocks;
    }

    /**
     * @brief 更新当前视图车道线RLE数据
     */
    void SetLaneRleView(const QVector<cav::CSDALineRleBlocks> &lane_rle_blocks)
    {
        lane_rle_blocks_ = lane_rle_blocks;
    }

private slots:
    void HandleActAddCamera();

private:
    /**
     * @brief 初始化窗口布局
     */
    void InitUi();

    /**
     * @brief 更新全局语义分割显示与否
     */
    void HandleSemanticActionChanged();

    /**
     * @brief 更新车道线语义分割显示与否
     */
    void HandleLaneSemanticActionChanged();

    /**
     * @brief 更新目标框集显示与否
     */
    void HandleObjectBoxActionChanged();

    /**
     * @brief 更新车道线点集显示与否
     */
    void HandleLaneLineActionChanged();

    /**
    * @brief 更新语义分割图(包含车道线语义分割图)
    * @param img 原始图像数据 
    * @param img_data 编码后语义分割图
    */
    void UpdateSemantic(cv::Mat &img, std::vector<unsigned char> &img_data);

    /**
     * @brief 更新更新目标框图; 需在语义分割后绘制!
     * @param img 原始图像数据
     * @param blocks 目标框图点集合(top_left and bottom right)
     */
    void UpdateObjectBox(cv::Mat &img, QVector<cav::CObjBoxCV> &blocks);

    /**
     * @brief 更新车道线点集
     * @param img 原始图像数据
     * @param blocks 车道线原图点集合，包含车道线类型
     */
    void UpdateLinePoints(cv::Mat &img, QVector<cav::CSDALineCVBlocks> &blocks);
    
    /**
     * @brief 更新车道线语义分割图
     * @param img 原始图像数据
     * @param blocks 车道线语义分割集合，包含车道线类型
     */
    void UpdateLineRLE(cv::Mat &img, QVector<cav::CSDALineRleBlocks> &blocks);

    /**
     * @brief 在图像上绘制点
     * @param img 原始图像数据
     * @param points 需要绘制的点集
     * @param color 点集颜色设置
     */
    void PaintPoint(cv::Mat &img, const QVector<cv::Point> &points, cv::Scalar color);
    /**
     * @brief 在图像上绘制线（包含判断线类型）
     * @param img 原始图像数据
     * @param line 需要绘制的线集，包含线类型及单线点集
     * @param color 点集颜色设置
     */
    void PaintLine(cv::Mat &img, const cav::CSDALineCV &line, cv::Scalar color);

private:
    QToolBar *vision_tool_ = nullptr; //视觉工具栏
    CCamera *camera_ = nullptr; // 视觉视频输出口
    CSlider *slider_ = nullptr;

    QAction *act_semantic_ = nullptr; //全局语义分割显示与否
    QAction *act_lane_semantic_ = nullptr; //车道线语义分割显示与否
    QAction *act_obj_boundingbox_ = nullptr; //车道线语义分割显示与否
    QAction *act_lane_line_ = nullptr;     //车道线点集显示与否
    bool semantic_show_ = false;
    bool lane_semantic_show_ = false;
    bool obj_boundingbox_show_ = false;
    bool lane_line_show_ = false;


    cav::CImageData raw_data_; //原始视频数据
    cav::CImageData semantic_data_;  //全局语义分割数据
    cav::CImageData lane_semantic_data_; //车道线语义分割数据
    QVector<cav::CObjBoxCV> object_data_; //视觉目标数据

    QVector<cav::CSDALineCVBlocks> lane_cv_blocks_;
    QVector<cav::CSDALineRleBlocks> lane_rle_blocks_;

    QMenu *camera_menu_ = nullptr;
    QString name_;
    cv::Mat last_mat_data_;
    QString last_time_str_;
    double last_time_;

signals:
    void SigCameraClose();
};

#endif // CVISION_WIDGET_H
