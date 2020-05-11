#include "ShadowProgrammingGuideView.h"

#include "../controller/EPGViewController.h"
#include "../list/EPGListView.h"
#include "../model/EPGModel.h"
#include "../utility/NodeUtilities.h"

#include <youireact/NativeModuleRegistry.h>
#include <youireact/props/DirectEventTypes.h>

#include <asset/YiAssetViewTemplate.h>
#include <framework/YiAppContext.h>
#include <framework/YiScreen.h>
#include <import/YiViewTemplate.h>

#if YI_OSX
#include <AppFactory.h>
#endif

using namespace yi::react;

#define LOG_TAG "ShadowProgrammingGuideView"

YI_TYPE_DEF_INST(ShadowProgrammingGuideView, ShadowView)

YI_RN_REGISTER_VIEW_MODULE(ShadowProgrammingGuideView);

YI_RN_DEFINE_DIRECT_EVENT_NAME(ShadowProgrammingGuideView, onHandleGuideChannelPressName);
YI_RN_DEFINE_DIRECT_EVENT_NAME(ShadowProgrammingGuideView, onHandleFavoriteIconPressName);
YI_RN_DEFINE_DIRECT_EVENT_NAME(ShadowProgrammingGuideView, onHandleGuideContentPressName);
YI_RN_DEFINE_DIRECT_EVENT_NAME(ShadowProgrammingGuideView, onHandleGuideContentFocusName);
YI_RN_DEFINE_DIRECT_EVENT_NAME(ShadowProgrammingGuideView, onHandleGuideContentBlurName);

ShadowProgrammingGuideView::ShadowProgrammingGuideView()
{
    m_savedProps.reset(new ProgrammingGuideProps());

    SetDirectEvent(DirectEvents::onLayout, onLayout);
    SetDirectEvent(DirectEvents::onFocus, onFocus);
    SetDirectEvent(DirectEvents::onBlur, onBlur);

    SetDirectEvent(ShadowProgrammingGuideView::onHandleGuideContentPressName, onHandleGuideContentPress);
    SetDirectEvent(ShadowProgrammingGuideView::onHandleGuideContentFocusName, onHandleGuideContentFocus);
    SetDirectEvent(ShadowProgrammingGuideView::onHandleGuideContentBlurName, onHandleGuideContentBlur);
    SetDirectEvent(ShadowProgrammingGuideView::onHandleGuideChannelPressName, onHandleGuideChannelPress);
    SetDirectEvent(ShadowProgrammingGuideView::onHandleFavoriteIconPressName, onHandleFavoriteIconPress);

    GetYogaNode()->SetYogaMeasureFunction(*this, &ShadowProgrammingGuideView::YogaMeasure);
}

ShadowProgrammingGuideView::~ShadowProgrammingGuideView() = default;

ProgrammingGuideProps *ShadowProgrammingGuideView::GetSavedProps()
{
    return static_cast<ProgrammingGuideProps *>(ShadowView::GetSavedProps());
}

std::unique_ptr<CYISceneNode> ShadowProgrammingGuideView::CreateCounterpart(CYISceneManager *pSceneManager)
{
    auto viewTemplate = CYIViewTemplate::GetViewTemplate("EPG_Main");
    
    auto pOwnedView = viewTemplate->BuildView(pSceneManager);
    pOwnedView->Init();
    CYISceneView *pView = pOwnedView.get();
    
    m_controller.Init(pView);

    m_controller.AssetSelected.Connect(*this, &ShadowProgrammingGuideView::OnAssetSelected);
    m_controller.AssetFocused.Connect(*this, &ShadowProgrammingGuideView::OnAssetFocused);
    m_controller.AssetUnfocused.Connect(*this, &ShadowProgrammingGuideView::OnAssetUnfocused);
    m_controller.ChannelButtonPressed.Connect(*this, &ShadowProgrammingGuideView::OnChannelButtonPressed);
    m_controller.ChannelFavoriteButtonToggled.Connect(*this, &ShadowProgrammingGuideView::OnChannelFavoriteButtonToggled);

    m_pModel = std::make_shared<EPGModel>();

    return pOwnedView;
}

const CYISceneView *ShadowProgrammingGuideView::GetCounterpart() const
{
    return static_cast<const CYISceneView *>(ShadowView::GetCounterpart());
}

CYISceneView *ShadowProgrammingGuideView::GetCounterpart()
{
    return static_cast<CYISceneView *>(ShadowView::GetCounterpart());
}

YGSize ShadowProgrammingGuideView::YogaMeasure(YGNodeRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode) const
{
    YI_UNUSED(node);
    YI_UNUSED(widthMode);
    YI_UNUSED(heightMode);

    return {width, height};
}

folly::dynamic ShadowProgrammingGuideView::GetDefaultProps() const
{
    //Default Props
    folly::dynamic defaultProps = folly::dynamic::object;

    defaultProps["currentChannelIndex"] = folly::dynamic::object("channelId", "")("resourceId", "");
    defaultProps["schedulesStartTime"] = ToDynamic(CYIDateTime().FormatString());
    defaultProps["dateSelector"] = folly::dynamic::object("label", "")("days", 14);
    defaultProps["sortKeys"] = folly::dynamic::array;
    defaultProps["filters"] = folly::dynamic::array;
    defaultProps["duration"] = 6;

    return defaultProps;
}

