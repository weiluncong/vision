#include "NetPacket.h"
#include "log.h"
#include <vector>
#include <cassert>
#include <ctime>
#include <cstdlib>
#include "network.h"
#include "3rd/zlib/zlib.h"
#include "common.h"

#ifdef WIN32
#include <Windows.h>
#endif

#ifndef MAKEWORD 
#define MAKEWORD(a, b)      ((uint16_t)(((uint8_t)(((uint32_t)(a)) & 0xff)) | ((uint16_t)((uint8_t)(((uint32_t)(b)) & 0xff))) << 8))
#endif

CPacketUtil::CPacketUtil()
{
}

CPacketUtil::~CPacketUtil()
{
}

uint8_t CPacketUtil::CalcCRC(const uint8_t* data, unsigned int len)
{
    uint8_t ret = 0;
    if (nullptr != data && len > 0)
    {
        ret = data[0];
        for (decltype(len) i = 1; i < len; i++)
        {
            ret ^= data[i];
        }
    }
    return ret;
}

bool CPacketUtil::IsValidPacketHead(const Packet& packet)
{
    bool bRet = false;
    uint8_t nPktType = 0;
    unsigned short nPacketDataLen = 0;
    auto size = packet.size();
    if (size >= PACKET_HEAD_LEN)
    {
        const PacketHead* pHead = (PacketHead*)packet.data();
        if (pHead && pHead->nPacketHead == PACKET_HEADER)
        {
            nPktType = pHead->nPacketType;
            nPacketDataLen = cc_ntohs(pHead->nPacketDataLen);
            switch (pHead->nPacketType)
            {
            case PACKET_TYPE_CAN:
                bRet = nPacketDataLen > 0;
                break;
            case PACKET_TYPE_CANFD:
                bRet = nPacketDataLen > 0;
                break;
            case PACKET_TYPE_CAN_AUTO_SEND:
                bRet = nPacketDataLen % FRAME_LEN_AUTO_SEND == 0;
                break;
            case PACKET_TYPE_BUS_USAGE:
                bRet = nPacketDataLen % FRAME_LEN_BUS_USAGE == 0;
                break;
            case PACKET_TYPE_DEV_STATE:
                bRet = nPacketDataLen > 0;
                break;
            case PACKET_TYPE_GPS:
                bRet = nPacketDataLen % FRAME_LEN_GPS == 0;
                break;
            case PACKET_TYPE_LIN:
                bRet = nPacketDataLen > 0;
                break;
            case PACKET_TYPE_AUTH_REQ:
            case PACKET_TYPE_AUTH_RES:
                bRet = nPacketDataLen == FRAME_LEN_AUTH;
                break;
            case PACKET_TYPE_DEV_REQ:
            case PACKET_TYPE_DEV_RES:
                bRet = nPacketDataLen >= 0;
                break;
            case PACKET_TYPE_REQUEST_RESPONSE:
                bRet = nPacketDataLen >= 0;
                break;
            default:
                bRet = false;
                break;
            }
        }
    }

    if (!bRet)
    {
        LOG_ERR("Invalid Head PktType:%d, PktLen:%d size:%d\n", nPktType, nPacketDataLen, size);
    }

    return bRet;
}

bool CPacketUtil::GetPacket(PacketBuffer& dequeBuffer, Packet& packet)
{
    while (dequeBuffer.size() > 0)
    {
        uint8_t f = dequeBuffer.front();
        if (f == PACKET_HEADER)
        {
            break;
        }
        else
        {
            //LOG_ERR("Drop: %02X\n", f);
            dequeBuffer.pop_front();
        }
    }

    auto size = dequeBuffer.size();
    if (size < PACKET_LEN_MIN)
    {
        return false;
    }

    Packet vecHead;
    if (!dequeBuffer.read(vecHead, PACKET_HEAD_LEN))
    {
        LOG_ERR("buf size:%d, request packet head failed\n", size);
        return false;
    }
    if (IsValidPacketHead(vecHead))
    {
        decltype(size) nPacketLen = PACKET_HEAD_LEN + GetPacketDataLen(vecHead) + PACKET_CRC_LEN;
        if (size < nPacketLen)
        {
            LOG_INFO("buf size:%d, calced pkt len:%d\n", size, nPacketLen);
            return false;
        }
        Packet vecPacket;
        if (!dequeBuffer.read(vecPacket, nPacketLen))
        {
            LOG_ERR("buf size:%d, request len:%d failed\n", size, nPacketLen);
            return false;
        }
        uint8_t nCRC = CalcCRC(vecPacket.data(), nPacketLen - 1);
        uint8_t nPktCRC = vecPacket.back();
        if (nCRC == nPktCRC)
        {
            packet.swap(vecPacket);
            dequeBuffer.pop_front(nPacketLen);
            return true;
        }
        else
        {
            LOG_ERR("Pkt CRC expect:0x%02X, now:%02X\n", nCRC, nPktCRC);
#if 0
            DumpData(vecPacket.data(), vecPacket.size());
#endif
        }
    }
    else
    {
        //LOG_ERR("Packet Head Invalid\n");
    }

    uint8_t f = dequeBuffer.front();
    //LOG_ERR("Drop1: %02X\n", f);
    dequeBuffer.pop_front();
    return GetPacket(dequeBuffer, packet);
}

//#include <iostream>
UINT64 CPacketUtil::GetNowTimeStampAsMicrosecond()
{
    static std::chrono::steady_clock::time_point tp_init = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point tp_now = std::chrono::steady_clock::now();
	UINT64 time= std::chrono::duration_cast<std::chrono::microseconds>(tp_now - tp_init).count();
	//UINT32 temp = 4294967295;
	//if (time > temp){
	//	std::cout << "timeover:" <<time<<std::endl;
	//}
	return time;
}

uint8_t CPacketUtil::GetPacketFrameType(const Packet& packet)
{
    uint8_t ret = 0xFF;
    if (IsValidPacketHead(packet))
    {
        const PacketHead* pHead = (PacketHead*)packet.data();
        ret = pHead->nPacketType;
    }
    return ret;
}

uint8_t CPacketUtil::GetPacketFrameTypeParam(const Packet& packet)
{
    uint8_t ret = 0xFF;
    if (IsValidPacketHead(packet))
    {
        const PacketHead* pHead = (PacketHead*)packet.data();
        ret = pHead->nPacketTypeParam;
    }
    return ret;
}

int CPacketUtil::GetPacketFrameCount(const Packet& packet)
{
    int ret = 0;
    uint8_t type = GetPacketFrameType(packet);
    int len = GetPacketDataLen(packet);
    if (type == PACKET_TYPE_CAN)
    {
        ret = len / FRAME_LEN_CAN;
    }
    else if (type == PACKET_TYPE_CANFD)
    {
        ret = len / FRAME_LEN_CANFD;
    }
    else if (type == PACKET_TYPE_AUTH_REQ ||
        type == PACKET_TYPE_AUTH_RES)
    {
        ret = len / FRAME_LEN_AUTH;
    }
    else if (type == PACKET_TYPE_CAN_AUTO_SEND)
    {
        ret = len / FRAME_LEN_AUTO_SEND;
    }
    else if (type == PACKET_TYPE_BUS_USAGE)
    {
        ret = len / FRAME_LEN_BUS_USAGE;
    }
    else if (type == PACKET_TYPE_GPS)
    {
        ret = len / FRAME_LEN_GPS;
    }
    else if (type == PACKET_TYPE_LIN)
    {
        ret = len / FRAME_LEN_LIN;
    }
    return ret;
}

int CPacketUtil::GetPacketDataLen(const Packet& packet)
{
    int ret = 0;
    if (IsValidPacketHead(packet))
    {
        const PacketHead* pHead = (PacketHead*)packet.data();
        ret = cc_ntohs(pHead->nPacketDataLen);
    }
    return ret;
}

bool CPacketUtil::IsPacketCompressed(const Packet& packet)
{
    uint8_t type = GetPacketFrameType(packet);
    uint8_t typeParam = GetPacketFrameTypeParam(packet);
    if (PACKET_TYPE_CAN == type ||
        PACKET_TYPE_CANFD == type)
    {
        return (typeParam & PACKET_TYPE_PARAM_FLAG_COMPRESS) != 0;
    }
    return false;
}

int CPacketUtil::GetPacketDataCAN(const Packet& packet, std::vector<PacketDataCAN>& vecFrame)
{
    int nRet = 0;
    if (PACKET_TYPE_CAN == GetPacketFrameType(packet))
    {
        auto itrDatabegin = packet.begin();
        vecFrame.clear();
        FrameData frmData;
        PacketDataCAN pktCAN;
        static Packet packetDecompress;
        if (IsPacketCompressed(packet))
        {
            int pktDatalen = GetPacketDataLen(packet);
            uLong cLen = PACKET_DATA_LEN_DECOMPRESS_MAX;
            if (packetDecompress.size() < cLen)
            {
                packetDecompress.resize(cLen, 0);
            }
            memset(packetDecompress.data(), 0, cLen);
            int ret = uncompress((Bytef*)&packetDecompress[0], &cLen, (const Bytef*)&packet[PACKET_HEAD_LEN], pktDatalen);
            if (ret != Z_OK)
            {
                LOG_ERR("uncompress packet data failed ret:(%d)\n", ret);
                return nRet;
            }
            LOG_INFO("uncompress packet data %d to %d\n", pktDatalen, cLen);
#if 0
            DumpData(packetDecompress.data(), cLen);
#endif
            assert(cLen % FRAME_LEN_CAN == 0);
            nRet = cLen/FRAME_LEN_CAN;
            itrDatabegin = packetDecompress.begin();
        }
        else
        {
            nRet = GetPacketFrameCount(packet);
            itrDatabegin = packet.begin() + PACKET_HEAD_LEN;
        }

        for (int i = 0; i < nRet; ++i)
        {
            frmData.assign(itrDatabegin, itrDatabegin + FRAME_LEN_CAN);
            if (GetPacket(frmData, pktCAN))
            {
                vecFrame.push_back(pktCAN);
            }
            else
            {
                LOG_ERR("GetPacket Failed!\n");
            }

            std::advance(itrDatabegin, FRAME_LEN_CAN);
        }
    }
    else
    {
        LOG_ERR("GetPacketFrameType CAN Mismatch!\n");
    }
    return nRet;
}

int CPacketUtil::GetPacketDataCANFD(const Packet& packet, std::vector<PacketDataCANFD>& vecFrame)
{
    int nRet = 0;
    if (PACKET_TYPE_CANFD == GetPacketFrameType(packet))
    {
        auto itrDatabegin = packet.begin();
        vecFrame.clear();
        FrameData frmData;
        PacketDataCANFD pktCANFD;
        static Packet packetDecompress;
        if (IsPacketCompressed(packet))
        {
            int pktDatalen = GetPacketDataLen(packet);
            uLong cLen = PACKET_DATA_LEN_DECOMPRESS_MAX;
            if (packetDecompress.size() < cLen)
            {
                packetDecompress.resize(cLen, 0);
            }
            memset(packetDecompress.data(), 0, cLen);
            int ret = uncompress((Bytef*)&packetDecompress[0], &cLen, (const Bytef*)&packet[PACKET_HEAD_LEN], pktDatalen);
            if (ret != Z_OK)
            {
                LOG_ERR("uncompress packet data failed ret:(%d)\n", ret);
                return nRet;
            }
            LOG_INFO("uncompress packet data %d to %d\n", pktDatalen, cLen);
#if 0
            DumpData(packetDecompress.data(), cLen);
#endif
            assert(cLen % FRAME_LEN_CANFD == 0);
            nRet = cLen / FRAME_LEN_CANFD;
            itrDatabegin = packetDecompress.begin();
        }
        else
        {
            nRet = GetPacketFrameCount(packet);
            itrDatabegin = packet.begin() + PACKET_HEAD_LEN;
        }

        for (int i = 0; i < nRet; ++i)
        {
            frmData.assign(itrDatabegin, itrDatabegin + FRAME_LEN_CANFD);
            if (GetPacket(frmData, pktCANFD))
            {
                vecFrame.push_back(pktCANFD);
            }
            else
            {
                LOG_ERR("GetPacket Failed!\n");
            }

            std::advance(itrDatabegin, FRAME_LEN_CANFD);
        }
    }
    else
    {
        LOG_ERR("GetPacketFrameType CANFD Mismatch!\n");
    }
    return nRet;
}

