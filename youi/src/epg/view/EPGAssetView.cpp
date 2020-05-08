#include "EPGAssetView.h"

#include "epg/list/EPGChannelListView.h"
#include "epg/model/EPGAssetModel.h"
#include "epg/model/EPGChannelModel.h"
#include "epg/utility/NodeUtilities.h"

#include <layout/YiLinearLayout.h>
#include <scenetree/YiTextSceneNode.h>
#include <view/YiPushButtonView.h>

YI_TYPE_DEF_INST(EPGAssetView, CYISceneView);

#define LOG_TAG "EPGAssetView"

EPGAssetView::EPGAssetView()
{
}

bool EPGAssetView::Init()
{
    bool bOK = CYISceneView::Init();

    if (bOK)
    {
        m_pTitleText = Utility::GetNode<CYITextSceneNode>(this, "Scheduled-Name", Utility::NODE_FLAG_OPTIONAL);
        if (m_pTitleText)
        {
            m_pTitleText->SetMultiline(false);
            m_initialTextOffset = m_pTitleText->GetPosition().x;
        }
        
        m_pSelectButton = Utility::GetNode<CYIPushButtonView>(this);
        m_pSelectButton->ButtonPressed.Connect(*this, &EPGAssetView::OnButtonPressed);
        m_pSelectButton->GainedFocus.Connect(*this, &EPGAssetView::OnButtonGainedFocus);
        m_pSelectButton->LostFocus.Connect(*this, &EPGAssetView::OnButtonLostFocus);
    }

    return bOK;
}

void EPGAssetView::Format(const std::shared_ptr<EPGAssetModel>& pModel, int32_t id)
{
    m_pAssetModel = pModel;

    SetID(id);

    if (m_pTitleText)
    {
        m_pTitleText->SetText(pModel->GetTitle());
    }
}

void EPGAssetView::SetListView(EPGChannelListView *pListView)
{
    m_pListView = pListView;
    auto pLayout = YiDynamicCast<CYILinearLayout>(m_pListView->GetLayout());
    m_listPadding = pLayout->GetPadding();
}

void EPGAssetView::SetCardSize(const glm::vec3 &size)
{
    SetSize(size);
}

void EPGAssetView::UpdateContentAlignment(float scrollPercent)
{
    if (m_pTitleText == nullptr)
    {
        return;
    }

    SetScrollPosition(scrollPercent);
    
    if (GetParent() == nullptr)
    {
        return;
    }
    
    float visibleLeft = m_listPadding.GetLeft() + scrollPercent * (m_listPadding.GetTotalX() + m_channelWidth) - scrollPercent * m_pListView->GetSize().x;
    
    if (GetParent()->GetPosition().x < visibleLeft)
    {
        m_isAlignmentModified = true;
        
        glm::vec3 textNodeSize = m_pTitleText->GetSize();
        
        float offsetPos = std::min(GetParent()->GetSize().x - textNodeSize.x - m_initialTextOffset, visibleLeft - GetParent()->GetPosition().x + m_initialTextOffset);
        offsetPos = std::max(m_initialTextOffset, offsetPos);

        m_pTitleText->SetPositionX(offsetPos);
    }
    else if (m_isAlignmentModified)
    {
        m_isAlignmentModified = false;
        
        m_pTitleText->SetPositionX(m_initialTextOffset);
    }
}

void EPGAssetView::ApplyMeasurements()
{
    CYISceneView::ApplyMeasurements();

    if (m_pTitleText && m_isFirstUpdate && GetParent() != nullptr)
    {
        SetScrollPosition(m_scrollPosition);
        
        m_pTitleText->SetTextMaxWidth(glm::max(GetParent()->GetSize().x - m_initialTextOffset, 0.1f));
        
        m_isFirstUpdate = false;
    }
}

void EPGAssetView::Reset()
{
    m_pAssetModel.reset();
    SetID(-1);
    
    if (m_pTitleText)
    {
        m_pTitleText->SetText(CYIString::EmptyString());
        m_pTitleText->SetPositionX(m_initialTextOffset);
    }

    m_isAlignmentModified = false;
    m_isFirstUpdate = true;
}

void EPGAssetView::OnButtonPressed(int32_t /*id*/)
{
    AssetSelected.Emit(GetID());
}

void EPGAssetView::OnButtonGainedFocus()
{
    AssetGainedFocus.Emit(GetID());
}

void EPGAssetView::OnButtonLostFocus()
{
    AssetLostFocus.Emit(GetID());
}
