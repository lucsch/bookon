#ifndef BOOKON_CONTOLMAIN_H
#define BOOKON_CONTOLMAIN_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/dataview.h>
#include <wx/treectrl.h>

class ControlMain {
 public:
  ControlMain(wxTreeCtrl* tree, wxDataViewListCtrl* list);
  void AddGroup(const wxString& group_name);
  void RemoveGroup();

  void AddGroupItem(const wxString& string);
  void RemoveGroupItem();

  void OnBeginDrag(wxTreeEvent& event);
  void OnBeginDrop(wxTreeEvent& event);

 private:
  wxTreeCtrl * m_tree;
  wxDataViewListCtrl* m_list;

  wxTreeItemId m_dragged_item;
  wxTreeItemId m_root;
  bool _move_tree_item(wxTreeItemId origin, wxTreeItemId destination);

};

#endif  // BOOKON_CONTOLMAIN_H
