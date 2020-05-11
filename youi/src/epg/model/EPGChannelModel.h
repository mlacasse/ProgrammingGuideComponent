#ifndef EPG_CHANNEL_MODEL_H
#define EPG_CHANNEL_MODEL_H

#include <folly/dynamic.h>

#include <utility/YiString.h>
#include <network/YiUrl.h>

#include <vector>

#include "EPGAssetModel.h"

class EPGChannelModel
{
public:
    EPGChannelModel();
    virtual ~EPGChannelModel() = default;

    bool Init(const folly::dynamic &value);

    bool IsFavorite() { return m_isFavorite; }
    void SetFavorite(bool state) { m_isFavorite = state; }

    uint32_t GetChannelNumber() const { return m_channelNumber; }
    const CYIString &GetChannelNumberString() const { return m_channelNumberString; }
    const CYIString &GetChannelId() const { return m_channelId; }
    const CYIString &GetCallSign() const { return m_callSign; }
    const CYIString &GetName() const { return m_name; }

    const CYIUrl &GetDefaultImageUrl() const { return m_defaultImageUrl; }
    const CYIUrl &GetImageUrl() const { return m_imageUrl; }

    const std::vector<std::shared_ptr<EPGAssetModel>> &GetAssetModels() const { return m_assets; };
    const std::shared_ptr<EPGAssetModel> &GetModelAtIndex(size_t index) const { return m_assets[index]; }
    const std::shared_ptr<EPGAssetModel> GetModelOnNow() const;
    size_t GetAssetCount() const { return m_assets.size(); }

private:
    bool m_isFavorite = false;
    uint32_t m_channelNumber = 0;

    CYIString m_channelNumberString;
    CYIString m_channelId;
    CYIString m_callSign;
    CYIString m_name;

    CYIUrl m_defaultImageUrl;
    CYIUrl m_imageUrl;

    std::vector<std::shared_ptr<EPGAssetModel>> m_assets;
};

#endif
