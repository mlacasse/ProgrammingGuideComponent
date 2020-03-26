#include "epg/model/EPGAssetModel.h"

#include <logging/YiLogger.h>
#include <utility/YiString.h>

#define LOG_TAG "EPGAssetModel"

EPGAssetModel::EPGAssetModel(bool isRestartable, int64_t duration, int64_t episodeNumber, int64_t seasonNumber, CYIString parentalRating, CYIString episodeTitle, CYIString description, CYIString resourceId, CYIString title, CYIString startTime, CYIString endTime)
    : m_isRestartable(isRestartable)
    , m_duration(duration)
    , m_episodeNumber(episodeNumber)
    , m_seasonNumber(seasonNumber)
    , m_parentalRating(parentalRating)
    , m_episodeTitle(episodeTitle)
    , m_description(description)
    , m_resourceId(resourceId)
    , m_title(title)
    , m_startTime(startTime)
    , m_endTime(endTime)
{
    YI_LOGD(LOG_TAG, "Asset: %s %lld %lld %lld %s %s %s %s %s %s %s", isRestartable ? "true" : "false", duration, episodeNumber, seasonNumber, parentalRating.GetData(), episodeTitle.GetData(), description.GetData(), resourceId.GetData(), title.GetData(), startTime.GetData(), endTime.GetData());
}
