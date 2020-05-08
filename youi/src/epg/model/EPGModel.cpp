#include "EPGModel.h"

#include <logging/YiLoggerHelper.h>

#include "EPGChannelModel.h"

#define LOG_TAG "EPGModel"

EPGModel::EPGModel()
{
}

EPGModel::~EPGModel()
{
    m_channelData.clear();
}

void EPGModel::InitTime(CYIDateTime startTime, int32_t durationInMinutes)
{
    m_startTime = startTime;
    m_endTime = m_startTime;
    m_endTime.AddMinutes(durationInMinutes);
}

void EPGModel::SetChannelModels(std::vector<std::shared_ptr<EPGChannelModel>> channelData)
{
    m_channelData = channelData;
}

void EPGModel::PrependChannelModels(std::vector<std::shared_ptr<EPGChannelModel>> channelData)
{
    m_channelData.insert(m_channelData.begin(), channelData.begin(), channelData.end());

    for (size_t i = 0; i < channelData.size(); i++)
    {
        ChannelAddedAtIndex.Emit(i);
    }
}

void EPGModel::AppendChannelModels(std::vector<std::shared_ptr<EPGChannelModel>> channelData)
{
    size_t prevSize = m_channelData.size();

    m_channelData.insert(m_channelData.end(), channelData.begin(), channelData.end());

    for (size_t i = prevSize; i < m_channelData.size(); i++)
    {
        ChannelAddedAtIndex.Emit(i);
    }
}

const std::shared_ptr<EPGChannelModel> EPGModel::GetChannelModel(const CYIString &rChannelId)
{
    auto itr = std::find_if(m_channelData.begin(), m_channelData.end(), [rChannelId](std::shared_ptr<EPGChannelModel> channel)
    {
        return channel->GetChannelId() == rChannelId;
    });

    return itr != m_channelData.end() ? *itr : nullptr;
}
