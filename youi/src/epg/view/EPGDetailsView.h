#ifndef _EPG_DETAILS_VIEW_H_
#define _EPG_DETAILS_VIEW_H_

#include <signal/YiSignal.h>
#include <utility/YiDateTime.h>
#include <view/YiSceneView.h>

class EPGAssetModel;
class EPGChannelModel;

class CYIImageView;
class CYITextSceneNode;

class EPGDetailsView : public CYISceneView
{
public:
    EPGDetailsView();
    virtual ~EPGDetailsView() = default;

    bool Init() override;
    void Reset() override;

    void Format(const std::shared_ptr<EPGAssetModel> &pAssetModel, const std::shared_ptr<EPGChannelModel> &pChannelModel);

private:
    CYIString FormatTime(const CYIDateTime &time, bool appendMeridiem) const;

    CYITextSceneNode *m_pTitleText = nullptr;
    CYITextSceneNode *m_pMetadataText = nullptr;
    CYITextSceneNode *m_pDescriptionText = nullptr;
    CYIImageView *m_pImageView = nullptr;

    std::shared_ptr<EPGChannelModel> m_pChannelModel;

    YI_TYPE_BASES(EPGDetailsView, CYISceneView)
};

#endif
