#ifndef _EPG_ON_NOW_VIEW_CONTROLLER_H_
#define _EPG_ON_NOW_VIEW_CONTROLLER_H_

#include <animation/YiAnimationToggleController.h>
#include <signal/YiSignal.h>
#include <signal/YiSignalHandler.h>
#include <utility/YiTimer.h>

class EPGAssetModel;
class EPGChannelModel;
class EPGOnNowListAdapter;

class CYIListView;
class CYISceneView;
class CYITextSceneNode;

class EPGOnNowViewController : public CYISignalHandler
{
public:
    EPGOnNowViewController();
    virtual ~EPGOnNowViewController();

    virtual void Init(CYISceneView *pView);
    void Populate(const CYIString &label, const std::vector<std::shared_ptr<EPGChannelModel>> &data);
    void Reset();
    void SetVisibility(bool isVisible);
    void RestoreFocus();

    CYISignal<const std::shared_ptr<EPGAssetModel> &, const std::shared_ptr<EPGChannelModel> &> AssetSelected;
    CYISignal<const std::shared_ptr<EPGChannelModel> &, bool> ChannelFavoriteButtonToggled;
    CYISignal<const std::shared_ptr<EPGChannelModel> &> ChannelButtonPressed;

private:
    void OnTick();

    CYISceneView *m_pView = nullptr;
    CYITextSceneNode *m_pHeaderText = nullptr;
    CYITextSceneNode *m_pDateText = nullptr;
    CYIListView *m_pListView = nullptr;
    EPGOnNowListAdapter *m_pListAdapter = nullptr;

    CYITimer m_tickTimer;
    CYIAnimationToggleController m_showHideToggleController;
};

#endif
