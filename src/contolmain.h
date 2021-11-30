#ifndef BOOKON_CONTOLMAIN_H
#define BOOKON_CONTOLMAIN_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/dataview.h>
#include <wx/treectrl.h>

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
  BKTreeItemData(const BKTreeItemData * data);

 private:
  BKTreeItemDataType m_type;

 public:
  BKTreeItemDataType GetType() const;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Main control class for managing treectrl and listctrl
////////////////////////////////////////////////////////////////////////////////////////////////////
class ControlMain {
 public:
  ControlMain(wxTreeCtrl* tree, wxDataViewListCtrl* list);
  void AddGroup(const wxString& group_name);
  void RemoveGroup();

  void AddGroupItem(const wxString& string);
  void RemoveGroupItem();

  void OnBeginDrag(wxTreeEvent& event);
  void OnBeginDrop(wxTreeEvent& event);
  void OnEditLabelBegin(wxTreeEvent & event);
  void OnEditLabelEnd(wxTreeEvent & event);

  BKTreeItemData * GetItemData (const wxTreeItemId& id);
  BKTreeItemData * GetItemDataCopy (const wxTreeItemId& id);

 private:
  wxTreeCtrl* m_tree;
  wxDataViewListCtrl* m_list;

  wxTreeItemId m_dragged_item;
  wxTreeItemId m_root;
  bool _move_tree_item(wxTreeItemId origin, wxTreeItemId destination);
  bool _is_folder(const wxTreeItemId & id);
  bool _is_item(const wxTreeItemId & id);
};

#endif  // BOOKON_CONTOLMAIN_H
