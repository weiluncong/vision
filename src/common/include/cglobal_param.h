#ifndef CGLOBAL_PARM_H
#define CGLOBAL_PARM_H

#include <QtWidgets>
#include <sstream>
#include <iostream>
#include <gflags/gflags.h>

#define TOQSTR(x) QString::fromStdString(x)
#define TOSTR(x) QString(x).toStdString()

#define SAFE_DELETE(x)   \
    {                    \
        if (x)           \
        {                \
            delete x;    \
            x = nullptr; \
        }                \
    }

template <typename T>
void SafeClear(T &container)
{
    T empty;
    empty.swap(container);
}

template <typename T>
T StringToAny(const std::string &str)
{
    std::istringstream iss(str);
    T num;
    iss >> num;
    return num;
}

DECLARE_bool(v_online);
DECLARE_bool(v_total_record);
DECLARE_bool(v_point_record);
DECLARE_bool(v_parser);
DECLARE_uint32(v_model_setting);
DECLARE_uint32(v_camera_num);
DECLARE_double(v_capilot_version);
DECLARE_double(v_total_record_size);
DECLARE_double(v_point_record_size);
DECLARE_string(v_data_address_pc);
DECLARE_string(v_proto_address_pc);
DECLARE_string(v_data_address_soc1);
DECLARE_string(v_proto_address_soc1);
DECLARE_string(v_data_address_soc2);
DECLARE_string(v_proto_address_soc2);

#endif // CGLOBAL_PARM_H