int CPacketUtil::GetPacketDataGPS(const Packet& packet, std::vector<PacketDataGPS>& vecFrame)
{
    int nRet = 0;
    if (PACKET_TYPE_GPS == GetPacketFrameType(packet))
    {
        nRet = GetPacketFrameCount(packet);
        vecFrame.clear();
        FrameData frmData;
        PacketDataGPS pktGPS;
        auto itrDatabegin = packet.begin() + PACKET_HEAD_LEN;
        for (int i = 0; i < nRet; ++i)
        {
            frmData.assign(itrDatabegin, itrDatabegin + FRAME_LEN_GPS);
            if (GetPacket(frmData, pktGPS))
            {
                vecFrame.push_back(pktGPS);
            }
            else
            {
                LOG_ERR("GetPacket GPS Failed!\n");
            }

            std::advance(itrDatabegin, FRAME_LEN_GPS);
        }
    }
    else
    {
        LOG_ERR("GetPacketFrameType GPS Mismatch!\n");
    }
    return nRet;
}

int CPacketUtil::GetPacketDataLIN(const Packet& packet, std::vector<PacketDataLIN>& vecFrame)
{
    int nRet = 0;
    if (PACKET_TYPE_LIN == GetPacketFrameType(packet))
    {
        nRet = GetPacketFrameCount(packet);
        vecFrame.clear();
        FrameData frmData;
        PacketDataLIN pktLIN;
        auto itrDatabegin = packet.begin() + PACKET_HEAD_LEN;
        for (int i = 0; i < nRet; ++i)
        {
            frmData.assign(itrDatabegin, itrDatabegin + FRAME_LEN_LIN);
            if (GetPacket(frmData, pktLIN))
            {
                vecFrame.push_back(pktLIN);
            }
            else
            {
                LOG_ERR("GetPacket LIN Failed!\n");
            }

            std::advance(itrDatabegin, FRAME_LEN_LIN);
        }
    }
    else
    {
        LOG_ERR("GetPacketFrameType LIN Mismatch!\n");
    }
    return nRet;
}

int CPacketUtil::GetPacketDataAUTH(const Packet& packet, FrameData& vecAuth, uint8_t& pktTypeParam)
{
    int nRet = 0;
    uint8_t pktType = GetPacketFrameType(packet);
    if (PACKET_TYPE_AUTH_REQ == pktType ||
        PACKET_TYPE_AUTH_RES == pktType )
    {
        nRet = GetPacketFrameCount(packet);
        if (nRet == 1)
        {
            auto itrDatabegin = packet.begin() + PACKET_HEAD_LEN;
            vecAuth.assign(itrDatabegin, itrDatabegin + FRAME_LEN_AUTH);
            const PacketHead* head = (PacketHead*)packet.data();
            pktTypeParam = head->nPacketTypeParam;
        }
        else
        {
            LOG_ERR("Packet Auth FrameCount Invalid : %d\n", nRet);
        }
    }
    else
    {
        LOG_ERR("GetPacketFrameType AUTH_REQ/RES Mismatch!\n");
    }
    return nRet;
}

int CPacketUtil::GetPacketDataBusUsage(const Packet& packet, PacketDataBusUsage& pktBusUsage)
{
    int nRet = 0;
    uint8_t pktType = GetPacketFrameType(packet);
    if (PACKET_TYPE_BUS_USAGE == pktType)
    {
        nRet = GetPacketFrameCount(packet);
        if (nRet == 1)
        {
            FrameData vecBusUsage;
            auto itrDatabegin = packet.begin() + PACKET_HEAD_LEN;
            vecBusUsage.assign(itrDatabegin, itrDatabegin + FRAME_LEN_BUS_USAGE);
            PacketDataBusUsage* pNetOrder = (PacketDataBusUsage*)vecBusUsage.data();
            pktBusUsage.nTimeStampBegin = cc_ntohll(pNetOrder->nTimeStampBegin);
            pktBusUsage.nTimeStampEnd = cc_ntohll(pNetOrder->nTimeStampEnd);
            pktBusUsage.nChnl = pNetOrder->nChnl;
            pktBusUsage.nReserved = pNetOrder->nReserved;
            pktBusUsage.nBusUsage = cc_ntohs(pNetOrder->nBusUsage);
            pktBusUsage.nFrameCount = cc_ntohl(pNetOrder->nFrameCount);
        }
        else
        {
            LOG_ERR("Packet BusUsage FrameCount Invalid : %d\n", nRet);
        }
    }
    else
    {
        LOG_ERR("GetPacketFrameType PACKET_TYPE_BUS_USAGE Mismatch!\n");
    }
    return nRet;
}

int CPacketUtil::GetPacketDataDevReq(const Packet& packet, FrameData& vecBusDevReq, uint8_t& pktTypeParam)
{
    int nRet = 0;
    uint8_t pktType = GetPacketFrameType(packet);
    if (PACKET_TYPE_DEV_REQ == pktType)
    {
        if (packet.size() >= PACKET_HEAD_LEN)
        {
            const PacketHead* head = (PacketHead*)packet.data();
            pktTypeParam = head->nPacketTypeParam;
            PacketTypeREQParam reqParam;
            reqParam.rawValue = pktTypeParam;
            int nDataLen = GetPacketDataLen(packet);
            switch (reqParam.unionValue.nReqVal)
            {
            case PACKET_TYPE_PARAM_DEV_REQ_RES_TIMESTAMP:
                {
                    if (nDataLen == PACKET_TYPE_DEV_REQ_TIMESTAMP_LEN)
                    {
                        nRet = 1;
                        auto itrDatabegin = packet.begin() + PACKET_HEAD_LEN;
                        vecBusDevReq.assign(itrDatabegin, itrDatabegin + nDataLen);
                    }
                    else
                    {
                        LOG_ERR("Packet DataDevReq Request TimeStamp Len Invalid: %d\n", nDataLen);
                    }
                }
                break;
            default:
                LOG_ERR("Packet DataDevReq Request Value Invalid: %d\n", reqParam.unionValue.nReqVal);
                break;
            }
        }
        else
        {
            LOG_ERR("Packet DataDevReq Lenth Invalid : %d\n", packet.size());
        }
    }
    else
    {
        LOG_ERR("GetPacketFrameType GetPacketDataDevReq Mismatch!\n");
    }
    return nRet;
}

int CPacketUtil::GetPacketDataDevRes(const Packet& packet, FrameData& vecBusDevRes, uint8_t& pktTypeParam)
{
    int nRet = 0;
    uint8_t pktType = GetPacketFrameType(packet);
    if (PACKET_TYPE_DEV_RES == pktType)
    {
        if (packet.size() >= PACKET_HEAD_LEN)
        {
            const PacketHead* head = (PacketHead*)packet.data();
            pktTypeParam = head->nPacketTypeParam;
            PacketTypeRESParam resParam;
            resParam.rawValue = pktTypeParam;
            int nDataLen = GetPacketDataLen(packet);
            switch (resParam.unionValue.nReqVal)
            {
            case PACKET_TYPE_PARAM_DEV_REQ_RES_TIMESTAMP:
                {
                    if (nDataLen == PACKET_TYPE_DEV_RES_TIMESTAMP_LEN)
                    {
                        nRet = 1;
                    }
                    else
                    {
                        LOG_ERR("Packet DataDevRes Request TimeStamp Len Invalid: %d\n", nDataLen);
                    }
                }
                break;
            default:
                LOG_ERR("Packet DataDevRes Res Value Invalid: %d\n", resParam.unionValue.nReqVal);
                break;
            }
        }
        else
        {
            LOG_ERR("Packet DataDevReq Lenth Invalid : %d\n", packet.size());
        }
    }
    else
    {
        LOG_ERR("GetPacketFrameType GetPacketDataDevReq Mismatch!\n");
    }
    return nRet;
}

int  CPacketUtil::GetPacketDataRequest(const Packet& packet, PacketDataRequest& request)
{
    int nRet = 0;
    uint8_t pktType = GetPacketFrameType(packet);
    PacketTypeRequestResponseParam pktTypeParam;
    pktTypeParam.rawValue = CPacketUtil::GetPacketFrameTypeParam(packet);
    if (PACKET_TYPE_REQUEST_RESPONSE == pktType && pktTypeParam.unionValue.bResponse == 0)
    {
        int nDataLen = GetPacketDataLen(packet);
        if (nDataLen >= PACKET_REQUEST_RESPONSE_DATALEN_MIN &&
            nDataLen + PACKET_HEAD_LEN + 1 == packet.size())  //crc 1 uint8_t
        {
            FrameData vecData;
            auto itrDatabegin = packet.begin() + PACKET_HEAD_LEN;
            vecData.assign(itrDatabegin, itrDatabegin + nDataLen);
            PacketDataRequest* pNetorder = (PacketDataRequest*)vecData.data();
            request.nTID = pNetorder->nTID;
            request.nRequest = pNetorder->nRequest;
            request.vecData.assign(vecData.begin() + 2, vecData.begin() + nDataLen);  // 2byte tid and result
            nRet = 1;
        }
        else
        {
            LOG_ERR("Packet GetPacketDataRequest Lenth Invalid, pkt.len=%d, pkt.size=%d\n", nDataLen, packet.size());
        }
    }
    else
    {
        LOG_ERR("GetPacketFrameType GetPacketDataRequest Mismatch!\n");
    }
    return nRet;
}

int  CPacketUtil::GetPacketDataResponse(const Packet& packet, PacketDataResponse& response)
{
    int nRet = 0;
    uint8_t pktType = GetPacketFrameType(packet);
    PacketTypeRequestResponseParam pktTypeParam;
    pktTypeParam.rawValue = CPacketUtil::GetPacketFrameTypeParam(packet);
    if (PACKET_TYPE_REQUEST_RESPONSE == pktType && pktTypeParam.unionValue.bResponse)
    {
        int nDataLen = GetPacketDataLen(packet);
        if (nDataLen >= PACKET_REQUEST_RESPONSE_DATALEN_MIN &&
            nDataLen + PACKET_HEAD_LEN + 1 == packet.size())  //crc 1 uint8_t
        {
            FrameData vecData;
            auto itrDatabegin = packet.begin() + PACKET_HEAD_LEN;
            vecData.assign(itrDatabegin, itrDatabegin + nDataLen);
            PacketDataResponse* pNetorder = (PacketDataResponse*)vecData.data();
            response.nTID = pNetorder->nTID;
            response.nResult = pNetorder->nResult;
            response.vecData.assign(vecData.begin() + 2, vecData.begin() + nDataLen);  // 2byte tid and result
            nRet = 1;
        }
        else
        {
            LOG_ERR("Packet GetPacketDataResponse Lenth Invalid, pkt.len=%d pkt.size=%d\n", nDataLen, packet.size());
        }
    }
    else
    {
        LOG_ERR("GetPacketFrameType GetPacketDataResponse Mismatch!\n");
    }
    return nRet;
}

int CPacketUtil::GetPacketDataDevState(const Packet& packet, FrameData& vecDevStat, uint8_t& pktTypeParam)
{
    int nRet = 0;
    uint8_t pktType = GetPacketFrameType(packet);
    if (PACKET_TYPE_DEV_STATE == pktType)
    {
        int nDataLen = GetPacketDataLen(packet);
        if (nDataLen + PACKET_HEAD_LEN + 1 == packet.size())
        {
            const PacketHead* head = (PacketHead*)packet.data();
            pktTypeParam = head->nPacketTypeParam;
            auto itrDatabegin = packet.begin() + PACKET_HEAD_LEN;
            vecDevStat.assign(itrDatabegin, itrDatabegin + nDataLen);
            nRet = 1;
        }
        else
        {
            LOG_ERR("Packet DataDevState Lenth Invalid : %d\n", packet.size());
        }
    }
    else
    {
        LOG_ERR("GetPacketFrameType DataDevState Mismatch!\n");
    }
    return nRet;
}

