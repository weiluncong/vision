#include "cglobal_param.h"

DEFINE_bool(v_online, true, "run cav as online mode");
DEFINE_bool(v_total_record, false, "record total data to dat");
DEFINE_bool(v_parser, true, "parser flag");
DEFINE_bool(v_point_record, false, "record some data to dat");
DEFINE_uint32(v_model_setting, 0, "connect mode is soc1");
DEFINE_uint32(v_camera_num, 5, "define usb camera num is 5");
DEFINE_double(v_capilot_version, 3.1, "capilot version");
DEFINE_double(v_total_record_size, 800 * 1e6, "set the dat size of totally record data");
DEFINE_double(v_point_record_size, 1 * 1e6, "set the dat size of point record data");
DEFINE_string(v_data_address_pc, "tcp://127.0.0.1:7890", "define init pc zmq address");
DEFINE_string(v_proto_address_pc, "tcp://127.0.0.1:7766", "define init pc zmq proto address");
DEFINE_string(v_data_address_soc1, "tcp://192.168.5.111:7890", "define init soc1 zmq address");
DEFINE_string(v_proto_address_soc1, "tcp://192.168.5.111:7766", "define init soc1 zmq proto address");
DEFINE_string(v_data_address_soc2, "tcp://192.168.1.202:7890", "define init soc2 zmq address");
DEFINE_string(v_proto_address_soc2, "tcp://192.168.1.202:7766", "define init soc2 zmq proto address");