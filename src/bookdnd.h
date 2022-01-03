#ifndef BOOKON_BOOKDND_H
#define BOOKON_BOOKDND_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/dataobj.h>
#include <wx/dnd.h>
#include <wx/treectrl.h>
#include "bookmark.h"

static wxString BKDNDFormatId() {return "com.terranum.bookmark";}

class BKDNDDataObject : public wxDataObjectSimple {
 public:
  BKDNDDataObject(BookMark* bookmark, wxTreeItemId parent_tree_id);
  virtual ~BKDNDDataObject();

 private:
  wxTreeItemId m_bookmark_tree_parent;
  BookMark * m_bookmark = nullptr;
};


#endif  // BOOKON_BOOKDND_H