bool CPacketUtil::GetPacket(const FrameData& pktFrame, PacketDataCAN& pktCAN)
{
    if (pktFrame.size() == sizeof(pktCAN))
    {
        memset(&pktCAN, 0, sizeof(pktCAN));
        PacketDataCAN* pNetOrder = (PacketDataCAN*)pktFrame.data();
        pktCAN.canHead.nID = cc_ntohl(pNetOrder->canHead.nID);
        pktCAN.canHead.nTimestamp = cc_ntohll(pNetOrder->canHead.nTimestamp);
        pktCAN.canHead.frameInfo.rawVal = cc_ntohs(pNetOrder->canHead.frameInfo.rawVal);
        pktCAN.canHead.nChnl = pNetOrder->canHead.nChnl;
        pktCAN.canHead.nDataLen = pNetOrder->canHead.nDataLen;
        memcpy(pktCAN.canData, pNetOrder->canData, sizeof(pktCAN.canData)); 

#if 0
        char buf[100] = { 0 };
        sprintf(buf, "CAN ID:%x, Timestamp net:0x%I64x(%I64u) host:0x%I64x(%I64u)", pktCAN.canHead.nID, pNetOrder->canHead.nTimestamp, pNetOrder->canHead.nTimestamp,
            pktCAN.canHead.nTimestamp, pktCAN.canHead.nTimestamp);
        OutputDebugStringA(buf);
#endif
        return true;
    }
    return false;
}

bool CPacketUtil::GetPacket(const FrameData& pktFrame, PacketDataCANFD& pktCANFD)
{
    if (pktFrame.size() == sizeof(pktCANFD))
    {
        memset(&pktCANFD, 0, sizeof(pktCANFD));
        PacketDataCANFD* pNetOrder = (PacketDataCANFD*)pktFrame.data();
        pktCANFD.canHead.nID = cc_ntohl(pNetOrder->canHead.nID);
        pktCANFD.canHead.nTimestamp = cc_ntohll(pNetOrder->canHead.nTimestamp);
        pktCANFD.canHead.frameInfo.rawVal = cc_ntohs(pNetOrder->canHead.frameInfo.rawVal);
        pktCANFD.canHead.nChnl = pNetOrder->canHead.nChnl;
        pktCANFD.canHead.nDataLen = pNetOrder->canHead.nDataLen;
        memcpy(pktCANFD.canData, pNetOrder->canData, sizeof(pktCANFD.canData));

#if 0
        char buf[100] = { 0 };
        sprintf(buf, "CANFD ID:%x, Timestamp net:0x%I64x(%I64u) host:0x%I64x(%I64u)", pktCANFD.canHead.nID, pNetOrder->canHead.nTimestamp, pNetOrder->canHead.nTimestamp,
            pktCANFD.canHead.nTimestamp, pktCANFD.canHead.nTimestamp);
        OutputDebugStringA(buf);
#endif
        return true;
    }
    return false;
}

bool CPacketUtil::GetPacket(const FrameData& pktFrame, PacketDataGPS& pktGPS)
{
    if (pktFrame.size() == sizeof(pktGPS))
    {
        memset(&pktGPS, 0, sizeof(pktGPS));
        PacketDataGPS* pNetOrder = (PacketDataGPS*)pktFrame.data();
        pktGPS.nTimestamp = cc_ntohll(pNetOrder->nTimestamp);
        pktGPS.nLongitude = cc_ntohl(pNetOrder->nLongitude);
        pktGPS.nLatitude = cc_ntohl(pNetOrder->nLatitude);
        pktGPS.nSpeed = cc_ntohl(pNetOrder->nSpeed);
        pktGPS.nCourseAngle = cc_ntohl(pNetOrder->nCourseAngle);
        pktGPS.nAltitude = cc_ntohl(pNetOrder->nAltitude);
        pktGPS.nReserved = cc_ntohl(pNetOrder->nReserved);

        return true;
    }
    return false;
}

bool CPacketUtil::GetPacket(const FrameData& pktFrame, PacketDataLIN& pktLIN)
{
    if (pktFrame.size() == sizeof(pktLIN))
    {
        memset(&pktLIN, 0, sizeof(pktLIN));
        PacketDataLIN* pNetOrder = (PacketDataLIN*)pktFrame.data();
        pktLIN.nTimestamp = cc_ntohll(pNetOrder->nTimestamp);
        pktLIN.nChnl = (pNetOrder->nChnl);
        pktLIN.stMsgID.nRawID = (pNetOrder->stMsgID.nRawID);
        pktLIN.frameInfo.rawVal = cc_ntohs(pNetOrder->frameInfo.rawVal);
        memcpy(pktLIN.nReserved, pNetOrder->nReserved, sizeof(pktLIN.nReserved));
        pktLIN.nDataLen = (pNetOrder->nDataLen);
        memcpy(pktLIN.nData, pNetOrder->nData, sizeof(pktLIN.nData));
        pktLIN.nChkSum = (pNetOrder->nChkSum);
        return true;
    }
    return false;
}

bool CPacketUtil::GetPacket(const FrameData& pktFrame, PacketDataAutoSend& pktAutoSend)
{
    if (pktFrame.size() == sizeof(pktAutoSend))
    {
        memset(&pktAutoSend, 0, sizeof(pktAutoSend));
        PacketDataAutoSend* pNetOrder = (PacketDataAutoSend*)pktFrame.data();
        pktAutoSend.nIndex = pNetOrder->nIndex;
        pktAutoSend.bEnable = pNetOrder->bEnable;
        pktAutoSend.nPeriodMs = cc_ntohs(pNetOrder->nPeriodMs);
        pktAutoSend.nRepeatCount = cc_ntohs(pNetOrder->nRepeatCount);
        pktAutoSend.stFlag.rawValue = cc_ntohs(pNetOrder->stFlag.rawValue);

        FrameData pktCANFDFrame;
        auto itrData = pktFrame.begin() + offsetof(PacketDataAutoSend, pktData);
        pktCANFDFrame.assign(itrData, itrData + sizeof(pktAutoSend.pktData));
        return GetPacket(pktCANFDFrame, pktAutoSend.pktData);
    }
    return false;
}

void CPacketUtil::ZCANTx2CANFrame(const ZCAN_Transmit_Data& zTx, int nChnl, bool bTxEchoEnable, PacketDataCAN& pktFrame)
{
    memset(&pktFrame, 0, sizeof(pktFrame));
    pktFrame.canHead.nID = GET_ID(zTx.frame.can_id);
    pktFrame.canHead.frameInfo.unionVal.nSendtype = zTx.transmit_type;
    pktFrame.canHead.frameInfo.unionVal.bEXT = IS_EFF(zTx.frame.can_id);
    pktFrame.canHead.frameInfo.unionVal.bFD = 0;
    pktFrame.canHead.frameInfo.unionVal.bRTR = IS_RTR(zTx.frame.can_id);
    pktFrame.canHead.frameInfo.unionVal.bSendDelay = IS_DELAY_SEND(zTx.frame.__pad);
    uint32_t nVal = MAKEWORD(zTx.frame.__res0, zTx.frame.__res1);
    if (IS_DELAY_SEND_TIME_UNIT_MS(zTx.frame.__pad))
    {
        nVal *= 10;
    }
    //nTimestamp低4位表示延时时间，单位100us
    pktFrame.canHead.nTimestamp = pktFrame.canHead.frameInfo.unionVal.bSendDelay ? nVal : GetNowTimeStampAsMicrosecond();
    pktFrame.canHead.nChnl = (uint8_t)nChnl;
    pktFrame.canHead.nDataLen = zTx.frame.can_dlc;
    memcpy(pktFrame.canData, zTx.frame.data, sizeof(pktFrame.canData));

    //TxEcho
    pktFrame.canHead.frameInfo.unionVal.nEcho = bTxEchoEnable ? 1 : IS_TX_ECHO(zTx.frame.__pad);
}

void CPacketUtil::ZCANTx2CANFrame(const ZCAN_TransmitFD_Data& zTx, int nChnl, bool bTxEchoEnable, PacketDataCANFD& pktFrame)
{
    memset(&pktFrame, 0, sizeof(pktFrame));
    pktFrame.canHead.nID = GET_ID(zTx.frame.can_id);
    pktFrame.canHead.frameInfo.unionVal.nSendtype = zTx.transmit_type;
    pktFrame.canHead.frameInfo.unionVal.bEXT = IS_EFF(zTx.frame.can_id);
    pktFrame.canHead.frameInfo.unionVal.bFD = 1;
    pktFrame.canHead.frameInfo.unionVal.bBRS = ((zTx.frame.flags & CANFD_BRS) != 0 ? 1 : 0);
    pktFrame.canHead.frameInfo.unionVal.bRTR = IS_RTR(zTx.frame.can_id);
    pktFrame.canHead.frameInfo.unionVal.bSendDelay = IS_DELAY_SEND(zTx.frame.flags);
    uint32_t nVal = MAKEWORD(zTx.frame.__res0, zTx.frame.__res1);
    if (IS_DELAY_SEND_TIME_UNIT_MS(zTx.frame.flags))
    {
        //用户设置单位为ms，需要转换为100us
        nVal *= 10;
    }
    //nTimestamp低4位表示延时时间，单位100us
    pktFrame.canHead.nTimestamp = pktFrame.canHead.frameInfo.unionVal.bSendDelay ? nVal : GetNowTimeStampAsMicrosecond();
    pktFrame.canHead.nChnl = (uint8_t)nChnl;
    pktFrame.canHead.nDataLen = zTx.frame.len;
    memcpy(pktFrame.canData, zTx.frame.data, sizeof(pktFrame.canData));

    //TxEcho
    pktFrame.canHead.frameInfo.unionVal.nEcho = bTxEchoEnable ? 1 : IS_TX_ECHO(zTx.frame.flags);
}

bool CPacketUtil::ZCANDataObj2CANFrame(const ZCANDataObj& zTx, PacketDataCANFD& pktFrame)
{
    assert(zTx.dataType == ZCAN_DT_ZCAN_CAN_CANFD_DATA);
    if (zTx.dataType != ZCAN_DT_ZCAN_CAN_CANFD_DATA)
    {
        return false;
    }
    memset(&pktFrame, 0, sizeof(pktFrame));
    pktFrame.canHead.nID = GET_ID(zTx.data.zcanCANFDData.frame.can_id);
    pktFrame.canHead.frameInfo.unionVal.nSendtype = zTx.data.zcanCANFDData.flag.unionVal.transmitType;
    pktFrame.canHead.frameInfo.unionVal.bEXT = IS_EFF(zTx.data.zcanCANFDData.frame.can_id);
    pktFrame.canHead.frameInfo.unionVal.bFD = zTx.data.zcanCANFDData.flag.unionVal.frameType;
    pktFrame.canHead.frameInfo.unionVal.bBRS = ((zTx.data.zcanCANFDData.frame.flags & CANFD_BRS) != 0 ? 1 : 0);
    pktFrame.canHead.frameInfo.unionVal.bRTR = IS_RTR(zTx.data.zcanCANFDData.frame.can_id);
    pktFrame.canHead.frameInfo.unionVal.bSendDelay = zTx.data.zcanCANFDData.flag.unionVal.txDelay != ZCAN_TX_DELAY_NO_DELAY;
    UINT64 nTimestamp = zTx.data.zcanCANFDData.timeStamp;
    if (zTx.data.zcanCANFDData.flag.unionVal.txDelay == ZCAN_TX_DELAY_UNIT_MS)
    {
        //用户设置单位为ms，需要转换为100us
        nTimestamp *= 10;
    }
    //nTimestamp低4位表示延时时间，单位100us
    pktFrame.canHead.nTimestamp = pktFrame.canHead.frameInfo.unionVal.bSendDelay ? nTimestamp : GetNowTimeStampAsMicrosecond();
    pktFrame.canHead.nChnl = (uint8_t)zTx.chnl;
    pktFrame.canHead.nDataLen = zTx.data.zcanCANFDData.frame.len;
    memcpy(pktFrame.canData, zTx.data.zcanCANFDData.frame.data, sizeof(pktFrame.canData));

    //TxEcho
    pktFrame.canHead.frameInfo.unionVal.nEcho = zTx.data.zcanCANFDData.flag.unionVal.txEchoRequest;

    return true;
}

