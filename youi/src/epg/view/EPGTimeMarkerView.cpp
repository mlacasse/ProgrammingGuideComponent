#include "EPGTimeMarkerView.h"

#include "epg/utility/NodeUtilities.h"

#include <utility/YiSceneNodeUtility.h>

YI_TYPE_DEF_INST(EPGTimeMarkerView, CYISceneView);

#define LOG_TAG "EPGTimeMarkerView"

EPGTimeMarkerView::EPGTimeMarkerView()
{
}

bool EPGTimeMarkerView::Init()
{
    bool isOk = CYISceneView::Init();
    
    if (isOk)
    {
        m_pTopLine = Utility::GetNode<CYISceneNode>(this, "Line-Top");
        m_pBottomLine = Utility::GetNode<CYISceneNode>(this, "Line-Bottom");
    }
    
    return isOk;
}

void EPGTimeMarkerView::SetPadding(float padding)
{
    m_padding = padding;
}

void EPGTimeMarkerView::SetListElementWidth(float width)
{
    m_listElementWidth = width;
}

void EPGTimeMarkerView::SetListWidth(float width)
{
    m_listWidth = width;
}

void EPGTimeMarkerView::SetDates(const CYIDateTime &startTime, const CYIDateTime &endTime)
{
    m_startTime = startTime;
    m_endTime = endTime;
}

void EPGTimeMarkerView::UpdateMarkerPosition(const CYIDateTime &currentTime, float percentScrolled)
{
    auto totalSeconds = m_endTime - m_startTime;
    float totalListWidth = (totalSeconds / (60.0f * 60.0f)) * m_listElementWidth;
    auto secondsElapsedSinceDayStart = currentTime - m_startTime;
    
    float percentageThroughCurrentDay = static_cast<float>(secondsElapsedSinceDayStart) / totalSeconds;
    float xPos = percentageThroughCurrentDay * totalListWidth;
    
    float offset = (totalListWidth - m_listWidth) * percentScrolled;
    xPos -= offset;
    xPos += m_padding;
    
    SetVisibility(xPos > m_padding);
    SetPositionX(xPos);
}

void EPGTimeMarkerView::UpdateMarkerGap(const glm::vec2 &startPosScreen, const glm::vec2 &endPosScreen)
{
    glm::vec3 startPos = CYISceneNodeUtility::ScreenToLocalSpace(this, startPosScreen);
    glm::vec3 endPos = CYISceneNodeUtility::ScreenToLocalSpace(this, endPosScreen);
    
    float topDelta = startPos.y - m_pTopLine->GetPosition().y;
    glm::vec3 topSize = m_pTopLine->GetSize();
    topSize.y = topDelta;
    m_pTopLine->SetSize(topSize);
    
    m_pBottomLine->SetPositionY(endPos.y);
}

void EPGTimeMarkerView::ResetMarkerGap()
{
    const float defaultOffset = 50.f;
    
    glm::vec3 topSize = m_pTopLine->GetSize();
    topSize.y = defaultOffset;
    m_pTopLine->SetSize(topSize);
    
    m_pBottomLine->SetPositionY(defaultOffset);
}
