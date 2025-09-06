#include "user/MauiControl.h"
#include "core/D3DRes.h"
#include <memory>

namespace Moho {

class CMauiItemList : public Moho::CMauiControl
{
public:
    std::auto_ptr<Moho::CD3DFont> mFont;
    Moho::color_t mForegroundColor;
    Moho::color_t mBackgroundColor;
    Moho::color_t mSelectedForegroundColor;
    Moho::color_t mSelectedBackgroundColor;
    Moho::color_t mHighlightForegroundColor;
    Moho::color_t mHighlightBackgroundColor;
    std::vector<std::string> mItems;
    int mCurSelection;
    int mHoverItem;
    bool mShowSelection;
    bool mShowMouseoverItem;
    int mScrollPosition;
};


}
