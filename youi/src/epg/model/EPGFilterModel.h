#ifndef EPG_FILTER_MODEL_H
#define EPG_FILTER_MODEL_H

#include <folly/dynamic.h>

#include <utility/YiString.h>

#include <vector>

class EPGFilterModel
{
public:
    enum class FilterType {
        None,
        Favorite,
        Category
    };

    EPGFilterModel();
    virtual ~EPGFilterModel() = default;

    bool Init(const folly::dynamic &value);

    const CYIString &GetLabel() const { return m_label; }
    FilterType GetFilterType() const { return m_filterType; }
    const CYIString &GetValue() const { return m_value; }
    bool IsEnabled() const { return m_isEnabled; }
    void SetIsEnabled(bool isEnabled) { m_isEnabled = isEnabled; }

private:
    CYIString m_label;
    FilterType m_filterType = FilterType::None;
    CYIString m_value;
    bool m_isEnabled = false;
};

#endif
