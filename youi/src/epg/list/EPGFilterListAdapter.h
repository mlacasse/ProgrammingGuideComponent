#ifndef _EPG_FILTER_LIST_ADAPTER_H_
#define _EPG_FILTER_LIST_ADAPTER_H_

#include <import/YiViewTemplate.h>
#include <streamer/YiViewAdapter.h>

class EPGFilterModel;

class CYIListView;

struct EPGFilterListElement
{
    CYIString Text;
    bool ToggleState;
    
    EPGFilterListElement(const CYIString &text, bool toggleState)
        : Text(text)
        , ToggleState(toggleState)
    { }
};

class EPGFilterListAdapter : public CYIViewAdapter, public CYISignalHandler
{
public:
    EPGFilterListAdapter(CYIListView *pListView);
    virtual ~EPGFilterListAdapter() = default;

    std::shared_ptr<CYIAssetViewTemplate> GetViewTemplate(size_t index) const override;
    size_t GetItemsCount() const override;
    
    void SetFilters(std::vector<EPGFilterModel> &&filters);
    void BuildElements();
    void Clear();

    CYISignal<const EPGFilterModel &/*filterText*/> FilterEnabled;

protected:
    void PopulateView(size_t index, CYISceneView *pView) override;
    
private:
    void OnFilterButtonPressed(bool state, int32_t id);
    void SetButtonStateForIndex(int32_t index, bool state);
    
    CYIListView *m_pListView = nullptr;
    std::vector<EPGFilterModel> m_filters;
    int32_t m_selectedFilterIndex = 0;
};

#endif