void ShadowProgrammingGuideView::OnLayoutApplied()
{
    ShadowView::OnLayoutApplied();
    
     GetCounterpart()->SetScale(GetScaleFactor());

    EmitDirectEvent(DirectEvents::onLayout);
}

void ShadowProgrammingGuideView::Reset()
{
    m_controller.Reset();
    m_pModel = std::make_shared<EPGModel>();

    CYIDateTime startTime = GetSavedProps()->startTime;
    startTime.SetMinute(startTime.GetMinute() > 30 ? 30 : 0);
    m_pModel->InitTime(startTime, GetSavedProps()->duration * 60);

    m_controller.ShowActivityIndicator();
    m_controller.SetFocusedAssetIds(GetSavedProps()->currentChannelId, GetSavedProps()->currentAssetId);
    m_controller.Populate(m_pModel);
}

void ShadowProgrammingGuideView::PrependChannels(const std::vector<std::shared_ptr<EPGChannelModel>> &channels)
{
    if (channels.size() > 0)
    {
        m_pModel->PrependChannelModels(channels);
        m_controller.HideActivityIndicator();
        m_controller.RestoreFocus();
    }
}

void ShadowProgrammingGuideView::AppendChannels(const std::vector<std::shared_ptr<EPGChannelModel>> &channels)
{
    if (channels.size() > 0)
    {
        m_pModel->AppendChannelModels(channels);
        m_controller.HideActivityIndicator();
        m_controller.RestoreFocus();
    }
}

void ShadowProgrammingGuideView::ResetChannelFocus()
{
    m_controller.SetFocusedAssetIds(GetSavedProps()->currentChannelId, GetSavedProps()->currentAssetId);
    m_controller.SetFocus();
}

void ShadowProgrammingGuideView::SetFilters(std::vector<EPGFilterModel> &&filters)
{
    m_controller.SetFilters(std::move(filters));
}

void ShadowProgrammingGuideView::ApplyProps(folly::dynamic properties)
{
    ShadowView::ApplyProps(std::move(properties));

    Reset();
}

glm::vec3 ShadowProgrammingGuideView::GetScaleFactor() const
{
    glm::vec3 scale(1.f, 1.f, 1.f);
    
    float scaleRatio = 1.f;

#ifndef YI_OSX    
    CYISurface *pSurface = CYIAppContext::GetInstance()->GetSurface();
    if (pSurface)
    {
        scaleRatio = pSurface->GetWidth() / 1920.f;
    }
#else
    CYISurface *pSurface = CYIAppContext::GetInstance()->GetSurface();
    if (pSurface)
    {
        scaleRatio = AppFactory::GetWindowWidth() / static_cast<float>(pSurface->GetWidth());
    }
#endif

    scale.x = scaleRatio;
    scale.y = scaleRatio;

    return scale;
}

void ShadowProgrammingGuideView::SetSize(CYISceneNode *pNode, float x, float y, float z)
{
    SetSize(pNode, glm::vec3(x, y, z));
}

void ShadowProgrammingGuideView::SetSize(CYISceneNode *pNode, const glm::vec3 &size)
{
    if (pNode)
    {
        pNode->SetSize(size);
    }
}

void ShadowProgrammingGuideView::OnAssetSelected(const std::shared_ptr<EPGAssetModel> &asset, const std::shared_ptr<EPGChannelModel> &channel)
{
    if (asset != nullptr)
    {
        folly::dynamic content = folly::dynamic::object;
        
        content["channelId"] = ToDynamic(asset->GetProgramChannelId());
        content["resourceId"] = ToDynamic(asset->GetResourceId());

        EmitDirectEvent(ShadowProgrammingGuideView::onHandleGuideContentPressName, content);
    }
}

void ShadowProgrammingGuideView::OnAssetFocused(const std::shared_ptr<EPGAssetModel> &asset, const std::shared_ptr<EPGChannelModel> &channel)
{
    if (asset != nullptr)
    {
        folly::dynamic content = folly::dynamic::object;
        
        content["channelId"] = ToDynamic(asset->GetProgramChannelId());
        content["resourceId"] = ToDynamic(asset->GetResourceId());

        EmitDirectEvent(ShadowProgrammingGuideView::onHandleGuideContentFocusName, content);
    }
}

void ShadowProgrammingGuideView::OnAssetUnfocused(const std::shared_ptr<EPGAssetModel> &asset, const std::shared_ptr<EPGChannelModel> &channel)
{
    if (asset != nullptr)
    {
        folly::dynamic content = folly::dynamic::object;
        
        content["channelId"] = ToDynamic(asset->GetProgramChannelId());
        content["resourceId"] = ToDynamic(asset->GetResourceId());

        EmitDirectEvent(ShadowProgrammingGuideView::onHandleGuideContentBlurName, content);
    }
}

void ShadowProgrammingGuideView::OnChannelButtonPressed(const std::shared_ptr<EPGChannelModel> &channel)
{
    EmitDirectEvent(ShadowProgrammingGuideView::onHandleGuideChannelPressName, folly::dynamic::object("channelId", ToDynamic(channel->GetChannelId())));
}

void ShadowProgrammingGuideView::OnChannelFavoriteButtonToggled(const std::shared_ptr<EPGChannelModel> &channel, bool isFavorite)
{
    YI_UNUSED(isFavorite);
    EmitDirectEvent(ShadowProgrammingGuideView::onHandleFavoriteIconPressName, folly::dynamic::object("channelId", ToDynamic(channel->GetChannelId())));
}