void CPacketUtil::CANFrame2ZCANRx(const PacketDataCAN& pktFrame, ZCAN_Receive_Data& zRx, int* pChnl)
{
    assert(pktFrame.canHead.frameInfo.unionVal.bFD == 0);
    memset(&zRx, 0, sizeof(zRx));
    zRx.timestamp = pktFrame.canHead.nTimestamp;
    zRx.frame.can_dlc = pktFrame.canHead.nDataLen;
    zRx.frame.can_id = MAKE_CAN_ID(pktFrame.canHead.nID, pktFrame.canHead.frameInfo.unionVal.bEXT,
        pktFrame.canHead.frameInfo.unionVal.bRTR, pktFrame.canHead.frameInfo.unionVal.bERR);
    memcpy(zRx.frame.data, pktFrame.canData, sizeof(zRx.frame.data));
    if (pktFrame.canHead.frameInfo.unionVal.nTx)
    {
        zRx.frame.__pad |= TX_ECHO_FLAG;
    }

    if (pChnl)
    {
        *pChnl = pktFrame.canHead.nChnl;
    }
}

void CPacketUtil::CANFrame2ZCANRx(const PacketDataCAN& pktFrame, ZCANDataObj& zRx)
{
    //assert(pktFrame.canHead.frameInfo.unionVal.bFD == 0);
    memset(&zRx, 0, sizeof(zRx));
    zRx.dataType = ZCAN_DT_ZCAN_CAN_CANFD_DATA;
    zRx.chnl = pktFrame.canHead.nChnl;
    ZCANCANFDData& canfdData = zRx.data.zcanCANFDData;
    canfdData.flag.unionVal.frameType = TYPE_CAN; //CAN
    canfdData.timeStamp = pktFrame.canHead.nTimestamp;
    canfdData.frame.len = pktFrame.canHead.nDataLen;
    canfdData.frame.can_id = MAKE_CAN_ID(pktFrame.canHead.nID, pktFrame.canHead.frameInfo.unionVal.bEXT,
        pktFrame.canHead.frameInfo.unionVal.bRTR, pktFrame.canHead.frameInfo.unionVal.bERR);
    memcpy(canfdData.frame.data, pktFrame.canData, sizeof(canfdData.frame.data));
    canfdData.flag.unionVal.txEchoed = pktFrame.canHead.frameInfo.unionVal.nTx;
}

void CPacketUtil::CANFrame2ZCANRx(const PacketDataCANFD& pktFrame, ZCAN_ReceiveFD_Data& zRx, int* pChnl, bool bRecvMerge)
{
    memset(&zRx, 0, sizeof(zRx));
    zRx.timestamp = pktFrame.canHead.nTimestamp;
    zRx.frame.len = pktFrame.canHead.nDataLen;
    zRx.frame.can_id = MAKE_CAN_ID(pktFrame.canHead.nID, pktFrame.canHead.frameInfo.unionVal.bEXT,
        pktFrame.canHead.frameInfo.unionVal.bRTR, pktFrame.canHead.frameInfo.unionVal.bERR);
    zRx.frame.flags |= (pktFrame.canHead.frameInfo.unionVal.bESI ? CANFD_ESI : 0);
    zRx.frame.flags |= (pktFrame.canHead.frameInfo.unionVal.bBRS ? CANFD_BRS : 0);
    memcpy(zRx.frame.data, pktFrame.canData, sizeof(zRx.frame.data));
    if (pktFrame.canHead.frameInfo.unionVal.nTx)
    {
        zRx.frame.flags |= TX_ECHO_FLAG;
    }

    if (pChnl)
    {
        *pChnl = pktFrame.canHead.nChnl;
    }

    if (bRecvMerge)
    {
        CANFD_FLAG_RES0 flag;
        flag.nRawValue = 0;
        flag.unionValue.nChnl = pktFrame.canHead.nChnl;
        flag.unionValue.nType = pktFrame.canHead.frameInfo.unionVal.bFD;
        flag.unionValue.nFlagValid = 1;
        zRx.frame.__res0 = flag.nRawValue;
    }
}

void CPacketUtil::CANFrame2ZCANRx(const PacketDataCANFD& pktFrame, ZCANDataObj& zRx)
{
    memset(&zRx, 0, sizeof(zRx));
    zRx.dataType = ZCAN_DT_ZCAN_CAN_CANFD_DATA;
    zRx.chnl = pktFrame.canHead.nChnl;
    ZCANCANFDData& canfdData = zRx.data.zcanCANFDData;
    canfdData.flag.unionVal.frameType = pktFrame.canHead.frameInfo.unionVal.bFD;
    canfdData.timeStamp = pktFrame.canHead.nTimestamp;
    canfdData.frame.len = pktFrame.canHead.nDataLen;
    canfdData.frame.can_id = MAKE_CAN_ID(pktFrame.canHead.nID, pktFrame.canHead.frameInfo.unionVal.bEXT,
        pktFrame.canHead.frameInfo.unionVal.bRTR, pktFrame.canHead.frameInfo.unionVal.bERR);
    canfdData.frame.flags |= (pktFrame.canHead.frameInfo.unionVal.bESI ? CANFD_ESI : 0);
    canfdData.frame.flags |= (pktFrame.canHead.frameInfo.unionVal.bBRS ? CANFD_BRS : 0);
    memcpy(canfdData.frame.data, pktFrame.canData, sizeof(canfdData.frame.data));
    canfdData.flag.unionVal.txEchoed = pktFrame.canHead.frameInfo.unionVal.nTx;
}

void CPacketUtil::CANFrame2CANFDFrame(const PacketDataCAN& pktFrameCAN, PacketDataCANFD& pktFrameCANFD)
{
    memset(&pktFrameCANFD, 0, sizeof(pktFrameCANFD));
    pktFrameCANFD.canHead = pktFrameCAN.canHead;
    memcpy(pktFrameCANFD.canData, pktFrameCAN.canData, sizeof(pktFrameCANFD.canData));
}

void CPacketUtil::CANFDFrame2CANFrame(const PacketDataCANFD& pktFrameCANFD, PacketDataCAN& pktFrameCAN)
{
    memset(&pktFrameCAN, 0, sizeof(pktFrameCAN));
    pktFrameCAN.canHead = pktFrameCANFD.canHead;
    //CANFD->CAN，只拷贝8字节数据
    memcpy(pktFrameCAN.canData, pktFrameCANFD.canData, sizeof(pktFrameCAN.canData));
}

bool CPacketUtil::CANErrorData2ZcanErrInfo(const PacketDataCANHead& canHead, const uint8_t* data, ZCAN_CHANNEL_ERR_INFO& rZCANFrame)
{
    if (!canHead.frameInfo.unionVal.bERR || nullptr == data)
    {
        return false;
    }

    memset(&rZCANFrame, 0, sizeof(rZCANFrame));
    int nLen = canHead.nDataLen;

    ZLGCAN_PASSIVE_ERRORINFO_BYTE passiveErrByte;
    passiveErrByte.nRawValue = 0;
    uint8_t nErrCode = (nLen > 0 ? data[0] : 0);
    uint8_t nTxErr = (nLen > 4 ? data[4] : 0);
    uint8_t nRxErr = (nLen > 3 ? data[3] : 0);
    rZCANFrame.error_code = nErrCode;
    switch (nErrCode)
    {
    case 0xE1:
    {
        //总线错误
        rZCANFrame.error_code = ZCAN_ERROR_CAN_BUSERR;
        uint8_t nBusErrSubcode = data[1];
        switch (nBusErrSubcode)
        {
            /*
            总线错误类型错误值
            位错误0x01
            应答错误0x02
            CRC 错误0x04
            格式错误0x08
            填充错误0x10
            超载错误0x20
            接收缓冲区满0x40
            发送缓冲区满0x80
            */
        case 0x01:
            rZCANFrame.error_code = ZCAN_ERROR_CAN_PASSIVE;
            passiveErrByte.unionValue.nErrCodeType = 0;     //位错误
            passiveErrByte.unionValue.nErrSection = 0;      //无
            break;
        case 0x02:
            rZCANFrame.error_code = ZCAN_ERROR_CAN_PASSIVE;
            passiveErrByte.unionValue.nErrCodeType = 3;     //其他错误
            passiveErrByte.unionValue.nErrSection = canHead.nChnl;  //通道
            break;
        case 0x04:
            rZCANFrame.error_code = ZCAN_ERROR_CAN_PASSIVE;
            passiveErrByte.unionValue.nErrCodeType = 3;     //其他错误
            passiveErrByte.unionValue.nErrSection = 0x08;   //crc序列错误
            break;
        case 0x08:
            rZCANFrame.error_code = ZCAN_ERROR_CAN_PASSIVE;
            passiveErrByte.unionValue.nErrCodeType = 1;     //格式错误
            passiveErrByte.unionValue.nErrSection = 0;      //无
            break;
        case 0x10:
            rZCANFrame.error_code = ZCAN_ERROR_CAN_PASSIVE;
            passiveErrByte.unionValue.nErrCodeType = 2;     //填充错误
            passiveErrByte.unionValue.nErrSection = 0;      //无
            break;
        case 0x20:
            rZCANFrame.error_code = ZCAN_ERROR_CAN_PASSIVE;
            passiveErrByte.unionValue.nErrCodeType = 3;     //其他错误
            passiveErrByte.unionValue.nErrSection = 0x1C;   //溢出标志
            break;
        case 0x40:
            rZCANFrame.error_code = ZCAN_ERROR_CAN_OVERFLOW;
            break;
        case 0x80:
            rZCANFrame.error_code = ZCAN_ERROR_CAN_OVERFLOW;
            break;
        default:
            break;
        }
    }
    break;
    case 0xE2:
    {
        //总线报警
        rZCANFrame.error_code = ZCAN_ERROR_CAN_ERRALARM;
    }
    break;
    case 0xE3:
    {
        //总线消极
        rZCANFrame.error_code = ZCAN_ERROR_CAN_PASSIVE;
        passiveErrByte.unionValue.nErrCodeType = 3;         //其他错误
        passiveErrByte.unionValue.nErrSection = 0x16;       //消极错误标志
    }
    break;
    case 0xE4:
    {
        //总线关闭
        rZCANFrame.error_code = ZCAN_ERROR_CAN_BUSOFF;
    }
    break;
    case 0xE5:
    {
        //总线超载
        rZCANFrame.error_code = ZCAN_ERROR_CAN_PASSIVE;
        passiveErrByte.unionValue.nErrCodeType = 3;         //其他错误
        passiveErrByte.unionValue.nErrSection = 0x1C;       //溢出标志
    }
    break;
    case 0xE6:
    {
        //总线唤醒
        rZCANFrame.error_code = 0;
    }
    break;
    case 0xFF:
    {
        //定时发送错误
        rZCANFrame.error_code = ZCAN_ERROR_CAN_PASSIVE;
        passiveErrByte.unionValue.nErrCodeType = 3;         //其他错误
        passiveErrByte.unionValue.nErrSection = 0;          //无
    }
    break;
    default:
        break;
    }

    rZCANFrame.passive_ErrData[0] = passiveErrByte.nRawValue;
    rZCANFrame.passive_ErrData[1] = nRxErr;
    rZCANFrame.passive_ErrData[2] = nTxErr;

    if (0 == rZCANFrame.error_code)
    {
        LOG_ERR("Error Flag with err code = 0, data[0]=0x%02X\n", nErrCode);
    }
    return true;
}

