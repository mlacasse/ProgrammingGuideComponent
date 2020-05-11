#ifndef EPG_MODEL_H
#define EPG_MODEL_H

#include <signal/YiSignal.h>
#include <utility/YiDateTime.h>

#include <vector>

class EPGAssetModel;
class EPGChannelModel;

class EPGModel
{
public:
    EPGModel();
    virtual ~EPGModel();

    void InitTime(CYIDateTime startTime, int32_t durationInMinutes);
    void SetChannelModels(std::vector<std::shared_ptr<EPGChannelModel>> &&channelData);
    void PrependChannelModels(const std::vector<std::shared_ptr<EPGChannelModel>> &channelData);
    void AppendChannelModels(const std::vector<std::shared_ptr<EPGChannelModel>> &channelData);
    const std::shared_ptr<EPGChannelModel> GetChannelModel(const CYIString &channelId);
    const std::vector<std::shared_ptr<EPGChannelModel> > GetChannelModels() const { return m_channelData; }
    const std::vector<std::shared_ptr<EPGChannelModel>> GetFavoriteChannelModels();
    const std::vector<std::shared_ptr<EPGChannelModel>> GetChannelModelsForCategory(const CYIString &genre);

    const CYIDateTime &GetStartTime() const { return m_startTime; }
    const CYIDateTime &GetEndTime() const { return m_endTime; }

    CYISignal<size_t> ChannelAddedAtIndex;

private:
    std::vector<std::shared_ptr<EPGChannelModel> > m_channelData;
    
    CYIDateTime m_startTime;
    CYIDateTime m_endTime;
};

#endif
