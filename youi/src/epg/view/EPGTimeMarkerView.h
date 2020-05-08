#ifndef _EPG_TIME_MARKER_VIEW_H
#define _EPG_TIME_MARKER_VIEW_H

#include <view/YiSceneView.h>
#include <utility/YiDateTime.h>

class EPGTimeMarkerView : public CYISceneView
{
public:
    EPGTimeMarkerView();
    virtual ~EPGTimeMarkerView() = default;
    
    bool Init() override;

    void SetPadding(float padding);
    void SetListElementWidth(float width);
    void SetListWidth(float width);
    
    void SetDates(const CYIDateTime &startTime, const CYIDateTime &endTime);
    void UpdateMarkerPosition(const CYIDateTime &currentTime, float percentScrolled);
    void UpdateMarkerGap(const glm::vec2 &startPosScreen, const glm::vec2 &endPosScreen);
    void ResetMarkerGap();
    
private:
    CYISceneNode *m_pTopLine = nullptr;
    CYISceneNode *m_pBottomLine = nullptr;
    
    float m_padding = 0.0f;
    float m_listElementWidth = 0.0f;
    float m_listWidth = 0.0f;
    CYIDateTime m_startTime;
    CYIDateTime m_endTime;
    
    YI_TYPE_BASES(EPGTimeMarkerView, CYISceneView)
};

#endif
