#include "ShadowProgrammingGuideView.h"

#include <youireact/NativeModuleRegistry.h>

using namespace yi::react;

#define LOG_TAG "ShadowProgrammingGuideView"

YI_TYPE_DEF_INST(ShadowProgrammingGuideView, ShadowView)

YI_RN_REGISTER_VIEW_MODULE(ShadowProgrammingGuideView);

ShadowProgrammingGuideView::ShadowProgrammingGuideView()
{
    m_savedProps.reset(new ProgrammingGuideProps());
}

ShadowProgrammingGuideView::~ShadowProgrammingGuideView() = default;

ProgrammingGuideProps *ShadowProgrammingGuideView::GetSavedProps()
{
    return static_cast<ProgrammingGuideProps *>(ShadowView::GetSavedProps());
}

std::unique_ptr<CYISceneNode> ShadowProgrammingGuideView::CreateCounterpart(CYISceneManager *pSceneManager)
{
    std::unique_ptr<CYISceneNode> pSceneNode(new CYISceneNode());

    pSceneNode->SetSceneManager(pSceneManager);
    pSceneNode->Init();

    return std::unique_ptr<CYISceneNode>{pSceneNode.release()};
}

const CYISceneNode *ShadowProgrammingGuideView::GetCounterpart() const
{
    return static_cast<const CYISceneNode *>(ShadowView::GetCounterpart());
}

CYISceneNode *ShadowProgrammingGuideView::GetCounterpart()
{
    return static_cast<CYISceneNode *>(ShadowView::GetCounterpart());
}

folly::dynamic ShadowProgrammingGuideView::GetDefaultProps() const
{
    //Default Props
    folly::dynamic defaultProps = folly::dynamic::object;

    return defaultProps;
}

void ShadowProgrammingGuideView::OnLayoutApplied()
{
    // DO SOMETHING
}

void ShadowProgrammingGuideView::ApplyProps(folly::dynamic properties)
{
    ShadowView::ApplyProps(std::move(properties));
    
    // DO SOMETHING
}