bool CPacketUtil::CANErrorData2ZCANDataObj(const PacketDataCANHead& canHead, const uint8_t* data, ZCANDataObj& zcanDataObj)
{
    if (!canHead.frameInfo.unionVal.bERR || nullptr == data)
    {
        return false;
    }

    memset(&zcanDataObj, 0, sizeof(zcanDataObj));
    zcanDataObj.dataType = ZCAN_DT_ZCAN_ERROR_DATA;
    zcanDataObj.chnl = canHead.nChnl;
    ZCANErrorData& errData = zcanDataObj.data.zcanErrData;
    int nLen = canHead.nDataLen;
    uint8_t nErrCode = (nLen > 0 ? data[0] : 0);
    uint8_t nBusErrSubcode = (nLen > 1 ? data[1] : 0); //当nErrorCode = 0xE1有效
    uint8_t nErrData = (nLen > 2 ? data[2] : 0);       //当nErrorCode != 0xE1有效
    uint8_t nRxErr = (nLen > 3 ? data[3] : 0);
    uint8_t nTxErr = (nLen > 4 ? data[4] : 0);

	LOG_INFO("CPacketUtil::CANErrorData2ZCANDataObj-nErrCode:%02x\n", nErrCode);
	LOG_INFO("CPacketUtil::CANErrorData2ZCANDataObj-nBusErrSubcode:%02x\n", nBusErrSubcode);
	LOG_INFO("CPacketUtil::CANErrorData2ZCANDataObj-nErrData:%02x\n", nErrData);
	LOG_INFO("CPacketUtil::CANErrorData2ZCANDataObj-nRxErr:%02x\n", nRxErr);
	LOG_INFO("CPacketUtil::CANErrorData2ZCANDataObj-nTxErr:%02x\n", nTxErr);

    errData.rxErrCount = nRxErr;
    errData.txErrCount = nTxErr;

    switch (nErrCode)
    {
    case 0x00:
        //总线积极,总线积极不是错误，是总线的正常状态，不进行错误转换
        errData.errType = 0;
        break;
    case 0xE1:
    {
        //总线错误, nBusErrSubcode表示具体错误类型，节点状态有效，根据Rx，tx错误计数推断
        errData.errType = ZCAN_ERR_TYPE_BUS_ERR;
        //errData.errSubType = ZCAN_BUS_ERR_NO_ERR;
		errData.errSubType = ZCAN_BUS_ERR_NODE_STATE_CHAGE;
        errData.nodeState = CPacketUtil::GetNodeStateByTxRxErrCounter(nTxErr, nRxErr);
		if (errData.nodeState == ZCAN_NODE_STATE_BUSOFF)
		{
			errData.rxErrCount = 255;
			errData.txErrCount = 255;
		}
        switch (nBusErrSubcode)
        {
            /*
            总线错误类型错误值
            位错误0x01
            应答错误0x02
            CRC 错误0x04
            格式错误0x08
            填充错误0x10
            超载错误0x20
            接收缓冲区满0x40
            发送缓冲区满0x80
            */
        case 0x01:
            errData.errSubType = ZCAN_BUS_ERR_BIT_ERR;
            break;
        case 0x02:
            errData.errSubType = ZCAN_BUS_ERR_ACK_ERR;
            break;
        case 0x04:
            errData.errSubType = ZCAN_BUS_ERR_CRC_ERR;
            break;
        case 0x08:
            errData.errSubType = ZCAN_BUS_ERR_FORM_ERR;
            break;
        case 0x10:
            errData.errSubType = ZCAN_BUS_ERR_STUFF_ERR;
            break;
        case 0x20:
            //超载错误不会出现，这里进行屏蔽
            /*errData.errType = 0;
            LOG_ERR("ErrCode:0xE1,sub:0x20, bus overload, will be ignored\n");*/
			errData.errType = ZCAN_ERR_TYPE_BUS_ERR;
			errData.errSubType = ZCAN_BUS_ERR_OVERLOAD_ERR;
            break;
        case 0x40:
            //接收缓冲区满转换为控制器错误,保留，新设备不再使用
            errData.errType = ZCAN_ERR_TYPE_CONTROLLER_ERR;
            errData.errSubType = ZCAN_CONTROLLER_DRIVER_RX_BUFFER_OVERFLOW;
            break;
        case 0x80:
            //发送缓冲区满转换为控制器错误，保留，新设备不再使用
            errData.errType = ZCAN_ERR_TYPE_CONTROLLER_ERR;
            errData.errSubType = ZCAN_CONTROLLER_DRIVER_RX_BUFFER_OVERFLOW;
            break;
        default:
            LOG_ERR("ErrCode:0xE1,sub:0x%02X, unkown bus errcode, will be ignored\n", nBusErrSubcode);
            errData.errType = 0;
            break;
        }
    }
    break;
    case 0xE2:
    {
        //总线报警,总线报警不是错误，是总线的正常状态，不进行错误转换
        /*LOG_ERR("ErrCode:0xE2, NodeState:Warnning, will be ignored\n");
        errData.errType = 0;*/
		errData.errType = ZCAN_ERR_TYPE_BUS_ERR;
		errData.errSubType = ZCAN_BUS_ERR_NODE_STATE_CHAGE;
    }
    break;
    case 0xE3:
    {
        //总线消极,总线消极不是错误，是总线的正常状态，不进行错误转换
        /*LOG_ERR("ErrCode:0xE3, NodeState:Passive, will be ignored\n");
        errData.errType = 0;*/
		errData.errType = ZCAN_ERR_TYPE_BUS_ERR;
		errData.errSubType = ZCAN_BUS_ERR_NODE_STATE_CHAGE;
    }
    break;
    case 0xE4:
    {
        //总线关闭,转换为总线错误，但是没有错误子代码，提供错误状态为总线关闭
        LOG_ERR("ErrCode:0xE4, NodeState:BusOff\n");
        errData.errType = ZCAN_ERR_TYPE_BUS_ERR;
		errData.errSubType = ZCAN_BUS_ERR_NODE_STATE_CHAGE;
        /*errData.errSubType = ZCAN_BUS_ERR_NO_ERR;
        errData.nodeState = ZCAN_NODE_STATE_BUSOFF;*/
    }
    break;
    case 0xE5:
    {
        //总线超载，保留，不再使用，不进行转换
        /*LOG_ERR("ErrCode:0xE5, Node Bus overload, will be ignored\n");
        errData.errType = 0;*/
		errData.errType = ZCAN_ERR_TYPE_BUS_ERR;
		errData.errSubType = ZCAN_BUS_ERR_OVERLOAD_ERR;
    }
    break;
    case 0xE6:
    {
        //控制器错误
        errData.errType = ZCAN_ERR_TYPE_CONTROLLER_ERR;
        switch (nErrData)
        {
        case 0x01:
            errData.errSubType = ZCAN_CONTROLLER_RX_FIFO_OVERFLOW;
            break;
        case 0x02:
            errData.errSubType = ZCAN_CONTROLLER_DRIVER_RX_BUFFER_OVERFLOW;
            break;
        case 0x03:
            errData.errSubType = ZCAN_CONTROLLER_DRIVER_TX_BUFFER_OVERFLOW;
            break;
        case 0x04:
            errData.errSubType = ZCAN_CONTROLLER_TX_FRAME_INVALID;
            break;
        case 0x05:
            errData.errSubType = ZCAN_CONTROLLER_INTERNAL_ERROR;
            break;
        default:
            LOG_ERR("ErrCode:0xE6, Controler Error, unkown sub code:0x%02X , will be ignored\n", nErrData);
            break;
        }
    }
    break;
    case 0xE7:
        //其他错误状态，用于定义终端设备错误
        errData.errType = ZCAN_ERR_TYPE_DEVICE_ERR;
        switch (nErrData)
        {
        case 0x01:
            errData.errSubType = ZCAN_DEVICE_APP_RX_BUFFER_OVERFLOW;
            break;
        case 0x02:
            errData.errSubType = ZCAN_DEVICE_APP_TX_BUFFER_OVERFLOW;
            break;
        default:
            LOG_ERR("ErrCode:0xE7, Device Error, unkown sub code:0x%02X , will be ignored\n", nErrData);
            break;
        }
        break;
    default:
        LOG_ERR("Unkown ErrCode:0x%02X, will be ignored\n", nErrCode);
        break;
    }

    if (0 == errData.errType)
    {
        LOG_ERR("Error Frame Generate invalid ErrData.errType = 0, data[0...2]=0x%02X 0x%02X 0x%02X RxErr:%d TxErr:%d\n", nErrCode, nBusErrSubcode, nErrData, nRxErr, nTxErr);
    }
    return true;
}

void CPacketUtil::DumpData(uint8_t* pData, int nLen)
{
    const int COLUMUS_ONE_LINE = 16;
    LOG_ALWAYS("Dump data:%d Bytes\n", nLen);
    std::string strLine;
    char tmpByte[4] = { 0 };
    int nLine = 0;
    for (int i = 0; i < nLen; i++)
    {
        snprintf(tmpByte, sizeof(tmpByte), " %02x", pData[i]);
        strLine.append(tmpByte);
        if ((i + 1) % COLUMUS_ONE_LINE == 0)
        {
            LOG_ALWAYS("%s\n", strLine.c_str());
            strLine.clear();
            nLine++;
        }
    }
    //最后一行
    if (strLine.length() > 0)
    {
        LOG_ALWAYS("%s\n", strLine);
        strLine.clear();
        nLine++;
    }
    LOG_ALWAYS("Dump data done, line count:%d\n", nLine);
}

int CPacketUtil::GetNodeStateByTxRxErrCounter(int nTxErr, int nRxErr)
{
    if (nTxErr <= 127 && nRxErr <= 127)
    {
        if (nTxErr > 96 || nRxErr > 96)
        {
            return ZCAN_NODE_STATE_WARNNING;
        }
        else
        {
            return ZCAN_NODE_STATE_ACTIVE;
        }
    }
    else
    {
        if (nTxErr >= 255 || nRxErr >= 255)
        {
            return ZCAN_NODE_STATE_BUSOFF;
        }
        else
        {
            return ZCAN_NODE_STATE_PASSIVE;
        }
    }
}

void CPacketUtil::PacketDataBusUsage2ZCANDataObj(const PacketDataBusUsage& pktBusUsage, ZCANDataObj& zRx)
{
	memset(&zRx, 0, sizeof(zRx));
	zRx.dataType = ZCAN_DT_ZCAN_BUSUSAGE_DATA;
	zRx.chnl = pktBusUsage.nChnl;
	BusUsage &busUsage = zRx.data.busUsage;
	memcpy(&busUsage, &pktBusUsage, sizeof(BusUsage));
	return;
}

VecVecZCANDataObjPtr CPacketUtil::SplitZCANData(const ZCANDataObj* pSend, ULONG len)
{
    VecVecZCANDataObjPtr vecRet;
    if (0 == len)
    {
        return vecRet;
    }

    uint8_t nLastType = pSend[0].dataType;
    VecZCANDataObjPtr vecDataPtr;
    for (ULONG i = 0; i < len; ++i)
    {
        if (pSend[i].dataType != nLastType)
        {
            if (vecDataPtr.size() > 0)
            {
                vecRet.push_back(vecDataPtr);
                vecDataPtr.clear();
            }
        }
        vecDataPtr.push_back(&pSend[i]);
        nLastType = pSend[i].dataType;
    }
    if (vecDataPtr.size() > 0)
    {
        vecRet.push_back(vecDataPtr);
        vecDataPtr.clear();
    }
    return vecRet;
}

bool CPacketUtil::PacketData2ZCANDataObj(const PacketDataCANFD& pktFrame, ZCANDataObj& zCANObj)
{
    if (pktFrame.canHead.frameInfo.unionVal.bERR)
    {
        return CANErrorData2ZCANDataObj(pktFrame.canHead, pktFrame.canData, zCANObj);
    }
    else
    {
        CANFrame2ZCANRx(pktFrame, zCANObj);
        return true;
    }
}

bool CPacketUtil::PacketData2ZCANDataObj(const PacketDataCAN& pktFrame, ZCANDataObj& zCANObj)
{
    if (pktFrame.canHead.frameInfo.unionVal.bERR)
    {
        return CANErrorData2ZCANDataObj(pktFrame.canHead, pktFrame.canData, zCANObj);
    }
    else
    {
        CANFrame2ZCANRx(pktFrame, zCANObj);
        return true;
    }
}

bool CPacketUtil::PacketData2ZCANDataObj(const PacketDataGPS& pktFrame, ZCANDataObj& zCANObj)
{
    PacketGPS2ZCANDataObj(pktFrame, zCANObj);
    return true;
}

bool CPacketUtil::PacketData2ZCANDataObj(const PacketDataLIN& pktFrame, ZCANDataObj& zCANObj)
{
    PacketLIN2ZCANDataObj(pktFrame, zCANObj);
    return true;
}

bool CPacketUtil::PacketData2ZCANDataObj(const PacketDataBusUsage &pktBusUsage, ZCANDataObj& zCANObj)
{
	PacketDataBusUsage2ZCANDataObj(pktBusUsage, zCANObj);
	return true;
}

bool CPacketUtil::CANFrame2ErrFrame(const PacketDataCAN& pktFrame, ZCAN_CHANNEL_ERR_INFO& errInfo)
{
    return CANErrorData2ZcanErrInfo(pktFrame.canHead, pktFrame.canData, errInfo);
}

