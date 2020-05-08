#ifndef _NODE_UTILITIES_H_
#define _NODE_UTILITIES_H_

#include <asset/YiAssetViewTemplate.h>
#include <import/YiViewTemplate.h>
#include <scenetree/YiSceneNode.h>
#include <scenetree/YiTextSceneNode.h>
#include <view/YiScrollingView.h>
#include <view/YiPushButtonView.h>

// Covers all platforms
#if !(defined YI_DEBUG)
#if defined (DEBUG) || defined (_DEBUG) || !(defined (NDEBUG) || defined (_NDEBUG))
#define YI_DEBUG 1
#endif
#endif

// Used to mark text for translation in situations
// where it isn't possible to use YiTranslate.
#define YI_TRANSLATE_MARKER(text) (text)

namespace Utility
{
    enum GET_NODE_FLAG
    {
        NODE_FLAG_MANDATORY,
        NODE_FLAG_OPTIONAL
    };
    
    const CYIString TAG("Utility");
    
    const size_t IndexedNodeLimit = 100;
    
    /*!
     \brief Gets a node with the given type, failing with an assertion if the node type cannot be found(if the bOptionalFlag is set to MANDATORY).
     Note that the name is not required for this method.
     */
    template<typename Type>
    inline Type *GetNode(CYISceneNode *pAncestor, GET_NODE_FLAG bOptionalFlag = NODE_FLAG_MANDATORY)
    {
        Type *pNode = nullptr;
        
        YI_ASSERT(pAncestor, TAG, "Ancestor node is null");
        pNode = pAncestor->GetNode<Type>();
        
        if (bOptionalFlag == NODE_FLAG_MANDATORY)
        {
            YI_ASSERT(pNode, TAG, "Missing node from ancestor: %s", pAncestor->GetName().GetData());
        }
        
        return pNode;
    }
    
    /*!
     \brief Searches children for the first child of the given node type.
     */
    template<typename Type>
    Type *GetImmediateChildNode(CYISceneNode *pAncestor, GET_NODE_FLAG eOptionalFlag)
    {
        Type *pNode = nullptr;
        
        YI_ASSERT(pAncestor, TAG, "Ancestor node is null");
        
        size_t uChildCount = pAncestor->GetChildCount();
        
        for (size_t uIndex = 0; uIndex < uChildCount; ++uIndex)
        {
            auto *pChild = pAncestor->GetChild(uIndex);
            pNode = YiDynamicCast<Type>(pChild);
            if (pNode != nullptr)
            {
                break;
            }
        }
        
        YI_ASSERT((eOptionalFlag == NODE_FLAG_OPTIONAL) || pNode, TAG, "Missing node from ancestor: %s", pAncestor->GetName().GetData());
        
        return pNode;
    }
    
    /*!
     \brief Gets a node with the given name and type, failing with an
     assertion if the node cannot be found(if the bOptionalFlag is set to MANDATORY) or is the wrong type.
     */
    template<typename Type>
    inline Type *GetNode(CYISceneNode *pAncestor, const CYIString &rName, GET_NODE_FLAG bOptionalFlag = NODE_FLAG_MANDATORY)
    {
        Type *pNode = nullptr;
        
        if (!rName.IsEmpty())
        {
            YI_ASSERT(pAncestor, TAG, "Ancestor node is null");
            pNode = pAncestor->GetNode<Type>(rName);
            
            if (bOptionalFlag == NODE_FLAG_MANDATORY)
            {
                YI_ASSERT(pNode, TAG, "Missing node: %s", rName.GetData());
            }
        }
        return pNode;
    }
    
    /*!
     \brief Gets a node with the given name, failing
     with an assertion if the node cannot be found if the bOptionalFlag is set to MANDATORY.
     */
    inline CYISceneNode *GetNode(CYISceneNode *pAncestor, const CYIString &rName, GET_NODE_FLAG bOptionalFlag = NODE_FLAG_MANDATORY)
    {
        return GetNode<CYISceneNode>(pAncestor, rName, bOptionalFlag);
    }
    
    /*!
     \brief Returns a vector of nodes of the given type whose names match
     the given name suffixed with an integral index starting at 0. Given the
     name "Node", this function would return a vector of nodes matching the
     names "Node0", "Node1", "Node2", etc. up to a limit of 100 nodes. The
     first missing node stops the search for nodes.
     */
    template<typename Type>
    inline std::vector<Type *> GetIndexedNodes(CYISceneNode *pAncestor, const CYIString &rName, size_t uStartIndex=0)
    {
        std::vector<Type *> nodes;
        
        if (!rName.IsEmpty())
        {
            YI_ASSERT(pAncestor, TAG, "Ancestor node is null");
            
            for (size_t uIndex = uStartIndex; uIndex < IndexedNodeLimit; ++uIndex)
            {
                if (Type *pNode = pAncestor->GetNode<Type>(rName + CYIString::FromValue(uIndex)))
                {
                    nodes.push_back(pNode);
                }
                else
                {
                    break;
                }
            }
        }
        
        return nodes;
    }
    
