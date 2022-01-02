#ifndef BOOKON_BOOKMARK_H
#define BOOKON_BOOKMARK_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "bookmark.h"
#include "wx/wx.h"
#endif
#include "bookmark.pb.h"
#include <wx/clipbrd.h>
using namespace bk;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Store bookmark type
////////////////////////////////////////////////////////////////////////////////////////////////////
enum BookMarkType { BKM_OPEN, BKM_WEB, BKM_COPY };

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Store bookmark information
////////////////////////////////////////////////////////////////////////////////////////////////////
class BookMark {
 public:
  BookMark();
  BookMark(const wxString& description, const wxString& path);
  wxString m_description = wxEmptyString;
  wxString m_path = wxEmptyString;
  BookMarkType m_type = BKM_OPEN;
  const wxVector<wxVariant> GetBookMarkDataForList() const;

  // save and load protobuf data
  void SaveToProto(bk::Folder::Bookmark * pbk);
  void LoadFromProto (const bk::Folder::Bookmark & proto_book);

  // action
  void DoAction();

};
#endif // BOOKON_BOOKMARK_H
