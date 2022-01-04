#ifndef BOOKON_TREEDATA_H
#define BOOKON_TREEDATA_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/treectrl.h>
#include "bookmark.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Tree item data type
////////////////////////////////////////////////////////////////////////////////////////////////////
enum BKTreeItemDataType { BK_FOLDER = 0, BK_ITEM };

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Tree item data
////////////////////////////////////////////////////////////////////////////////////////////////////
class BKTreeItemData : public wxTreeItemData {
 public:
  BKTreeItemData(BKTreeItemDataType type);
  BKTreeItemData(const BKTreeItemData* data);

  BKTreeItemDataType GetType() const;
  void SetBookmarks(const wxVector<BookMark>& mBookmarks);
  wxVector<BookMark>& GetBookmarks();
  bool RemoveBookMarkAtIndex (int index, BookMark * removed_bookmark = nullptr);

 private:
  BKTreeItemDataType m_type;
  wxVector<BookMark> m_bookmarks;
};


#endif  // BOOKON_TREEDATA_H
