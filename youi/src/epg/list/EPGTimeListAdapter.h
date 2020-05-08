#ifndef _EPG_TIME_LIST_ADAPTER_H_
#define _EPG_TIME_LIST_ADAPTER_H_

#include <import/YiViewTemplate.h>
#include <streamer/YiViewAdapter.h>
#include <utility/YiDateTime.h>

class EPGAssetListAdapter;
class EPGListView;

struct EPGTimeListElement
{
    CYIString Text;
    glm::vec3 Size;
    
    EPGTimeListElement(const CYIString &text, glm::vec3 size)
        : Text(text)
        , Size(size)
    { }
};

class EPGTimeListAdapter : public CYIViewAdapter, public CYISignalHandler
{
public:
    EPGTimeListAdapter();
    virtual ~EPGTimeListAdapter() = default;

    std::shared_ptr<CYIAssetViewTemplate> GetViewTemplate(size_t index) const override;
    size_t GetItemsCount() const override;
    glm::vec3 GetInitialNodeSize(size_t index) const override;

    void BuildElements(const CYIDateTime &startTime, const CYIDateTime &endTime);
    void Clear();

protected:
    void PopulateView(size_t index, CYISceneView *pView) override;
    
private:
    void InsertElement(const CYIString &text, const float horizontalScaleFactor);
    float CalculateScaleForMinutes(uint64_t minutes) const;
    CYIString FormatTime(const CYIDateTime &time, bool appendMeridiem) const;
    
    std::vector<EPGTimeListElement> m_listElements;
    std::shared_ptr<IYIViewRecycler> m_pViewRecycler;
};

#endif
