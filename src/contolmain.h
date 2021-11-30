#ifndef BOOKON_CONTOLMAIN_H
#define BOOKON_CONTOLMAIN_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/dataview.h>

class ControlMain {
 public:
  ControlMain(wxDataViewTreeCtrl* tree, wxDataViewListCtrl* list);
  void AddGroup(const wxString& group_name);
  void RemoveGroup();

  void AddGroupItem(const wxString& string);
  void RemoveGroupItem();

  void OnBeginDrag(wxDataViewEvent& event);
  void OnBeginDrop(wxDataViewEvent& event);
  void OnDropPossible(wxDataViewEvent& event);

 private:
  wxDataViewTreeCtrl* m_tree;
  wxDataViewListCtrl* m_list;

  wxDataViewItem m_dragged_item;
  bool _move_tree_item(wxDataViewItem origin, wxDataViewItem destination);

};

#endif  // BOOKON_CONTOLMAIN_H
