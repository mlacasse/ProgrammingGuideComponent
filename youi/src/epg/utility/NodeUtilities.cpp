#include "epg/utility/NodeUtilities.h"

#include <layout/YiLayout.h>

namespace Utility
{

void InsertChild(CYISceneNode *pParent, std::unique_ptr<CYISceneNode> pChild, size_t nIndex)
{
    YI_ASSERT(pParent && pChild, TAG, "Parent and child must not be NULL!");
    YI_ASSERT(pParent != pChild.get(), TAG, "Parent must not be child!");
    YI_ASSERT(nIndex <= pParent->GetChildCount(), TAG, "Index out of range!");

    CYISceneView *pParentAsView = YiDynamicCast<CYISceneView>(pParent);

    if (pParentAsView != nullptr)
    {
        InsertChild(pParentAsView, std::move(pChild), nIndex);
    }
    else
    {
        pParent->AddChild(std::move(pChild), nIndex);
    }
}

void InsertChild(CYISceneView *pParent, std::unique_ptr<CYISceneNode> pChild, size_t nIndex)
{
    YI_ASSERT(pParent && pChild, TAG, "Parent and child must not be NULL!");
    YI_ASSERT(pParent != pChild.get(), TAG, "Parent must not be child!");
    YI_ASSERT(nIndex <= pParent->GetChildCount(), TAG, "Index out of range!");

    CYILayout *pParentLayout = pParent->GetLayout();
    if (pParentLayout != nullptr && pParentLayout->IsUsingReverseChildrenOrder())
    {
        nIndex = pParent->GetChildCount() - nIndex;
    }

    pParent->AddChild(std::move(pChild), nIndex);
}

void AppendChild(CYISceneNode *pParent, std::unique_ptr<CYISceneNode> pChild)
{
    YI_ASSERT(pParent, TAG, "Parent is NULL!");
    InsertChild(pParent, std::move(pChild), pParent->GetChildCount());
}

void AppendChild(CYISceneView *pParent, std::unique_ptr<CYISceneNode> pChild)
{
    YI_ASSERT(pParent, TAG, "Parent is NULL!");
    InsertChild(pParent, std::move(pChild), pParent->GetChildCount());
}

} // namespace Utility