    /*!
     \brief Returns a vector of nodes type whose names match the given
     name suffixed with an integral index starting at 0. Given the name
     "Node", this function would return a vector of nodes matching the names
     "Node0", "Node1", "Node2", etc. up to a limit of 100 nodes. The first
     missing node stops the search for nodes.
     */
    inline std::vector<CYISceneNode *> GetIndexedNodes(CYISceneNode *pAncestor, const CYIString &rName, size_t uStartIndex=0)
    {
        return GetIndexedNodes<CYISceneNode>(pAncestor, rName, uStartIndex);
    }
    
    /*!
     \brief Returns the property with the given name or false if
     the property is missing or can't be converted to the given type.
     */
    template<typename Type>
    inline bool GetProperty(const CYISceneNode *pNode, const CYIString &propertyName, Type &propertyValue)
    {
        YI_ASSERT(pNode, TAG, "Node is null");
        
        CYIString propertyString;
        if (pNode->GetProperty(propertyName, &propertyString) && propertyString.IsNotEmpty())
        {
            bool bError = false;
            propertyValue = propertyString.To<Type>(&bError);
            return !bError;
        }
        return false;
    }
    
    /*!
     \brief A specialization of template<typename Type>GetProperty() for bool.
     */
    template<>
    inline bool GetProperty<bool>(const CYISceneNode *pNode, const CYIString &propertyName, bool &propertyValue)
    {
        YI_ASSERT(pNode, TAG, "Node is null");
        
        CYIString propertyString;
        if (pNode->GetProperty(propertyName, &propertyString) && propertyString.IsNotEmpty())
        {
            propertyValue = propertyString == "true";
            return true;
        }
        return false;
    }
    
    /*!
     \brief A specialization of template<typename Type>GetProperty() for CYIString.
     */
    template<>
    inline bool GetProperty<CYIString>(const CYISceneNode *pNode, const CYIString &propertyName, CYIString &propertyValue)
    {
        YI_ASSERT(pNode, TAG, "Node is null");
        
        if (pNode->GetProperty(propertyName, &propertyValue) && propertyValue.IsNotEmpty())
        {
            return true;
        }
        return false;
    }
    
    template<typename Type>
    CYIPushButtonView *ConnectButton(CYISceneView &pView, const CYIString &rButtonName, GET_NODE_FLAG eNodeFlag, Type &signal)
    {
        CYIPushButtonView *pButton = GetNode<CYIPushButtonView>(&pView, rButtonName, eNodeFlag);
        if (pButton != nullptr)
        {
            pButton->ButtonClicked.Connect(signal);
        }
        return pButton;
    }
    
    template<typename Type>
    CYIPushButtonView *ConnectButton(CYISceneView &pView, const CYIString &rButtonName, GET_NODE_FLAG eNodeFlag, Type &object, void (Type::*pFunction)(int32_t))
    {
        CYIPushButtonView *pButton = GetNode<CYIPushButtonView>(&pView, rButtonName, eNodeFlag);
        if (pButton != nullptr)
        {
            pButton->ButtonClicked.Connect(object, pFunction);
        }
        return pButton;
    }
    
    template<typename Type>
    CYIPushButtonView *ConnectButton(CYISceneView &pView, const CYIString &rButtonName, GET_NODE_FLAG eNodeFlag, Type &rObject, void (Type::*pFunction)(int32_t), const CYIString &rButtonText)
    {
        CYIPushButtonView *pButton = ConnectButton(pView, rButtonName, eNodeFlag, rObject, pFunction);
        if (pButton != nullptr)
        {
            pButton->SetText(rButtonText);
            
            CYITextSceneNode *pHighlightText = GetNode<CYITextSceneNode>(pButton, "Text-Highlight", NODE_FLAG_OPTIONAL);
            if (pHighlightText != nullptr)
            {
                pHighlightText->SetText(rButtonText);
            }
        }
        return pButton;
    }
    
    template<typename Type>
    CYIPushButtonView *ConnectButton(CYISceneView &pView, const CYIString &rButtonName, GET_NODE_FLAG eNodeFlag, Type &object, void (Type::*pFunction)())
    {
        CYIPushButtonView *pButton = GetNode<CYIPushButtonView>(&pView, rButtonName, eNodeFlag);
        if (pButton != nullptr)
        {
            pButton->ButtonClicked.Connect(object, pFunction);
        }
        return pButton;
    }
    
