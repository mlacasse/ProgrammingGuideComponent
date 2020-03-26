#include "epg/model/EPGModel.h"
#include "epg/model/EPGChannelModel.h"

#include <logging/YiLoggerHelper.h>

#define LOG_TAG "EPGModel"

EPGModel::EPGModel()
{}

EPGModel::~EPGModel()
{
    m_channelData.clear();
}

const std::shared_ptr<EPGChannelModel> EPGModel::GetChannelModel(const CYIString &rChannelId)
{
    std::shared_ptr<EPGChannelModel> pChannel = std::shared_ptr<EPGChannelModel>();
    
    for (std::vector<std::shared_ptr<EPGChannelModel> >::const_iterator itr = m_channelData.begin(); itr != m_channelData.end(); ++itr)
    {
        if ((*itr)->GetChannelId() == rChannelId)
        {
            pChannel = *itr;
            break;
        }
    }

    return pChannel;
}
