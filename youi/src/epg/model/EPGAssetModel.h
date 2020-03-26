#ifndef EPG_ASSET_MODEL_H
#define EPG_ASSET_MODEL_H

#include <folly/dynamic.h>

#include <utility/YiDateTime.h>
#include <utility/YiString.h>
#include <network/YiUrl.h>

class EPGAssetModel
{
public:
    EPGAssetModel(bool isRestartable, int64_t duration, int64_t episodeNumber, int64_t seasonNumber, CYIString parentalRating, CYIString episodeTitle, CYIString description, CYIString resourceId, CYIString title, CYIString startTime, CYIString endTime);
    virtual ~EPGAssetModel() = default;

    void SetCategories(std::vector<CYIString> categories) { m_categories = categories; }
    void SetGenres(std::vector<CYIString> genres) { m_genres = genres; }
    void SetImageUrls(std::vector<CYIUrl> imageUrls) { m_imageUrls = imageUrls; }

    bool isRestartable() { return m_isRestartable; }

    int64_t GetDuration() { return m_duration; }
    int64_t GetEpisodeNumber() { return m_episodeNumber; }
    int64_t GetSeasonNumber() { return m_seasonNumber; }

    const std::vector<CYIString> GetCategories() const { return m_categories; }
    const std::vector<CYIString> GetGenres() const { return m_genres; }
    const std::vector<CYIUrl> GetImageUrls() const { return m_imageUrls; }

    const CYIString GetParentalRating() const { return m_parentalRating; }
    const CYIString GetEpisodeTitle() const { return m_episodeTitle; }
    const CYIString GetDescription() const { return m_description; }
    const CYIString GetResourceId() const { return m_resourceId; }
    const CYIString GetTitle() const { return m_title; }

    const CYIDateTime GetStartTime() const { return m_startTime; }
    const CYIDateTime GetEndTime() const { return m_endTime; }

private:
    bool m_isRestartable;

    int64_t m_duration;

    int64_t m_episodeNumber;
    int64_t m_seasonNumber;
    
    std::vector<CYIString> m_categories{};
    std::vector<CYIString> m_genres{};

    std::vector<CYIUrl> m_imageUrls{};

    CYIString m_parentalRating;
    CYIString m_episodeTitle;
    CYIString m_description;
    CYIString m_resourceId; // content identifier
    CYIString m_title;

    CYIDateTime m_startTime;
    CYIDateTime m_endTime;
};

#endif
