// © You i Labs Inc. 2000-2017. All rights reserved.

#include "epg/model/EPGChannelModel.h"

#include <logging/YiLogger.h>
#include <utility/YiString.h>

#define LOG_TAG "EPGChannelModel"

EPGChannelModel::EPGChannelModel(bool isFavorite, uint32_t channelNumber, CYIString channelId, CYIString callSign, CYIString name, CYIUrl imageUrl)
    : m_isFavorite(isFavorite)
    , m_channelNumber(channelNumber)
    , m_channelId(channelId)
    , m_callSign(callSign)
    , m_name(name)
    , m_imageUrl(imageUrl)
{
    YI_LOGD(LOG_TAG, "Channel: %s %u %s %s %s %s", isFavorite ? "true" : "false", channelNumber, channelId.GetData(), callSign.GetData(), name.GetData(), imageUrl.ToString().GetData());
}

EPGChannelModel::~EPGChannelModel()
{
    m_assets.clear();
}