bool CPacketUtil::CANFrame2ErrFrame(const PacketDataCANFD& pktFrame, ZCAN_CHANNEL_ERR_INFO& errInfo)
{
    return CANErrorData2ZcanErrInfo(pktFrame.canHead, pktFrame.canData, errInfo);
}

void CPacketUtil::ZCANAutoSend2AutoSendFrame(const ZCAN_AUTO_TRANSMIT_OBJ& zCAN, int nChnl, bool bTxEchoEnable, PacketDataAutoSend& autoSend)
{
    memset(&autoSend, 0, sizeof(autoSend));
    autoSend.nIndex = (uint8_t)zCAN.index;
    autoSend.bEnable = (uint8_t)zCAN.enable;
    autoSend.nPeriodMs = (USHORT)zCAN.interval;
    autoSend.nRepeatCount = 0;
    PacketDataCAN pktCAN;
    ZCANTx2CANFrame(zCAN.obj, nChnl, bTxEchoEnable, pktCAN);
    CANFrame2CANFDFrame(pktCAN, autoSend.pktData);

    autoSend.stFlag.unionValue.nUseDataTimestamp = 1;
    //nTimestamp低4位表示定时时间间隔，单位100us，zCAN.interval单位ms
    autoSend.pktData.canHead.nTimestamp = zCAN.interval * 10;
}

void CPacketUtil::ZCANAutoSend2AutoSendFrame(const ZCANFD_AUTO_TRANSMIT_OBJ& zCAN, int nChnl, bool bTxEchoEnable, PacketDataAutoSend& autoSend)
{
    memset(&autoSend, 0, sizeof(autoSend));
    autoSend.nIndex = (uint8_t)zCAN.index;
    autoSend.bEnable = (uint8_t)zCAN.enable;
    autoSend.nPeriodMs = (USHORT)zCAN.interval;
    autoSend.nRepeatCount = 0;
    ZCANTx2CANFrame(zCAN.obj, nChnl, bTxEchoEnable, autoSend.pktData);

    autoSend.stFlag.unionValue.nUseDataTimestamp = 1;
    //nTimestamp低4位表示定时时间间隔，单位100us，zCAN.interval单位ms
    autoSend.pktData.canHead.nTimestamp = zCAN.interval * 10;
}

void CPacketUtil::AutoSendData2AutoSendFrame(const AutoSendData& autoSendData, int nChnl, bool bTxEchoEnable, PacketDataAutoSend& autoSendFrame)
{
    memset(&autoSendFrame, 0, sizeof(autoSendFrame));
    autoSendFrame.nIndex = (uint8_t)autoSendData.data.index;
    autoSendFrame.bEnable = (uint8_t)autoSendData.data.enable;
    autoSendFrame.nPeriodMs = (USHORT)autoSendData.data.interval;
    autoSendFrame.nRepeatCount = autoSendData.sendcount;
    if (autoSendData.iscanfd)
    {
        ZCANTx2CANFrame(autoSendData.data.obj, nChnl, bTxEchoEnable, autoSendFrame.pktData);
    }
    else
    {
        ZCAN_Transmit_Data* pZCANTx = (ZCAN_Transmit_Data*)&autoSendData.data.obj;
        PacketDataCAN pktCAN;
        ZCANTx2CANFrame(*pZCANTx, nChnl, bTxEchoEnable, pktCAN);
        CANFrame2CANFDFrame(pktCAN, autoSendFrame.pktData);
    }

    autoSendFrame.stFlag.unionValue.nUseDataTimestamp = 1;
    //nTimestamp低4位表示定时时间间隔，单位100us，zCAN.interval单位ms
    autoSendFrame.pktData.canHead.nTimestamp = autoSendData.data.interval * 10;
}

void CPacketUtil::AutoSendFrame2ZCANAutoSend(const PacketDataAutoSend& autoSend, ZCAN_AUTO_TRANSMIT_OBJ& zCAN, int& nChnl)
{
    assert(autoSend.pktData.canHead.frameInfo.unionVal.bFD == 0);
    memset(&zCAN, 0, sizeof(zCAN));
    zCAN.enable = autoSend.bEnable;
    zCAN.index = autoSend.nIndex;
    if (autoSend.stFlag.unionValue.nUseDataTimestamp)
    {
        //zCAN.interval 单位ms，autoSend.pktData.canHead.nTimestamp单位100us
        zCAN.interval = (UINT)(autoSend.pktData.canHead.nTimestamp/10);
    }
    else
    {
        zCAN.interval = (UINT)autoSend.pktData.canHead.nTimestamp;
    }

    zCAN.obj.frame.can_dlc = autoSend.pktData.canHead.nDataLen;
    zCAN.obj.frame.can_id = MAKE_CAN_ID(autoSend.pktData.canHead.nID, autoSend.pktData.canHead.frameInfo.unionVal.bEXT,
        autoSend.pktData.canHead.frameInfo.unionVal.bRTR, autoSend.pktData.canHead.frameInfo.unionVal.bERR);
    memcpy(zCAN.obj.frame.data, autoSend.pktData.canData, CAN_MAX_DLEN);
    nChnl = autoSend.pktData.canHead.nChnl;
}

void CPacketUtil::AutoSendFrame2ZCANAutoSend(const PacketDataAutoSend& autoSend, ZCANFD_AUTO_TRANSMIT_OBJ& zCAN, int& nChnl)
{
    assert(autoSend.pktData.canHead.frameInfo.unionVal.bFD == 1);
    memset(&zCAN, 0, sizeof(zCAN));
    zCAN.enable = autoSend.bEnable;
    zCAN.index = autoSend.nIndex;
    if (autoSend.stFlag.unionValue.nUseDataTimestamp)
    {
        //zCAN.interval 单位ms，autoSend.pktData.canHead.nTimestamp单位100us
        zCAN.interval = (UINT)(autoSend.pktData.canHead.nTimestamp/10);
    }
    else
    {
        zCAN.interval = (UINT)autoSend.pktData.canHead.nTimestamp;
    }

    zCAN.obj.frame.len = autoSend.pktData.canHead.nDataLen;
    zCAN.obj.frame.flags |= (autoSend.pktData.canHead.frameInfo.unionVal.bBRS ? CANFD_BRS : 0);
    zCAN.obj.frame.flags |= (autoSend.pktData.canHead.frameInfo.unionVal.bESI ? CANFD_ESI : 0);
    zCAN.obj.frame.can_id = MAKE_CAN_ID(autoSend.pktData.canHead.nID, autoSend.pktData.canHead.frameInfo.unionVal.bEXT,
        autoSend.pktData.canHead.frameInfo.unionVal.bRTR, autoSend.pktData.canHead.frameInfo.unionVal.bERR);
    memcpy(zCAN.obj.frame.data, autoSend.pktData.canData, CANFD_MAX_DLEN);
    nChnl = autoSend.pktData.canHead.nChnl;
}

void CPacketUtil::PacketGPS2ZGPS(const PacketDataGPS& pktGPS, ZCAN_GPS_FRAME& zGPS)
{
    memset(&zGPS, 0, sizeof(zGPS));
    //time
    GPSTimeVal stTime;
    stTime.rawValue = pktGPS.nTimestamp;
    zGPS.validflag.unionValue.btm = stTime.unionValue.bValid;
    if (stTime.unionValue.bValid)
    {
        time_t utc = stTime.unionValue.nUTC/1000000;
        struct tm* time = gmtime(&utc);
        if (time)
        {
            zGPS.tm.year = time->tm_year + 1900;
            zGPS.tm.mon = time->tm_mon + 1;
            zGPS.tm.day = time->tm_mday;
            zGPS.tm.hour = time->tm_hour;
            zGPS.tm.min = time->tm_min;
            zGPS.tm.sec = time->tm_sec;
        }
    }

    //Latitude longtitude
    GPSLatLongVal stLatLong;
    stLatLong.rawValue = pktGPS.nLatitude;
    zGPS.validflag.unionValue.blatlong = stLatLong.unionValue.bValid;
    zGPS.latitude = (float)((double)stLatLong.unionValue.nData / 1E6);
    zGPS.latitude *= stLatLong.unionValue.bWestSourth ? -1 : 1;
    //longtitude
    stLatLong.rawValue = pktGPS.nLongitude;
    zGPS.longitude = (float)((double)stLatLong.unionValue.nData / 1E6);
    zGPS.longitude *= stLatLong.unionValue.bWestSourth ? -1 : 1;

    //spd
    GPSSpdAltitude stSpdAltitude;
    stSpdAltitude.rawValue = pktGPS.nSpeed;
    zGPS.validflag.unionValue.bspd = stSpdAltitude.unionValue.bValid;
    zGPS.speed = stSpdAltitude.unionValue.nData * 0.1f;

    //altitude
    stSpdAltitude.rawValue = pktGPS.nAltitude;
    zGPS.validflag.unionValue.baltitude = stSpdAltitude.unionValue.bValid;
    zGPS.altitude = stSpdAltitude.unionValue.nData - 1000.0f;

    //couse angle
    GPSCourseAngle stCourseAngle;
    stCourseAngle.rawValue = pktGPS.nCourseAngle;
    zGPS.validflag.unionValue.bcouseangle = stCourseAngle.unionValue.bValid;
    zGPS.courseangle = (float)((double)stCourseAngle.unionValue.nData / 1E6);
}

void CPacketUtil::PacketGPS2ZCANDataObj(const PacketDataGPS& pktGPS, ZCANDataObj& zRx)
{
    memset(&zRx, 0, sizeof(zRx));
    zRx.dataType = ZCAN_DT_ZCAN_GPS_DATA;
    ZCANGPSData& zGPS = zRx.data.zcanGPSData;
    //time
    GPSTimeVal stTime;
    stTime.rawValue = pktGPS.nTimestamp;
    zGPS.flag.unionVal.timeValid = stTime.unionValue.bValid;
    if (stTime.unionValue.bValid)
    {
        time_t utc = stTime.unionValue.nUTC/1000000;
        time_t milsec = (stTime.unionValue.nUTC % 1000000) / 1000;
        struct tm* time = gmtime(&utc);
        if (time)
        {
            zGPS.time.year = time->tm_year + 1900;
            zGPS.time.mon = time->tm_mon + 1;
            zGPS.time.day = time->tm_mday;
            zGPS.time.hour = time->tm_hour;
            zGPS.time.min = time->tm_min;
            zGPS.time.sec = time->tm_sec;
            zGPS.time.milsec = (USHORT)milsec;
        }
    }

    //Latitude longtitude
    GPSLatLongVal stLatLong;
    stLatLong.rawValue = pktGPS.nLatitude;
    zGPS.flag.unionVal.latlongValid = stLatLong.unionValue.bValid;
    zGPS.latitude = (double)stLatLong.unionValue.nData / 1E6;
    zGPS.latitude *= stLatLong.unionValue.bWestSourth ? -1 : 1;
    //longtitude
    stLatLong.rawValue = pktGPS.nLongitude;
    zGPS.flag.unionVal.latlongValid &= stLatLong.unionValue.bValid;
    zGPS.longitude = (double)stLatLong.unionValue.nData / 1E6;
    zGPS.longitude *= stLatLong.unionValue.bWestSourth ? -1 : 1;

    //spd
    GPSSpdAltitude stSpdAltitude;
    stSpdAltitude.rawValue = pktGPS.nSpeed;
    zGPS.flag.unionVal.speedValid = stSpdAltitude.unionValue.bValid;
    zGPS.speed = (double)stSpdAltitude.unionValue.nData * 0.1f;

    //altitude
    stSpdAltitude.rawValue = pktGPS.nAltitude;
    zGPS.flag.unionVal.altitudeValid = stSpdAltitude.unionValue.bValid;
    zGPS.altitude = (double)stSpdAltitude.unionValue.nData - 1000.0f;

    //course angle
    GPSCourseAngle stCourseAngle;
    stCourseAngle.rawValue = pktGPS.nCourseAngle;
    zGPS.flag.unionVal.courseAngleValid = stCourseAngle.unionValue.bValid;
    zGPS.courseAngle = (double)stCourseAngle.unionValue.nData / 1E6;
}

