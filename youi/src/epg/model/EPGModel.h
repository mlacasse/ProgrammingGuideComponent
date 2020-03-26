#ifndef EPG_MODEL_H
#define EPG_MODEL_H

#include <utility/YiDateTime.h>
#include <network/YiUrl.h>

#include <map>
#include <vector>

class EPGAssetModel;
class EPGChannelModel;

class EPGModel
{
public:
    EPGModel();
    virtual ~EPGModel();

    void SetModel(std::vector<std::shared_ptr<EPGChannelModel> > channelData) { m_channelData = channelData; }

    const std::shared_ptr<EPGChannelModel> GetChannelModel(const CYIString &channelId);
    const std::vector<std::shared_ptr<EPGChannelModel> > GetChannelModels() const { return m_channelData; }

private:
    std::vector<std::shared_ptr<EPGChannelModel> > m_channelData;
};

#endif
