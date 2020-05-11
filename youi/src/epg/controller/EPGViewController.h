#ifndef _EPG_VIEW_CONTROLLER_H_
#define _EPG_VIEW_CONTROLLER_H_

#include "epg/controller/EPGSideMenuViewController.h"

#include <signal/YiSignal.h>
#include <signal/YiSignalHandler.h>
#include <utility/YiTimer.h>

class EPGAssetModel;
class EPGChannelModel;
class EPGChannelListAdapter;
class EPGFilterModel;
class EPGListView;
class EPGModel;
class EPGTimeListAdapter;
class EPGTimeMarkerView;

class CYIActivityIndicatorView;
class CYIListView;
class CYISceneView;

class EPGViewController : public CYISignalHandler
{
public:
    EPGViewController();
    virtual ~EPGViewController();
  
    virtual void Init(CYISceneView *pView);
    void SetFilters(std::vector<EPGFilterModel> &&filters);
    void Populate(std::shared_ptr<EPGModel> data);
    void Reset();
    void SetFocusedAssetIds(const CYIString &channelId, const CYIString &assetId);
    void SetFocus();
    void RestoreFocus();

    void ShowActivityIndicator();
    void HideActivityIndicator();

    CYISignal<const std::shared_ptr<EPGAssetModel> &, const std::shared_ptr<EPGChannelModel> &> AssetSelected;
    CYISignal<const std::shared_ptr<EPGAssetModel> &, const std::shared_ptr<EPGChannelModel> &> AssetFocused;
    CYISignal<const std::shared_ptr<EPGAssetModel> &, const std::shared_ptr<EPGChannelModel> &> AssetUnfocused;

    CYISignal<const std::shared_ptr<EPGChannelModel> &, bool> ChannelFavoriteButtonToggled;
    CYISignal<const std::shared_ptr<EPGChannelModel> &> ChannelButtonPressed;

private:
    void InitTimeList(CYISceneView *pView);
    void InitCurrentTimeMarker(CYISceneView *pView);
    void OnChannelListScrolled(float scrolledPosition);
    void OnFilterEnabled(const EPGFilterModel &filter);
    void OnAssetGainedFocus(const std::shared_ptr<EPGAssetModel>& pAsset, const std::shared_ptr<EPGChannelModel> &pChannel);
    void OnAssetLostFocus(const std::shared_ptr<EPGAssetModel>& pAsset, const std::shared_ptr<EPGChannelModel> &pChannel);

    void OnTick();
    
    CYISceneView *m_pView = nullptr;
    EPGListView *m_pEPGListView = nullptr;
    CYIListView *m_pTimeListView = nullptr;
    EPGChannelListAdapter *m_pEPGListAdapter;
    EPGTimeListAdapter *m_pTimeListAdapter;
    EPGTimeMarkerView *m_pCurrentTimeMarker = nullptr;
    EPGSideMenuViewController m_sideMenuViewController;
    CYIActivityIndicatorView *m_pActivityIndicatorView;
    
    CYITimer m_tickTimer;

    std::shared_ptr<EPGModel> m_pModel;

    CYIString m_lastFocusedAssetId;
    CYIString m_lastFocusedChannelId;

    bool m_didRestoreFocus = false;
};

#endif