void CPacketUtil::PacketLIN2ZLIN(const PacketDataLIN& pktLIN, ZCAN_LIN_MSG& zLIN, int* pChnl)
{
    memset(&zLIN, 0, sizeof(zLIN));
    zLIN.ID = pktLIN.stMsgID.nRawID;
    zLIN.DataLen = pktLIN.nDataLen;
    zLIN.Flag = pktLIN.frameInfo.rawVal;
    zLIN.TimeStamp = (UINT)(pktLIN.nTimestamp/100);
    memcpy(zLIN.Data, pktLIN.nData, 8);
    if (pChnl != NULL)
    {
        *pChnl = pktLIN.nChnl;
    }
}

void CPacketUtil::PacketLIN2ZCANDataObj(const PacketDataLIN& pktLIN, ZCANDataObj& zRx)
{
    memset(&zRx, 0, sizeof(zRx));
    zRx.dataType = ZCAN_DT_ZCAN_LIN_DATA;
    zRx.chnl = pktLIN.nChnl;
    ZCANLINData& zLIN = zRx.data.zcanLINData;
    zLIN.timeStamp = pktLIN.nTimestamp;
    zLIN.PID.rawVal = pktLIN.stMsgID.nRawID;
    zLIN.dataLen = pktLIN.nDataLen;
    zLIN.flag.rawVal = pktLIN.frameInfo.rawVal;
    zLIN.chkSum = pktLIN.nChkSum;
    memcpy(zLIN.data, pktLIN.nData, 8);
}

void CPacketUtil::ZLIN2PacketLIN(int nChnl, const ZCAN_LIN_MSG& zLIN, PacketDataLIN& pktLIN)
{
    memset(&pktLIN, 0, sizeof(pktLIN));
    pktLIN.nChnl = nChnl;
    pktLIN.stMsgID.nRawID = zLIN.ID;
    pktLIN.frameInfo.unionVal.bTx = 1;
    pktLIN.nDataLen = zLIN.DataLen;
    memcpy(pktLIN.nData, zLIN.Data, 8);
}

bool CPacketUtil::ZCANDataObj2PacketLIN(const ZCANDataObj& zTx, PacketDataLIN& pktLIN)
{
    assert(zTx.dataType == ZCAN_DT_ZCAN_LIN_DATA);
    if (zTx.dataType != ZCAN_DT_ZCAN_LIN_DATA)
    {
        return false;
    }
    memset(&pktLIN, 0, sizeof(pktLIN));
    pktLIN.nChnl = zTx.chnl;
    pktLIN.stMsgID.nRawID = zTx.data.zcanLINData.PID.rawVal;
    pktLIN.frameInfo.unionVal.bTx = 1;
    pktLIN.nDataLen = zTx.data.zcanLINData.dataLen;
    memcpy(pktLIN.nData, zTx.data.zcanLINData.data, 8);
    return true;
}

CPacketDecoder::CPacketDecoder(const char* rmt) : m_strName(rmt)
{

}

CPacketDecoder::~CPacketDecoder()
{
}

void CPacketDecoder::AppendData(const char* data, unsigned int len)
{
    if (nullptr == data || 0 == len) return;
    std::unique_lock<std::mutex> lk_(m_dequeMutex);
    const uint8_t* bdata = reinterpret_cast<const uint8_t*>(data);
    size_t ret = m_dequeData.write(bdata, len);
    if (ret != len)
    {
        LOG_ERR("Deocoder Append data len:%d, written:%d buffer data size:%d\n", len, ret, m_dequeData.size());
    }
}

size_t CPacketDecoder::GetPackets(VecPackets& vecPackets)
{
    const int nMaxPktOnce = 50;
    size_t nRet = 0;
    vecPackets.clear();
    Packet packet;
    std::unique_lock<std::mutex> lk_(m_dequeMutex);
    while (CPacketUtil::GetPacket(m_dequeData, packet))
    {
        vecPackets.push_back(packet);
        if (vecPackets.size() >= nMaxPktOnce)
        {
            break;
        }
    }
    nRet = vecPackets.size();
    return nRet;
}

CPacketEncoder::CPacketEncoder()
{

}

CPacketEncoder::~CPacketEncoder()
{

}

VecPackets CPacketEncoder::BuildPacketCAN(int nChnl, const ZCAN_Transmit_Data* pSend, ULONG len, bool bTxEchoEnable)
{
    VecPackets vecPackets;
    Packet packet;
    PacketDataCAN pktFrame;
    const ULONG max_frame_cnt = 60;     //max 1460, 1460 - 1 -6 / FRAME_LEN_CAN = 60
    ULONG doneCnt = 0;
    while (len)
    {
        ULONG cnt = len < max_frame_cnt ? len : max_frame_cnt;
        packet.clear();
        PacketAddHead(packet, BuildPacketHead(PACKET_TYPE_CAN, (int)cnt));
        for (ULONG i = 0; i<cnt; i++) 
        {
            CPacketUtil::ZCANTx2CANFrame(pSend[doneCnt + i], nChnl, bTxEchoEnable, pktFrame);
            PacketAddData(packet, pktFrame);
        }
        PacketAddCRC(packet);
        vecPackets.push_back(packet);

        doneCnt += cnt;
        len -= cnt;
    }
    return vecPackets;
}

VecPackets CPacketEncoder::BuildPacketCANFD(int nChnl, const ZCAN_TransmitFD_Data* pSend, ULONG len, bool bTxEchoEnable)
{
    VecPackets vecPackets;
    Packet packet;
    PacketDataCANFD pktFrame;
    const ULONG max_frame_cnt = 18;  //max 1460,  1460 - 6 - 1 / FRAME_LEN_CANFD = 18
    ULONG doneCnt = 0;
    while (len)
    {
        ULONG cnt = len < max_frame_cnt ? len : max_frame_cnt;
        packet.clear();
        PacketAddHead(packet, BuildPacketHead(PACKET_TYPE_CANFD, (int)cnt));
        for (ULONG i = 0; i<cnt; i++)
        {
            CPacketUtil::ZCANTx2CANFrame(pSend[doneCnt + i], nChnl, bTxEchoEnable, pktFrame);
            PacketAddData(packet, pktFrame);
        }
        PacketAddCRC(packet);
        vecPackets.push_back(packet);

        doneCnt += cnt;
        len -= cnt;
    }
    return vecPackets;
}

VecPackets CPacketEncoder::BuildPacketCANCANFD(const VecZCANDataObjPtr& vecData)
{
    VecPackets vecPackets;
    Packet packet;
    PacketDataCANFD pktFrame;
    const ULONG max_frame_cnt = 18;  //max 1460,  1460 - 6 - 1 / FRAME_LEN_CANFD = 18
    ULONG len = vecData.size();
    ULONG doneCnt = 0;
    while (len)
    {
        ULONG cnt = len < max_frame_cnt ? len : max_frame_cnt;
        packet.clear();
        PacketAddHead(packet, BuildPacketHead(PACKET_TYPE_CANFD, (int)cnt));
        for (ULONG i = 0; i < cnt; i++)
        {
            if (CPacketUtil::ZCANDataObj2CANFrame(*vecData[doneCnt + i], pktFrame))
            {
                PacketAddData(packet, pktFrame);
            }
            else
            {
                LOG_ERR("Invalid ZCANDataObj.datatype:%d\n", vecData[doneCnt + i]->dataType);
            }
        }
        PacketAddCRC(packet);
        vecPackets.push_back(packet);

        doneCnt += cnt;
        len -= cnt;
    }
    return vecPackets;
}

VecPackets CPacketEncoder::BuildPacketMerge(const ZCANDataObj* pSend, ULONG len)
{
    //用户数据可能混杂CAN/CANFD/LIN等数据，把数据按照数据类型进行分类，保证发送顺序和用户设置的顺序一致
    VecVecZCANDataObjPtr dataVec = CPacketUtil::SplitZCANData(pSend, len);
    VecPackets vecPackets;
    for (size_t i = 0; i < dataVec.size(); ++i)
    {
        VecZCANDataObjPtr& vecData = dataVec[i];
        if (ZCAN_DT_ZCAN_CAN_CANFD_DATA == vecData[0]->dataType)
        {
            VecPackets vecPkt = BuildPacketCANCANFD(vecData);
            for (auto& pkt : vecPkt)
            {
                vecPackets.push_back(pkt);
            }
        }
        else if (ZCAN_DT_ZCAN_LIN_DATA == vecData[0]->dataType)
        {
            VecPackets vecPkt = BuildPacketLIN(vecData);
            for (auto& pkt : vecPkt)
            {
                vecPackets.push_back(pkt);
            }
        }
        else
        {
            LOG_ERR("Splited datatype:%d invalid\n", vecData[0]->dataType);
        }
    }
    return vecPackets;
}

Packet CPacketEncoder::BuildPacketAUTH(bool bRequest, const uint8_t* data, unsigned int len, uint8_t autoIncIndex)
{
    Packet packet;
    if (nullptr != data && len == FRAME_LEN_AUTH)
    {
        PacketAddHead(packet, BuildPacketHead((bRequest ? PACKET_TYPE_AUTH_REQ : PACKET_TYPE_AUTH_RES), 1, autoIncIndex));
        PacketAddData(packet, data, len);
        PacketAddCRC(packet);
    }
    return packet;
}

Packet CPacketEncoder::BuildPacketAutosend(int nChnl, const ZCAN_AUTO_TRANSMIT_OBJ* pObj, bool bTxEchoEnable)
{
    Packet packet;
    PacketAddHead(packet, BuildPacketHead(PACKET_TYPE_CAN_AUTO_SEND, 1, 0));
    PacketDataAutoSend autoSend;
    CPacketUtil::ZCANAutoSend2AutoSendFrame(*pObj, nChnl, bTxEchoEnable, autoSend);
    PacketAddData(packet, autoSend);
    PacketAddCRC(packet);
    return packet;
}

Packet CPacketEncoder::BuildPacketAutosend(int nChnl, const ZCANFD_AUTO_TRANSMIT_OBJ* pObj, bool bTxEchoEnable)
{
    Packet packet;
    PacketAddHead(packet, BuildPacketHead(PACKET_TYPE_CAN_AUTO_SEND, 1, 0));
    PacketDataAutoSend autoSend;
    CPacketUtil::ZCANAutoSend2AutoSendFrame(*pObj, nChnl, bTxEchoEnable, autoSend);
    PacketAddData(packet, autoSend);
    PacketAddCRC(packet);
    return packet;
}

VecPackets CPacketEncoder::BuildPacketAutosend(int nChnl, const VecAutoSendData& vecAutoSend, bool bTxEchoEnable)
{
    VecPackets vecPackets;
    Packet packet;
    PacketDataAutoSend pktFrame;
    const ULONG max_frame_cnt = 10;
    ULONG doneCnt = 0;
    ULONG len = vecAutoSend.size();
    while (len)
    {
        ULONG cnt = len < max_frame_cnt ? len : max_frame_cnt;
        packet.clear();
        PacketAddHead(packet, BuildPacketHead(PACKET_TYPE_CAN_AUTO_SEND, (int)cnt, 0));
        for (ULONG i = 0; i<cnt; i++)
        {
            CPacketUtil::AutoSendData2AutoSendFrame(vecAutoSend[doneCnt + i], nChnl, bTxEchoEnable, pktFrame);
            PacketAddData(packet, pktFrame);
        }
        PacketAddCRC(packet);
        vecPackets.push_back(packet);

        doneCnt += cnt;
        len -= cnt;
    }
    return vecPackets;

}

Packet CPacketEncoder::BuildPacketDevReqRes(bool bRequest, const uint8_t* data, unsigned int len, uint8_t pktTypeParam)
{
    Packet packet;
    uint8_t pktType = bRequest ? PACKET_TYPE_DEV_REQ : PACKET_TYPE_DEV_RES;
    PacketHead head{ PACKET_HEADER, pktType, pktTypeParam, 0, len };
    PacketAddHead(packet, head);
    if (data != nullptr && len > 0)
    {
        PacketAddData(packet, data, len);
    }
    PacketAddCRC(packet);

    return packet;
}

