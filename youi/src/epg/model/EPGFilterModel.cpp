#include "EPGFilterModel.h"

#include "../react/ProgrammingGuideProps.h"

#include <logging/YiLogger.h>
#include <utility/FollyDynamicUtilities.h>
#include <utility/InitFromValue.h>

#define LOG_TAG "EPGFilterModel"

EPGFilterModel::EPGFilterModel()
{}

bool EPGFilterModel::Init(const folly::dynamic &value)
{
    bool ok = true;

    ok = ok && InitFromMandatoryField(m_label, value, "label");
    ok = ok && InitFromMandatoryField(m_value, value, "match");

    //Filter type
    CYIString propName;

    ok = ok && InitFromMandatoryField(propName, value, "prop");

    if (propName == "isFavorite")
    {
        m_filterType = FilterType::Favorite;
    }
    else if (propName == "categories")
    {
        m_filterType = FilterType::Category;
    }

    return ok;
}