    template<typename Type>
    CYIPushButtonView *ConnectButton(CYISceneView &pView, const CYIString &rButtonName, GET_NODE_FLAG eNodeFlag, Type &rObject, void (Type::*pFunction)(), const CYIString &rButtonText)
    {
        CYIPushButtonView *pButton = ConnectButton(pView, rButtonName, eNodeFlag, rObject, pFunction);
        if (pButton != nullptr)
        {
            pButton->SetText(rButtonText);
            
            CYITextSceneNode *pHighlightText = GetNode<CYITextSceneNode>(pButton, "Text-Highlight", NODE_FLAG_OPTIONAL);
            if (pHighlightText != nullptr)
            {
                pHighlightText->SetText(rButtonText);
            }
        }
        return pButton;
    }
    
    template<typename Type>
    std::tuple<CYISceneNode *, std::shared_ptr<CYIAssetViewTemplate>> GetParentAndTemplate(CYISceneNode *pAncestor, const CYIString &rName, bool bDeleteNode = true)
    {
        static_assert(std::is_base_of<CYISceneView, Type>::value, "Type is not a CYISceneView");
        
        auto *pView = Utility::GetNode<Type>(pAncestor, rName);
        auto parentAndTemplate = std::make_tuple(pView->GetParent(), pView->GetViewTemplate());
        
        if (bDeleteNode)
        {
            YiDeleteLater(std::move(pView->GetParent()->RemoveChild(pView)));
        }
        
        return parentAndTemplate;
    }
    
    template<typename ViewType>
    std::unique_ptr<ViewType> CreateViewFromTemplate(const std::shared_ptr<CYIAssetViewTemplate> &rTemplate, CYISceneManager *pSceneManager)
    {
        YI_ASSERT(rTemplate, TAG, "rTemplate is null; a valid template is needed to create a view");
        
        std::unique_ptr<ViewType> pView(new ViewType);
        
        if (!pView->BuildFromTemplate(pSceneManager, rTemplate))
        {
            YI_ASSERT(false, TAG, "Couldn't build view from template '%s'", rTemplate->GetName().GetData());
            YI_LOGE(TAG, "Couldn't build view from template '%s'", rTemplate->GetName().GetData());
            return std::unique_ptr<ViewType>();
        }
        
        // A workaround for US-7145.
        if (std::is_base_of<CYIScrollingView, ViewType>::value)
        {
            pView->SetSize(glm::vec3(1.0f, 1.0f, 0.1f));
        }
        
        if (!pView->Init())
        {
            YI_ASSERT(false, TAG, "Couldn't initialize view built from template '%s'", rTemplate->GetName().GetData());
            YI_LOGE(TAG, "Couldn't initialize view built from template '%s'", rTemplate->GetName().GetData());
            return std::unique_ptr<ViewType>();
        }
        
        return pView;
    }
    
    template<typename ViewType>
    std::unique_ptr<ViewType> CreateViewFromTemplateWithName(const CYIString &rTemplateName, CYISceneManager *pSceneManager)
    {
        YI_ASSERT(!rTemplateName.IsEmpty(), TAG, "rTemplateName is empty; a valid template name is needed to create a view");
        
        return CreateViewFromTemplate<ViewType>(CYIViewTemplate::GetViewTemplate(rTemplateName), pSceneManager);
    }
    
    /*!
     \brief Inserts child in parent, adjusting for reverse child order.
     */
    void InsertChild(CYISceneNode *pParent, std::unique_ptr<CYISceneNode> pChild, size_t uIndex);
    
    /*!
     \brief Inserts child in parent, adjusting for reverse child order.
     */
    void InsertChild(CYISceneView *pParent, std::unique_ptr<CYISceneNode> pChild, size_t uIndex);
    
    /*!
     \brief Appends child to end of parent's children, adjusting for reverse child order.
     */
    void AppendChild(CYISceneNode *pParent, std::unique_ptr<CYISceneNode> pChild);
    
    /*!
     \brief Appends child to end of parent's children, adjusting for reverse child order.
     */
    void AppendChild(CYISceneView *pParent, std::unique_ptr<CYISceneNode> pChild);
    
    /*!
     \brief Search through all descendants in a depth-first manner, running the given
     function taking `CYISceneNode &`.
     */
    template<typename Fn>
    void ForEachDescendant(CYISceneNode &rParent, Fn fn)
    {
        auto uCount = rParent.GetChildCount();
        for (decltype(uCount) i = 0; i < uCount; ++i)
        {
            bool bStop = false;
            auto *pChild = rParent.GetChild(i);
            fn(*pChild, bStop);
            if (!bStop)
            {
                ForEachDescendant(*pChild, fn);
            }
        }
    }
    
    inline void HideIfNotNull(CYISceneNode *pNode)
    {
        if (pNode != nullptr)
        {
            pNode->Hide();
        }
    }
}
#endif // _NODE_UTILITIES_H_

