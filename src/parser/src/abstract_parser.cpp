#include "abstract_parser.h"
#include "cglobal_param.h"
#include "csignal_manager.h"

#include <sstream>
using namespace std;

std::string AbstractParser::FieldTypeConvert(const google::protobuf::Message &msg, const google::protobuf::FieldDescriptor *field)
{
    ostringstream os;
    switch (field->cpp_type())
    {
    case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
        os << msg.GetReflection()->GetEnumValue(msg, field);
        break;
    case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
        os << msg.GetReflection()->GetInt32(msg, field);
        break;
    case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
        os << msg.GetReflection()->GetInt64(msg, field);
        break;
    case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
        os << msg.GetReflection()->GetUInt32(msg, field);
        break;
    case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
        os << msg.GetReflection()->GetUInt64(msg, field);
        break;
    case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
        os << fixed << msg.GetReflection()->GetDouble(msg, field);
        break;
    case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
        os << msg.GetReflection()->GetFloat(msg, field);
        break;
    case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
        os << msg.GetReflection()->GetBool(msg, field);
        break;
    case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
        os << msg.GetReflection()->GetString(msg, field);
        break;
    case google::protobuf::FieldDescriptor::TYPE_BYTES:
    {
        std::string data;
        msg.GetReflection()->GetStringReference(msg, field, &data);
        os << data;
    }
    break;
    default:
        break;
    }
    return os.str();
}

QString AbstractParser::GetRepeatedMsg(const google::protobuf::Message &msg, const google::protobuf::FieldDescriptor *field, int index)
{
    QString res;
    switch (field->type())
    {
    case google::protobuf::FieldDescriptor::TYPE_DOUBLE:
    {
        double value = msg.GetReflection()->GetRepeatedDouble(msg, field, index);
        res = QString::number(value, 'g', 10);
    }
    break;
    case google::protobuf::FieldDescriptor::TYPE_FLOAT:
    {
        float value = msg.GetReflection()->GetRepeatedFloat(msg, field, index);
        res = QString::number(value, 'f', 10);
    }
    break;
    case google::protobuf::FieldDescriptor::TYPE_INT64:
    {
        int64_t value = msg.GetReflection()->GetRepeatedInt64(msg, field, index);
        res = QString::number(value);
    }
    break;
    case google::protobuf::FieldDescriptor::TYPE_UINT64:
    {
        uint64_t value = msg.GetReflection()->GetRepeatedUInt64(msg, field, index);
        res = QString::number(value);
    }
    break;
    case google::protobuf::FieldDescriptor::TYPE_INT32:
    {
        int value = msg.GetReflection()->GetRepeatedInt32(msg, field, index);
        res = QString::number(value);
    }
    break;
    case google::protobuf::FieldDescriptor::TYPE_UINT32:
    {
        uint value = msg.GetReflection()->GetRepeatedUInt32(msg, field, index);
        res = QString::number(value);
    }
    break;
    case google::protobuf::FieldDescriptor::TYPE_BOOL:
    {
        bool value = msg.GetReflection()->GetRepeatedBool(msg, field, index);
        res = QString::number(value);
    }
    break;
    case google::protobuf::FieldDescriptor::TYPE_STRING:
    {
        std::string value = msg.GetReflection()->GetRepeatedString(msg, field, index);
        res = QString::fromStdString(value);
    }
    break;
    case google::protobuf::FieldDescriptor::TYPE_GROUP:
    case google::protobuf::FieldDescriptor::TYPE_MESSAGE:
    case google::protobuf::FieldDescriptor::TYPE_BYTES:
    case google::protobuf::FieldDescriptor::TYPE_ENUM:
    case google::protobuf::FieldDescriptor::TYPE_SFIXED32:
    case google::protobuf::FieldDescriptor::TYPE_SFIXED64:
    case google::protobuf::FieldDescriptor::TYPE_SINT32:
    case google::protobuf::FieldDescriptor::TYPE_FIXED64:
    case google::protobuf::FieldDescriptor::TYPE_FIXED32:
    case google::protobuf::FieldDescriptor::TYPE_SINT64:
        break;
    default:
        break;
    }
    return res;
}

void AbstractParser::ParseFinished(const QString &type, double timestamp)
{
    if (FLAGS_v_online) //在线模式
    {
        emit CSignalManager::GetCSignalManager()->SigParseFinished(type, timestamp); 
    }
}
