
// net-tcpDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "CANFDNET.h"
#include "zlgcan/zlgcan.h"

// Cusbcanfdx00udemoDlg 对话框
class CnettcpDlg : public CDialog
{
// 构造
public:
	CnettcpDlg(CWnd* pParent = NULL);	// 标准构造函数
// 对话框数据
	enum { IDD = IDD_USBCANFDX00UDEMO_DIALOG };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	void OnRecv();
// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonInitcan();
	afx_msg void OnBnClickedButtonStartcan();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnCbnSelchangeComboMode();
	afx_msg void OnCbnSelchangeComboDevice();
    afx_msg void OnBnClickedCheckTxEcho();
    afx_msg void OnBnClickedButtonSetDevTimestamp();
    afx_msg void OnBnClickedButtonGetDelaySendAvailableCount();
    afx_msg void OnBnClickedButtonGetAutoSendInfo();
    afx_msg void OnBnClickedCheckDelayMode();
    afx_msg void OnBnClickedButtonDevInfo();
    afx_msg void OnBnClickedButtonCancelDelaySend();
    afx_msg void OnBnClickedButtonSetAutoSend();
    afx_msg void OnBnClickedButtonCancelAutoSend();
    afx_msg void OnBnClickedButtonStateSysInfo();
    afx_msg void OnBnClickedButtonStateCanInfo();
    afx_msg void OnBnClickedButtonStateRecorder();
    afx_msg void OnBnClickedButtonStateNet();
    afx_msg void OnBnClickedCheckTimeUnit();
    afx_msg void OnBnClickedButtonCancelSingle();
    afx_msg void OnBnClickedCheckLinMaster();
    afx_msg void OnCbnSelchangeComboLinVersion();
    afx_msg void OnBnClickedButtonInitLin();
    afx_msg void OnBnClickedButtonStartLin();
    afx_msg void OnBnClickedButtonResetLin();
    afx_msg void OnBnClickedButtonSendLin();
    afx_msg void OnBnClickedButtonClearSlaveIdResponse();
    afx_msg void OnBnClickedButtonGetLinFifoInfo();
    afx_msg void OnBnClickedCheckRecvMerge();
    afx_msg void OnBnClickedButtonSendMerge();
    afx_msg void OnBnClickedCheckRecvData();

	DECLARE_MESSAGE_MAP()
private:
	void EnableCtrl(BOOL opened);
	static UINT WINAPI OnDataRecv(LPVOID data);
	void AddData(const ZCAN_Receive_Data* data, UINT len);
	void AddData(const CString& data);
    void AddData(const ZCAN_ReceiveFD_Data* data, UINT len);
    void AddData(const ZCAN_GPS_FRAME* data, UINT len);
    void AddData(const ZCAN_LIN_MSG* data, UINT len);
    void AddData(const ZCANDataObj* data, UINT len);
	void InitCombobox( int ctrl_id, int start, int end, int current );
	BOOL SetWorkMode();
	BOOL SetClientParam();
	BOOL SetServerParam();
    CString FormatVersion(USHORT uVersion);
    void ShowDevInfo(ZCAN_DEVICE_INFO& denInfo);

private:
	int device_type_index_;
	int device_index_;
    int device_tcp_;
	int work_mode_index_;
	int frame_type_index_;
	int format_index_;
	int send_type_index_;
	CListBox data_recv_list_;
	CString id_;
	CString datas_;
	CComboBox ctrl_device_type_;
	CComboBox ctrl_device_index_;
	DEVICE_HANDLE device_handle_;
    CHANNEL_HANDLE channel_handle_;
    CHANNEL_HANDLE lin_channel_handle_;
	IProperty* property_;
	BOOL start_;
    BOOL recv_thread_running_;
	CButton ctrl_open_device_;
	CButton ctrl_int_can_;
	CButton ctrl_start_can_;
	CButton ctrl_close_device_;
	BOOL device_opened_;
	UINT local_port_;
	CString ip_;
	UINT work_port_;
	int channel_index_;
    CButton m_checkTxEcho;

    ULONGLONG m_nDevTimeStamp;
    CButton m_bthShowBusUsage;
    int send_count_once_;
    int send_count_repeat_;
    CButton m_btnShowData;
    int protocol_index_;
    int canfd_exp_index_;
    int sendtime_{500};
    CButton m_checkDelaySend;
    CButton m_checkTimeUnit;
    CStatic m_staticTimeUnit;
    CButton m_checkLINMaster;
    CButton m_checkLINEnhanceChksum;
    CButton m_checkLINVarDLC;
    CComboBox m_comboLINBaud;
    CComboBox m_comboLINVersion;
    CButton ctrl_int_lin_;
    CButton ctrl_start_lin_;
    CButton ctrl_reset_lin_;
    bool    lin_mode_master_;
    CButton m_btnRecvData;
    BOOL    m_bRecvData;
    BOOL    m_bRecvMerge;
    CButton m_checkRecvMerge;


};
