#ifndef BOOKON_CONTOLMAIN_H
#define BOOKON_CONTOLMAIN_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/dataview.h>

class ControlMain {
 public:
  ControlMain(wxDataViewTreeCtrl * tree, wxDataViewListCtrl * list);

  bool AddTreeItem(wxDataViewItem * root, const wxString & text);

  void AddGroup(const wxString& group_name);

 private:
  wxDataViewTreeCtrl * m_tree;
  wxDataViewListCtrl * m_list;
};


#endif  // BOOKON_CONTOLMAIN_H
