#ifndef EPG_CHANNEL_MODEL_H
#define EPG_CHANNEL_MODEL_H

#include <folly/dynamic.h>

#include <utility/YiString.h>
#include <network/YiUrl.h>

#include <vector>

class EPGAssetModel;

class EPGChannelModel
{
public:
    EPGChannelModel(bool isFavorite, uint32_t channelNumber, CYIString channelId, CYIString callSign, CYIString name, CYIUrl imageUrl);
    virtual ~EPGChannelModel();

    bool IsFavorite() { return m_isFavorite; }

    uint32_t GetChannelNumber() { return m_channelNumber; }

    const CYIString GetChannelId() const { return m_channelId; }
    const CYIString GetCallSign() const { return m_callSign; }
    const CYIString GetName() const { return m_name; }

    const CYIUrl GetImageUrl() const { return m_imageUrl; }

    void SetAssetModels(std::vector<std::shared_ptr<EPGAssetModel> > assets) { m_assets = assets; };
    const std::vector<std::shared_ptr<EPGAssetModel> > GetAssetModels() const { return m_assets; };
    const std::shared_ptr<EPGAssetModel> GetAssetModel(size_t index) const { return m_assets.at(index); }

private:
    bool m_isFavorite;

    uint32_t m_channelNumber;

    CYIString m_channelId;
    CYIString m_callSign;
    CYIString m_name;
    
    CYIUrl m_imageUrl;

    std::vector<std::shared_ptr<EPGAssetModel>> m_assets;
};

#endif
