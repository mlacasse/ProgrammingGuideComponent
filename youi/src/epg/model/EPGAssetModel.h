#ifndef EPG_ASSET_MODEL_H
#define EPG_ASSET_MODEL_H

#include <folly/dynamic.h>

#include <utility/YiDateTime.h>
#include <utility/YiString.h>
#include <network/YiUrl.h>

class EPGAssetModel
{
public:
    EPGAssetModel();
    virtual ~EPGAssetModel() = default;

    bool Init(const folly::dynamic &value);

    void SetImageUrls(const std::vector<CYIUrl> &imageUrls) { m_imageUrls = imageUrls; }

    bool IsFastForwardDisabled() const { return m_isFastForwardDisabled; }
    bool IsRestartable() const { return m_isRestartable; }
    bool IsExtendable() const { return m_isExtendable; }
    bool IsRecordable() const { return m_isRecordable; }
    bool IsLookback() const { return m_isLookback; }
    bool IsPlayable() const { return m_isPlayable; }
    bool IsDAI() const { return m_isDAI; }
    
    int64_t GetDuration() const { return m_duration; }
    int64_t GetEpisodeNumber() const { return m_episodeNumber; }
    int64_t GetSeasonNumber() const { return m_seasonNumber; }

    const std::vector<CYIUrl> &GetImageUrls() const { return m_imageUrls; }

    const CYIString &GetProgramChannelId() const { return m_programChannelId; }
    const CYIString &GetParentalRating() const { return m_parentalRating; }
    const CYIString &GetEpisodeTitle() const { return m_episodeTitle; }
    const CYIString &GetDescription() const { return m_description; }
    const CYIString &GetResourceId() const { return m_resourceId; }
    const CYIString &GetTitle() const { return m_title; }

    const CYIDateTime &GetStartTime() const { return m_startTime; }
    const CYIDateTime &GetEndTime() const { return m_endTime; }
    
private:
    bool m_isFastForwardDisabled;
    bool m_isRestartable;
    bool m_isExtendable;
    bool m_isRecordable;
    bool m_isLookback;
    bool m_isPlayable;
    bool m_isDAI;

    int64_t m_duration;
    int64_t m_episodeNumber;
    int64_t m_seasonNumber;

    std::vector<CYIUrl> m_imageUrls{};
    
    CYIString m_programChannelId;
    CYIString m_parentalRating;
    CYIString m_episodeTitle;
    CYIString m_description;
    CYIString m_resourceId; // content identifier
    CYIString m_scheduleId;
    CYIString m_title;

    CYIDateTime m_startTime;
    CYIDateTime m_endTime;
};

#endif
