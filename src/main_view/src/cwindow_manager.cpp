#include "cwindow_manager.h"
#include "cmain_window.h"

CWindowManager::CWindowManager(CMainWindow *main_window, QObject *parent)
    : QObject(parent)
{
    main_window_ = main_window;
    data_producer_ = new CDataProducer(this);
    data_scheduler_ = CDataScheduler::GetCDataScheduler();
    signal_manager_ = CSignalManager::GetCSignalManager();
    connect(signal_manager_, &CSignalManager::SigLoadFinished, this, &CWindowManager::HandleDataLoadFinish);
}

CWindowManager::~CWindowManager()
{
    SAFE_DELETE(data_producer_);
    SAFE_DELETE(data_total_record_);
}

/** @brief 模式切换*/
void CWindowManager::HandleActModel()
{
    if (main_window_->act_switch_->text().contains("stop"))
        HandleActSwitch();

    if (FLAGS_v_online)
    {
        FLAGS_v_online = false;
        main_window_->act_mode_->setIcon(QIcon(":/icon/offline.png"));
        main_window_->act_open_file_->setEnabled(true);
        main_window_->act_switch_->setEnabled(false);
        if (FLAGS_v_total_record)
            HandleActDataTotalRecord();
        if (FLAGS_v_point_record)
            HandleActDataPointRecord();
        main_window_->act_total_record_->setEnabled(false);
        main_window_->act_point_record_->setEnabled(false);
        main_window_->main_widget_->mdi_area_->closeAllSubWindows();
    }
    else
    {
        FLAGS_v_online = true;
        main_window_->act_mode_->setIcon(QIcon(":/icon/online.png"));
        main_window_->act_open_file_->setEnabled(false);
        main_window_->act_switch_->setEnabled(true);
        main_window_->act_total_record_->setEnabled(true);
        main_window_->act_point_record_->setEnabled(true);
        data_scheduler_->StopPlay(true);
        main_window_->main_widget_->mdi_area_->closeAllSubWindows();
        main_window_->setWindowTitle("CaVision");
    }
    data_producer_->Switch();
}

/** @brief 离线模式打开数据地址*/
void CWindowManager::HandleActOpenFile()
{
    QString path = QFileDialog::getOpenFileName(main_window_, "select file",
                                                file_path_, "DAT Files (*.dat)");
    if (!path.isEmpty())
    {
        if (main_window_->act_switch_->text().contains("stop"))
            HandleActSwitch();
        main_window_->act_open_file_->setEnabled(false);
        main_window_->act_mode_->setEnabled(false);
        main_window_->setWindowTitle(path.split("/").back());
        data_producer_->OpenFile(path);
    }
}

/** @brief 在线或离线模式下点击播放按钮*/
void CWindowManager::HandleActSwitch()
{
    static bool switch_flag = true;
    if (switch_flag)
    {
        switch_flag = false;
        main_window_->act_switch_->setIcon(QIcon(":/icon/stop.png"));
        main_window_->act_switch_->setStatusTip("stop to play data");
        main_window_->act_switch_->setText("stop");
        if (FLAGS_v_online)
        {
            data_producer_->StartReceive();
        }
        else
        {
            data_scheduler_->StartPlay();
        }
    }
    else
    {
        switch_flag = true;
        main_window_->act_switch_->setIcon(QIcon(":/icon/start.png"));
        main_window_->act_switch_->setStatusTip("start to play data");
        main_window_->act_switch_->setText("start");
        if (FLAGS_v_online)
        {
            data_producer_->PauseReceive();
        }
        else
        {
            data_scheduler_->StopPlay(false);
        }
    }
}

void CWindowManager::HandleDataLoadFinish()
{
    data_scheduler_->StopPlay(true);
    main_window_->main_widget_->mdi_area_->closeAllSubWindows();
    main_window_->act_mode_->setEnabled(true);
    main_window_->act_switch_->setEnabled(true);
    main_window_->act_open_file_->setEnabled(true);
}

void CWindowManager::HandleActDataTotalRecord()
{
    FLAGS_v_total_record = !FLAGS_v_total_record;
    if (FLAGS_v_total_record)
    {
        QString record_file_path = QFileDialog::getExistingDirectory(main_window_, "select file path", total_record_address_,
                                                                     QFileDialog::DontUseNativeDialog | QFileDialog::ShowDirsOnly);
        if (!record_file_path.isEmpty())
        {
            main_window_->act_point_record_->setEnabled(false);
            main_window_->act_total_record_->setIcon(QIcon(":/icon/record_.png"));
            if (!data_total_record_)
            {
                data_total_record_ = new CDataTotalRecord(record_file_path);
                data_total_record_->StartRecord();
            }
            total_record_address_ = record_file_path;
        }
    }
    else
    {
        main_window_->act_point_record_->setEnabled(true);
        main_window_->act_total_record_->setIcon(QIcon(":/icon/record.png"));
        data_total_record_->ReleaseRecord();
        SAFE_DELETE(data_total_record_);
    }
}

void CWindowManager::HandleActDataPointRecord()
{
    FLAGS_v_point_record = !FLAGS_v_point_record;
    if (FLAGS_v_point_record)
    {
        connect(main_window_->act_once_point_record_, &QAction::triggered,
                this, &CWindowManager::HandleActOnceDataPointRecord);
        QString record_file_path = QFileDialog::getExistingDirectory(main_window_, "select file path", point_record_address_,
                                                                     QFileDialog::DontUseNativeDialog | QFileDialog::ShowDirsOnly);

        if (!record_file_path.isEmpty())
        {
            /** @brief create dir to save .dat*/
            QString record_file_name = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
            record_file_path += "/" + record_file_name;
            QDir dir;
            if (!dir.exists(record_file_path))
            {
                if (!dir.mkdir(record_file_path))
                {
                    qDebug() << QString("create dir(%1) failed").arg(record_file_path);
                    return;
                }
            }
            main_window_->act_point_record_->setIcon(QIcon(":/icon/point_record.png"));
            main_window_->act_total_record_->setEnabled(false);
            if (!data_point_record_)
            {
                data_point_record_ = new CDataPointRecord(record_file_path);
            }
            point_record_address_ = record_file_path;
        }
    }
    else
    {
        main_window_->act_total_record_->setEnabled(true);
        main_window_->act_point_record_->setIcon(QIcon(":/icon/point_record_.png"));
        disconnect(main_window_->act_once_point_record_, &QAction::triggered,
                   this, &CWindowManager::HandleActOnceDataPointRecord);
        SAFE_DELETE(data_point_record_);
    }
}

void CWindowManager::HandleActOnceDataPointRecord()
{
    data_point_record_->StartPointRecord();
}