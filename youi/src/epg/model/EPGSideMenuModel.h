#ifndef EPG_SIDE_MENU_MODEL_H
#define EPG_SIDE_MENU_MODEL_H

#include <folly/dynamic.h>

#include <utility/YiString.h>
#include <network/YiUrl.h>

#include <vector>

class EPGSideMenuModel
{
public:
    EPGSideMenuModel();
    virtual ~EPGSideMenuModel() = default;

    bool Init(const folly::dynamic &value);

private:

};

#endif
