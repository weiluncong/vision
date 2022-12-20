
#include "cstm_error_message.h"

CStmErrorMessage::CStmErrorMessage() = default;

CStmErrorMessage::~CStmErrorMessage() = default;

CStmErrorMessage *CStmErrorMessage::p = new CStmErrorMessage();

string CStmErrorMessage::GetErrorMessage(const string &signalName, int errorCode) {
    return p->GetMessage(signalName, errorCode);
}

QString CStmErrorMessage::GetErrorMessage(const QString &signalName, int errorCode) {
    QString qSignalName = QString(signalName);
    string errorMessage = GetErrorMessage(qSignalName.toStdString(), errorCode);

    return QString::fromStdString(errorMessage);
}

bool CStmErrorMessage::ContainSignalName(const string &signalName) {
    auto iterator = p->signal_error_map_.find(signalName);

    return iterator != p->signal_error_map_.end();
}

bool CStmErrorMessage::ContainSignalName(const QString &signalName) {
    QString qSignalName = QString(signalName);

    return ContainSignalName(qSignalName.toStdString());
}


string CStmErrorMessage::GetMessage(const string &signalName, int errorCode) {
    string errorMessage;

    //检查信号名称
    auto outerItr = signal_error_map_.find(signalName);
    if (outerItr == signal_error_map_.end()) {
        return errorMessage;
    }

    //检查错误信息代码
    unordered_map<int, string> errorMap = outerItr->second;
    vector<int> bitErrorCodes = GetErrorCodeByBit(signalName, errorCode);

    //多个错误信息用逗号隔开
    for (const auto &bitErrorCode : bitErrorCodes) {
        auto innerItr = errorMap.find(bitErrorCode);
        if (innerItr != errorMap.end()) {
            if (!errorMessage.empty()) {
                errorMessage.append(", ");
            }

            errorMessage.append(innerItr->second);
        }
    }

    return errorMessage.empty() ? to_string(errorCode) : errorMessage;
}

vector<int> CStmErrorMessage::GetErrorCodeByBit(const string &signalName, int errorCode) {
    vector<int> errorCodes;

    //不是按位的不做处理
    if (errorCode < 0 || signal_name_to_check_bit_error_code_set_.find(signalName) == signal_name_to_check_bit_error_code_set_.end()) {
        errorCodes.push_back(errorCode);
    } else {
        unsigned int j = 1;
        //int类型为32位
        static const int countOfPosition = sizeof(int) * 8;

        //获取位
        for (int position = 0; position < countOfPosition; position++) {

            //判当前位是是否是1
            if ((errorCode & j) != 0) {
                errorCodes.push_back(position);
            }

            j = j << 1;
        }
    }

    return errorCodes;
}