Packet CPacketEncoder::BuildPacketRequest(uint8_t nSeq, const PacketDataRequest& request)
{
    Packet packet;
    PacketTypeRequestResponseParam pktParam;
    pktParam.rawValue = 0;
    pktParam.unionValue.bResponse = 0;  // 0: request
    pktParam.unionValue.nSeq = nSeq;
    USHORT len = request.vecData.size() + 2; // 2Byte : tid + request
    PacketHead head{ PACKET_HEADER, PACKET_TYPE_REQUEST_RESPONSE, pktParam.rawValue , 0, len };
    PacketAddHead(packet, head);
    PacketAddData(packet, (uint8_t*)&request, 2);
    PacketAddData(packet, request.vecData.data(), request.vecData.size());
    PacketAddCRC(packet);
    return packet;
}

Packet CPacketEncoder::BuildPacketEmulateResponse(uint8_t nSeq, const PacketDataRequest& requestData)
{
    Packet packet;
    PacketTypeRequestResponseParam pktParam;
    pktParam.rawValue = 0;
    pktParam.unionValue.bResponse = 1;  // 1: response
    pktParam.unionValue.nSeq = nSeq;

    PacketDataResponse responseData;
    GenRequestResponseEmulateData(requestData, responseData);

    USHORT len = responseData.vecData.size() + 2; // 2Byte : tid + result
    PacketHead head{ PACKET_HEADER, PACKET_TYPE_REQUEST_RESPONSE, pktParam.rawValue, 0, len };
    PacketAddHead(packet, head);
    PacketAddData(packet, (uint8_t*)&responseData, 2);
    PacketAddData(packet, responseData.vecData.data(), responseData.vecData.size());
    PacketAddCRC(packet);
    return packet;
}

void CPacketEncoder::GenRequestResponseEmulateData(const PacketDataRequest& requestData, PacketDataResponse& responseData)
{
    responseData.nTID = requestData.nTID;
    responseData.nResult = 1;   // 1: 应答正确
    if (requestData.nRequest == PACKET_REQUEST_GET_DEVICE_INFO)
    {
        char devInfo[200] = { 0 };
        int nLen = snprintf(devInfo, sizeof(devInfo), "{\r\n\"Model\":\"Emulator Device\",\r\n\"Firmware\":\"V1.00.00\",\r\n\"Hardware\":\"V1.00.00\",\r\n\"SN\":\"1234567890\"\r\n}");
        uint8_t* itr = (uint8_t*)&devInfo[0];
        responseData.vecData.assign(itr, itr + nLen + 1);
    }
    else if (requestData.nRequest == PACKET_REQUEST_GET_DEVICE_STATE)
    {
        char devInfo[200] = { 0 };
        int nLen = snprintf(devInfo, sizeof(devInfo), "{\r\n\r\n}");
        uint8_t* itr = (uint8_t*)&devInfo[0];
        responseData.vecData.assign(itr, itr + nLen + 1);
    }
    else if (requestData.nRequest == PACKET_REQUEST_GET_DEVICE_AUTOSEND_LIST)
    {
        responseData.vecData.clear();
    }
    else if (requestData.nRequest == PACKET_REQUEST_GET_DEVICE_DELAY_SEND_STATE)
    {
        char devInfo[200] = { 0 };
        int nLen = snprintf(devInfo, sizeof(devInfo), "{\r\n\"Size\":%d,\r\n\"Remain\":%d\r\n}", 0, 0);
        uint8_t* itr = (uint8_t*)&devInfo[0];
        responseData.vecData.assign(itr, itr + nLen + 1);
    }
}

VecPackets CPacketEncoder::BuildPacketLIN(int nChnl, const PZCAN_LIN_MSG pSend, ULONG len)
{
    VecPackets vecPackets;
    Packet packet;
    PacketDataLIN pktFrame;
    const ULONG max_frame_cnt = 60;     //max 1460, 1460 - 1 -6 / FRAME_LEN_LIN = 60
    ULONG doneCnt = 0;
    while (len)
    {
        ULONG cnt = len < max_frame_cnt ? len : max_frame_cnt;
        packet.clear();
        PacketAddHead(packet, BuildPacketHead(PACKET_TYPE_LIN, (int)cnt));
        for (ULONG i = 0; i<cnt; i++)
        {
            CPacketUtil::ZLIN2PacketLIN(nChnl, pSend[doneCnt + i], pktFrame);
            PacketAddData(packet, pktFrame);
        }
        PacketAddCRC(packet);
        vecPackets.push_back(packet);

        doneCnt += cnt;
        len -= cnt;
    }
    return vecPackets;
}

VecPackets CPacketEncoder::BuildPacketLIN(const VecZCANDataObjPtr& vecData)
{
    VecPackets vecPackets;
    Packet packet;
    PacketDataLIN pktFrame;
    const ULONG max_frame_cnt = 60;     //max 1460, 1460 - 1 -6 / FRAME_LEN_LIN = 60
    ULONG len = vecData.size();
    ULONG doneCnt = 0;
    while (len)
    {
        ULONG cnt = len < max_frame_cnt ? len : max_frame_cnt;
        packet.clear();
        PacketAddHead(packet, BuildPacketHead(PACKET_TYPE_LIN, (int)cnt));
        for (ULONG i = 0; i < cnt; i++)
        {
            if (CPacketUtil::ZCANDataObj2PacketLIN(*vecData[doneCnt + i], pktFrame))
            {
                PacketAddData(packet, pktFrame);
            }
            else
            {
                LOG_ERR("Invalid ZCANDataobj.dataType:%d\n", vecData[doneCnt + i]->dataType);
            }
        }
        PacketAddCRC(packet);
        vecPackets.push_back(packet);

        doneCnt += cnt;
        len -= cnt;
    }
    return vecPackets;
}

Packet CPacketEncoder::BuildPacketEchoResponse(const PacketDataCAN& canFrame)
{
    Packet packet;
    PacketDataCAN pkt = canFrame;
    //设置回显位
    pkt.canHead.frameInfo.unionVal.nTx = 1;
    //设置回显请求位为0
    pkt.canHead.frameInfo.unionVal.nEcho = 0;
    //设置发送时间戳
    pkt.canHead.frameInfo.unionVal.bSendDelay = 0;
    pkt.canHead.nTimestamp = CPacketUtil::GetNowTimeStampAsMicrosecond();
    PacketAddHead(packet, BuildPacketHead(PACKET_TYPE_CAN, 1, 0));
    PacketAddData(packet, pkt);
    PacketAddCRC(packet);
    return packet;
}

Packet CPacketEncoder::BuildPacketEchoResponse(const PacketDataCANFD& canFrame)
{
    Packet packet;
    PacketDataCANFD pkt = canFrame;
    //设置回显位
    pkt.canHead.frameInfo.unionVal.nTx = 1;
    //设置回显请求位为0
    pkt.canHead.frameInfo.unionVal.nEcho = 0;
    //设置发送时间戳
    pkt.canHead.frameInfo.unionVal.bSendDelay = 0;
    pkt.canHead.nTimestamp = CPacketUtil::GetNowTimeStampAsMicrosecond();
    PacketAddHead(packet, BuildPacketHead(PACKET_TYPE_CANFD, 1, 0));
    PacketAddData(packet, pkt);
    PacketAddCRC(packet);
    return packet;

}

PacketHead CPacketEncoder::BuildPacketHead(uint8_t pktType, int pktCnt, uint8_t pktTypeParam /*= 0*/)
{
    PacketHead head{ PACKET_HEADER, pktType, pktTypeParam, 0, 0 };
    if (pktType == PACKET_TYPE_CAN)
    {
        head.nPacketDataLen = pktCnt * FRAME_LEN_CAN;
    }
    else if (pktType == PACKET_TYPE_CANFD)
    {
        head.nPacketDataLen = pktCnt * FRAME_LEN_CANFD;
    }
    else if (pktType == PACKET_TYPE_AUTH_REQ ||
        pktType == PACKET_TYPE_AUTH_RES)
    {
        head.nPacketDataLen = FRAME_LEN_AUTH;
    }
    else if (pktType == PACKET_TYPE_CAN_AUTO_SEND)
    {
        head.nPacketDataLen = pktCnt * FRAME_LEN_AUTO_SEND;
    }
    else if (pktType == PACKET_TYPE_LIN)
    {
        head.nPacketDataLen = pktCnt * FRAME_LEN_LIN;
    }

    return head;
}

void CPacketEncoder::PacketAddHead(Packet& packet, const PacketHead& pktHead)
{
    PacketHead netHead;
    memcpy(&netHead, &pktHead, sizeof(netHead));
    netHead.nPacketDataLen = cc_htons(pktHead.nPacketDataLen);
    uint8_t* data = (uint8_t*)&netHead;
    packet.insert(packet.end(), data, data + sizeof(netHead));
}

void CPacketEncoder::PacketAddData(Packet& packet, const PacketDataCAN& canFrame)
{
    PacketDataCAN netFrame;
    netFrame.canHead.nID = cc_htonl(canFrame.canHead.nID);
    netFrame.canHead.nTimestamp = cc_htonll(canFrame.canHead.nTimestamp);
    netFrame.canHead.frameInfo.rawVal = cc_htons(canFrame.canHead.frameInfo.rawVal);
    netFrame.canHead.nChnl = canFrame.canHead.nChnl;
    netFrame.canHead.nDataLen = canFrame.canHead.nDataLen;
    memcpy(netFrame.canData, canFrame.canData, sizeof(netFrame.canData));
    uint8_t* data = (uint8_t*)&netFrame;
    packet.insert(packet.end(), data, data + sizeof(netFrame));
}

void CPacketEncoder::PacketAddData(Packet& packet, const PacketDataCANFD& canFDFrame)
{
    PacketDataCANFD netFrame;
    netFrame.canHead.nID = cc_htonl(canFDFrame.canHead.nID);
    netFrame.canHead.nTimestamp = cc_htonll(canFDFrame.canHead.nTimestamp);
    netFrame.canHead.frameInfo.rawVal = cc_htons(canFDFrame.canHead.frameInfo.rawVal);
    netFrame.canHead.nChnl = canFDFrame.canHead.nChnl;
    netFrame.canHead.nDataLen = canFDFrame.canHead.nDataLen;
    memcpy(netFrame.canData, canFDFrame.canData, sizeof(netFrame.canData));
    uint8_t* data = (uint8_t*)&netFrame;
    packet.insert(packet.end(), data, data + sizeof(netFrame));
}

void CPacketEncoder::PacketAddData(Packet& packet, const PacketDataAutoSend& autoSendFrame)
{
    PacketDataAutoSend netFrame = { 0 };
    netFrame.nIndex = autoSendFrame.nIndex;
    netFrame.bEnable = autoSendFrame.bEnable;
    netFrame.nPeriodMs = cc_htons(autoSendFrame.nPeriodMs);
    netFrame.nRepeatCount = cc_htons(autoSendFrame.nRepeatCount);
    netFrame.stFlag.rawValue = cc_htons(autoSendFrame.stFlag.rawValue);
    uint8_t *data = (uint8_t*)&netFrame;
    unsigned int len = offsetof(PacketDataAutoSend, pktData);
    assert(len > 0);
    PacketAddData(packet, data, len);
    PacketAddData(packet, autoSendFrame.pktData);
}

void CPacketEncoder::PacketAddData(Packet& packet, const PacketDataLIN& pktLIN)
{
    PacketDataLIN netFrame;
    netFrame.nTimestamp = cc_htonll(pktLIN.nTimestamp);
    netFrame.nChnl = pktLIN.nChnl;
    netFrame.stMsgID.nRawID = (pktLIN.stMsgID.nRawID);
    netFrame.frameInfo.rawVal = cc_htons(pktLIN.frameInfo.rawVal);
    netFrame.nDataLen = pktLIN.nDataLen;
    netFrame.nChkSum = pktLIN.nChkSum;
    memcpy(netFrame.nData, pktLIN.nData, 8);
    memcpy(netFrame.nReserved, pktLIN.nReserved, sizeof(netFrame.nReserved));
    uint8_t* data = (uint8_t*)&netFrame;
    packet.insert(packet.end(), data, data + sizeof(netFrame));
}

void CPacketEncoder::PacketAddData(Packet& packet, const uint8_t* data, unsigned int len)
{
    packet.insert(packet.end(), data, data + len);
}

void CPacketEncoder::PacketAddCRC(Packet& packet)
{
    auto size = packet.size();
    if (size > 0)
    {
        uint8_t crc = CPacketUtil::CalcCRC(packet.data(), size);
        packet.push_back(crc);
    }
}


