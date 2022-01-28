#ifndef BOOKON_CONTOLMAIN_H
#define BOOKON_CONTOLMAIN_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "bookmark.h"
#include "bookmark.pb.h"
#include "wx/wx.h"
#endif

#include <wx/dataview.h>
#include <wx/treectrl.h>

#include "treedata.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Main control class for managing treectrl and listctrl
////////////////////////////////////////////////////////////////////////////////////////////////////
class ControlMain {
 public:
  ControlMain(wxTreeCtrl* tree, wxDataViewListCtrl* list);
  wxTreeItemId AddGroup(const wxString& group_name, bool is_root_child);
  void RemoveGroup();

  wxTreeItemId AddGroupItem(const wxString& string);
  void RemoveGroupItem();

  void BookMarkAdd();
  void BookMarkEdit();
  void BookMarkDel();

  // tree operations
  void OnBeginDrag(wxTreeEvent& event);
  void OnBeginDrop(wxTreeEvent& event);
  void OnEditLabelBegin(wxTreeEvent& event);
  void OnEditLabelEnd(wxTreeEvent& event);
  void OnDoubleClick(wxTreeEvent& event);

  // list operations
  void OnDoubleClickList(wxDataViewEvent& event);
  void OnRightClickMenu(wxDataViewEvent& event);
  void OnMenuEdit(wxCommandEvent& event);
  void OnMenuOpen(wxCommandEvent& event);
  void OnMenuCopy(wxCommandEvent& event);
  void OnMenuWeb(wxCommandEvent& event);
  void OnDragBookMarkStart(wxDataViewEvent& event);

  BKTreeItemData* GetItemData(const wxTreeItemId& id);
  BKTreeItemData* GetItemDataCopy(const wxTreeItemId& id);

  bool SaveFile(const wxString& pathname);
  void OpenFile(const wxString& pathname);

  void DropData(wxCoord x, wxCoord y, const int bookmark_index);
  bool IsDropPossible(wxCoord x, wxCoord y);

  bool IsProjectModified() const;
  void SetProjectModified(bool mProjectIsModified);

 private:
  wxTreeCtrl* m_tree;
  wxDataViewListCtrl* m_list;
  wxTreeItemId m_dragged_item;
  wxTreeItemId m_root;
  wxTreeItemId m_displayed_id;
  bool _move_tree_item(wxTreeItemId origin, wxTreeItemId destination);
  bool _is_folder(const wxTreeItemId& id);
  bool _is_item(const wxTreeItemId& id);
  bool _has_item_selected();
  void _display_bookmarks_for_item(const wxTreeItemId& my_sel_id);
  void _iterate_tree(const wxTreeItemId& id, bk::Folder* actual_folder);
  void _populate_tree(const wxTreeItemId idParent, const bk::Folder& folder);

  BookMark* _get_list_selected_bookmark();

  void _create_contextual_menu();
  wxMenu m_contextual_menu;
  wxMenuItem* m_menui_ctxt_edit;
  wxMenuItem* m_menui_ctxt_open;
  wxMenuItem* m_menui_ctxt_web;
  wxMenuItem* m_menui_ctxt_copy;

  bool m_project_is_modified = false;

 private:
  void _connect_event();
};

#endif  // BOOKON_CONTOLMAIN_H
