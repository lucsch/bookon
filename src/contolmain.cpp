#include "contolmain.h"

#include <fstream>
#include <iostream>

#include "bookdnd.h"
#include "bookmark.pb.h"
#include "framebookmark.h"
using namespace bk;

ControlMain::ControlMain(wxTreeCtrl *tree, wxDataViewListCtrl *list) {
  m_tree = tree;
  m_list = list;

  _create_contextual_menu();
  _connect_event();

  m_tree->SetDropTarget(new BKDNDDropTarget(this));

  m_root = m_tree->AddRoot("Root");
  m_tree->SetBackgroundColour(m_list->GetBackgroundColour().GetAsString());

  // computing list width
  int my_width[] = {150, 320, 50};
  int my_list_width = m_list->GetSize().GetWidth();
  if (my_list_width - 50 > 0){
    my_width[0] = wxRound((my_list_width -50) / 3.0);
    my_width[1] = wxRound((my_list_width -50) / 3.0 * 2.0);
  }

  // init list columns
  m_list->AppendTextColumn(_("Description"), wxDATAVIEW_CELL_INERT, my_width[0]);
  m_list->AppendTextColumn(_("Path"), wxDATAVIEW_CELL_INERT,my_width[1]);
  m_list->AppendTextColumn(_("Type"), wxDATAVIEW_CELL_INERT, my_width[2]);
}

void ControlMain::_connect_event() {  // connecting tree events
  m_tree->Bind(wxEVT_TREE_BEGIN_DRAG, &ControlMain::OnBeginDrag, this, m_tree->GetId());
  m_tree->Bind(wxEVT_TREE_END_DRAG, &ControlMain::OnBeginDrop, this, m_tree->GetId());
  m_tree->Bind(wxEVT_TREE_BEGIN_LABEL_EDIT, &ControlMain::OnEditLabelBegin, this, m_tree->GetId());
  m_tree->Bind(wxEVT_TREE_END_LABEL_EDIT, &ControlMain::OnEditLabelEnd, this, m_tree->GetId());
  m_tree->Bind(wxEVT_TREE_ITEM_ACTIVATED, &ControlMain::OnDoubleClick, this, m_tree->GetId());

  // connecting list event
  m_list->Bind(wxEVT_DATAVIEW_ITEM_ACTIVATED, &ControlMain::OnDoubleClickList, this, m_list->GetId());
  m_list->Bind(wxEVT_DATAVIEW_ITEM_CONTEXT_MENU, &ControlMain::OnRightClickMenu, this, m_list->GetId());
  m_list->Bind(wxEVT_DATAVIEW_ITEM_BEGIN_DRAG, &ControlMain::OnDragBookMarkStart, this, m_list->GetId());

  // connecting contextual menu
  m_list->Bind(wxEVT_MENU, &ControlMain::OnMenuEdit, this, m_menui_ctxt_edit->GetId());
  m_list->Bind(wxEVT_MENU, &ControlMain::OnMenuCopy, this, m_menui_ctxt_copy->GetId());
  m_list->Bind(wxEVT_MENU, &ControlMain::OnMenuOpen, this, m_menui_ctxt_open->GetId());
  m_list->Bind(wxEVT_MENU, &ControlMain::OnMenuWeb, this, m_menui_ctxt_web->GetId());
}

wxTreeItemId ControlMain::AddGroup(const wxString &group_name, bool is_root_child) {
  wxASSERT(m_tree);
  SetProjectModified(true);

  // add a child to the root node
  if (is_root_child) {
    return m_tree->AppendItem(m_root, group_name, 0, -1, new BKTreeItemData(BK_FOLDER));
  }

  wxTreeItemId my_selected_item = m_tree->GetSelection();
  wxTreeItemId my_parent = m_root;
  // if an entry is selected :
  //  - add a child if the entry is a folder
  //  - add a sibling otherwise
  if (my_selected_item.IsOk() && my_selected_item != m_root) {
    if (_is_folder(my_selected_item)) {
      my_parent = my_selected_item;
    } else {
      my_parent = m_tree->GetItemParent(my_selected_item);
    }
  }
  wxTreeItemId my_added_group = m_tree->AppendItem(my_parent, group_name, 0, -1, new BKTreeItemData(BK_FOLDER));
  if (my_parent != m_root) {
    m_tree->Expand(my_parent);
  }
  return my_added_group;
}

void ControlMain::RemoveGroup() {
  wxASSERT(m_tree);
  wxTreeItemId my_selected_item = m_tree->GetSelection();
  // cannot remove, not a container
  if (!_is_folder(my_selected_item)) {
    wxLogError("Selected item isn't a group!");
    return;
  }

  // check if container not empty
  int number_child = m_tree->GetChildrenCount(my_selected_item);
  if (number_child > 0) {
    wxLogError("Unable to remove entry, %d children!", number_child);
    return;
  }
  m_tree->Delete(my_selected_item);
  SetProjectModified(true);
}

wxTreeItemId ControlMain::AddGroupItem(const wxString &string) {
  wxASSERT(m_tree);
  wxTreeItemId my_selected_item = m_tree->GetSelection();
  if (!my_selected_item.IsOk()) {
    wxLogError("Please select a group entry first!");
    return {};
  }

  wxTreeItemId my_insert_pos;
  if (my_selected_item == m_root) {
    my_insert_pos = m_root;
  } else {
    // append after the item because item isn't a folder
    if (!_is_folder(my_selected_item)) {
      my_insert_pos = m_tree->GetItemParent(my_selected_item);
    }
    // append into the item, because item is a folder
    else {
      my_insert_pos = my_selected_item;
    }
  }
  wxTreeItemId my_added_item = m_tree->AppendItem(my_insert_pos, string, 1, -1, new BKTreeItemData(BK_ITEM));
  if (my_insert_pos != m_root) {
    m_tree->Expand(my_insert_pos);
  }
  SetProjectModified(true);
  return my_added_item;
}

void ControlMain::RemoveGroupItem() {
  wxASSERT(m_tree);
  wxTreeItemId my_selected_item = m_tree->GetSelection();
  if (!my_selected_item.IsOk()) {
    wxLogError("Nothing selected!");
    return;
  }
  if (!_is_item(my_selected_item)) {
    wxLogError("Selected object isn't a item!");
    return;
  }
  SetProjectModified(true);
  m_tree->Delete(my_selected_item);
}

void ControlMain::OnBeginDrag(wxTreeEvent &event) {
  m_dragged_item.Unset();
  if (!event.GetItem().IsOk()) {
    event.Veto();
    return;
  }

  if (_is_folder(event.GetItem())) {
    wxLogWarning("Dragging folder isn't supported!");
    event.Veto();
    return;
  }

  m_dragged_item = event.GetItem();
  wxLogDebug("Begin drag %s", m_tree->GetItemText(event.GetItem()));
  event.Allow();
}

void ControlMain::OnBeginDrop(wxTreeEvent &event) {
  wxTreeItemId origin = m_dragged_item;
  wxTreeItemId destination = event.GetItem();

  wxLogDebug("Dropping");

  if (!origin.IsOk()) {
    event.Veto();
    return;
  }

  if (!destination.IsOk()) {
    event.Veto();
    return;
  }

  wxLogDebug("Dropping on: %s", m_tree->GetItemText(destination));
  _move_tree_item(m_dragged_item, event.GetItem());
  m_dragged_item.Unset();
  SetProjectModified(true);
}

bool ControlMain::_move_tree_item(wxTreeItemId origin, wxTreeItemId destination) {
  // TODO: Add support for moving containers
  wxASSERT(!_is_folder(origin));

  // moving items (dropping to containers)
  wxTreeItemId my_new_id;
  if (_is_folder(destination)) {
    my_new_id = m_tree->AppendItem(destination, m_tree->GetItemText(origin), -1, -1, GetItemDataCopy(origin));
    m_tree->Expand(destination);
  }
  // moving items (dropping to item)
  else {
    my_new_id = m_tree->InsertItem(m_tree->GetItemParent(destination), destination, m_tree->GetItemText(origin), -1, -1,
                                   GetItemDataCopy(origin));
  }
  m_tree->SelectItem(my_new_id);
  m_tree->Delete(origin);
  SetProjectModified(true);
  return true;
}

bool ControlMain::_is_folder(const wxTreeItemId &id) {
  if (GetItemData(id) == nullptr) {
    return false;
  }
  if (GetItemData(id)->GetType() != BK_FOLDER) {
    return false;
  }
  return true;
}

bool ControlMain::_is_item(const wxTreeItemId &id) {
  if (GetItemData(id) == nullptr) {
    return false;
  }
  if (GetItemData(id)->GetType() == BK_ITEM) {
    return true;
  }
  return false;
}

BKTreeItemData *ControlMain::GetItemData(const wxTreeItemId &id) {
  return dynamic_cast<BKTreeItemData *>(m_tree->GetItemData(id));
}

BKTreeItemData *ControlMain::GetItemDataCopy(const wxTreeItemId &id) {
  auto *my_data = new BKTreeItemData((BKTreeItemData *)m_tree->GetItemData(id));
  return my_data;
}

void ControlMain::OnEditLabelBegin(wxTreeEvent &event) {
  wxLogDebug("Edit label");
  event.Allow();
}

void ControlMain::OnEditLabelEnd(wxTreeEvent &event) {
  if (event.GetLabel().IsEmpty()) {
    wxLogError("Empty name isn't allowed!");
    event.Veto();
  }
  SetProjectModified(true);
}

void ControlMain::OnDoubleClick(wxTreeEvent &event) {
  wxTreeItemId my_sel_id(event.GetItem());
  if (!_is_item(my_sel_id)) {
    event.Veto();
    return;
  }
  _display_bookmarks_for_item(my_sel_id);
}

void ControlMain::_display_bookmarks_for_item(const wxTreeItemId &my_sel_id) {
  BKTreeItemData *my_data = GetItemData(my_sel_id);
  if (!my_data) {
    return;
  }
  m_displayed_id = my_sel_id;
  wxVector<BookMark> bookmarks = my_data->GetBookmarks();
  m_list->DeleteAllItems();
  for (wxVector<BookMark>::iterator iter = bookmarks.begin(); iter != bookmarks.end(); ++iter) {
    int my_index = iter - bookmarks.begin();
    m_list->AppendItem(iter->GetBookMarkDataForList(), my_index);
  }
}

void ControlMain::BookMarkEdit() {
  int my_list_sel_index = m_list->GetSelectedRow();
  if (my_list_sel_index == wxNOT_FOUND) {
    wxLogWarning("Please select an item first!");
    return;
  }

  int my_bk_index = (int)m_list->GetItemData(m_list->GetSelection());
  BKTreeItemData *my_data = GetItemData(m_displayed_id);
  if (!my_data) {
    return;
  }
  BookMark bk = my_data->GetBookmarks()[my_bk_index];
  FrameBookmark myDlg(m_list);
  myDlg.SetBookmark(bk);
  if (myDlg.ShowModal() != wxID_OK) {
    return;
  }
  my_data->GetBookmarks()[my_bk_index] = myDlg.GetBookmark();
  _display_bookmarks_for_item(m_displayed_id);
  SetProjectModified(true);
}

void ControlMain::BookMarkAdd() {
  if (!_has_item_selected()) {
    wxLogError(_("Select an item first!"));
    return;
  }

  FrameBookmark myDlg(m_list);
  if (myDlg.ShowModal() != wxID_OK) {
    return;
  }

  wxTreeItemId my_selection(m_tree->GetSelection());
  BookMark book = myDlg.GetBookmark();
  BKTreeItemData *my_data = GetItemData(my_selection);
  wxASSERT(my_data);
  my_data->GetBookmarks().push_back(book);

  _display_bookmarks_for_item(my_selection);
  SetProjectModified(true);
}

void ControlMain::BookMarkDel() {
  int my_list_sel_index = m_list->GetSelectedRow();
  if (my_list_sel_index == wxNOT_FOUND) {
    wxLogWarning("Please select an item first!");
    return;
  }

  int my_bk_index = (int)m_list->GetItemData(m_list->GetSelection());
  BKTreeItemData *my_data = GetItemData(m_tree->GetSelection());
  if (!my_data) {
    return;
  }
  wxVector<BookMark>::iterator iter = my_data->GetBookmarks().begin();
  my_data->GetBookmarks().erase(iter + my_bk_index);
  _display_bookmarks_for_item(m_tree->GetSelection());
  SetProjectModified(true);
}

bool ControlMain::_has_item_selected() {
  wxTreeItemId id(m_tree->GetSelection());
  if (!id.IsOk()) {
    return false;
  }
  if (!_is_item(id)) {
    return false;
  }
  return true;
}

bool ControlMain::SaveFile(const wxString &pathname) {
  // protobuf structure
  FolderList my_list;

  // iterate first level tree ctrl
  wxTreeItemId root = m_tree->GetRootItem();
  wxASSERT(root.IsOk());
  wxTreeItemId id;
  wxTreeItemIdValue cookie = nullptr;
  for (int i = 0; i < m_tree->GetChildrenCount(root, false); i++) {
    if (!cookie) {
      id = m_tree->GetFirstChild(root, cookie);
    } else {
      id = m_tree->GetNextChild(root, cookie);
    }
    wxASSERT(id.IsOk());
    _iterate_tree(id, my_list.add_folders());
  }

  std::fstream output(pathname.ToStdString(), std::ios::out | std::ios::trunc | std::ios::binary);
  my_list.SerializeToOstream(&output);
  SetProjectModified(false);
  return true;
}

void ControlMain::_iterate_tree(const wxTreeItemId &id, bk::Folder *actual_folder) {
  wxString my_text = m_tree->GetItemText(id);
  BKTreeItemData *my_data = GetItemData(id);
  wxASSERT(my_data);
  if (my_data->GetType() == BK_FOLDER) {
    actual_folder->set_name(my_text);
    actual_folder->set_is_group(true);
  } else {
    actual_folder->set_is_group(false);
    actual_folder->set_name(my_text);
    wxVector<BookMark> my_books = my_data->GetBookmarks();
    for (wxVector<BookMark>::iterator iter = my_books.begin(); iter != my_books.end(); ++iter) {
      Folder::Bookmark *my_pbk = actual_folder->add_bookmarks();
      iter->SaveToProto(my_pbk);
    }
  }

  if (m_tree->ItemHasChildren(id)) {
    wxTreeItemId child_id;
    wxTreeItemIdValue child_cookie = 0;
    for (int c = 0; c < m_tree->GetChildrenCount(id, false); c++) {
      if (!child_cookie) {
        child_id = m_tree->GetFirstChild(id, child_cookie);
      } else {
        child_id = m_tree->GetNextChild(id, child_cookie);
      }
      wxASSERT(child_id.IsOk());
      _iterate_tree(child_id, actual_folder->add_folders());
    }
  }
}

void ControlMain::OpenFile(const wxString &pathname) {
  // read the data
  FolderList my_list;
  std::fstream input(pathname.ToStdString(), std::ios::in | std::ios::binary);
  if (!my_list.ParseFromIstream(&input)) {
    wxLogError("Error reading: '%s'", pathname);
    return;
  }

  // iterate the data and populate the treectrl
  m_tree->DeleteAllItems();
  wxTreeItemId rootid = m_tree->AddRoot("Root");
  for (int i = 0; i < my_list.folders_size(); i++) {
    Folder my_folder = my_list.folders(i);
    _populate_tree(rootid, my_folder);
  }

  m_tree->ExpandAll();
}

void ControlMain::_populate_tree(const wxTreeItemId idParent, const Folder &folder) {
  if (folder.is_group()) {
    wxTreeItemId id = m_tree->AppendItem(idParent, folder.name(), 0, -1, new BKTreeItemData(BK_FOLDER));
    // re-entring method for adding folders
    for (int f = 0; f < folder.folders_size(); f++) {
      _populate_tree(id, folder.folders(f));
    }
  } else {
    wxVector<BookMark> my_bookmarks;
    for (int b = 0; b < folder.bookmarks_size(); b++) {
      BookMark my_book;
      my_book.LoadFromProto(folder.bookmarks(b));
      my_bookmarks.push_back(my_book);
    }
    auto *ptreedata = new BKTreeItemData(BK_ITEM);
    ptreedata->SetBookmarks(my_bookmarks);
    m_tree->AppendItem(idParent, folder.name(), 1, -1, ptreedata);
  }
}

void ControlMain::OnDoubleClickList(wxDataViewEvent &event) {
  if (!m_displayed_id.IsOk()) {
    wxLogError("Something is wrong with the selected tree's node!");
    return;
  }

  BKTreeItemData *my_data = GetItemData(m_displayed_id);
  if (!my_data) {
    return;
  }
  int my_index = m_list->GetItemData(event.GetItem());
  wxVector<BookMark> bookmarks = my_data->GetBookmarks();
  wxASSERT(my_index >= 0 && my_index < bookmarks.size());
  wxLogDebug("Selected bookmark is: %s", bookmarks[my_index].m_description);
  bookmarks[my_index].DoAction();
}

void ControlMain::OnRightClickMenu(wxDataViewEvent &event) {
  if (!event.GetItem().IsOk()) {
    event.Veto();
    return;
  }
  m_list->PopupMenu(&m_contextual_menu, event.GetPosition());
}

void ControlMain::_create_contextual_menu() {
  m_menui_ctxt_edit = m_contextual_menu.Append(wxID_ANY, _("Edit..."));
  m_contextual_menu.AppendSeparator();
  m_menui_ctxt_open = m_contextual_menu.Append(wxID_ANY, _("Open"));
  m_menui_ctxt_web = m_contextual_menu.Append(wxID_ANY, _("Web"));
  m_menui_ctxt_copy = m_contextual_menu.Append(wxID_ANY, _("Copy"));
}

void ControlMain::OnMenuEdit(wxCommandEvent &event) {
  BookMarkEdit();
}

void ControlMain::OnMenuOpen(wxCommandEvent &event) {
  BookMark *my_book = _get_list_selected_bookmark();
  if (my_book == nullptr) {
    return;
  }
  BookMark bk_copy = *my_book;
  bk_copy.m_type = BKM_OPEN;
  bk_copy.DoAction();
}

void ControlMain::OnMenuCopy(wxCommandEvent &event) {
  BookMark *my_book = _get_list_selected_bookmark();
  if (my_book == nullptr) {
    return;
  }
  BookMark bk_copy = *my_book;
  bk_copy.m_type = BKM_COPY;
  bk_copy.DoAction();
}

void ControlMain::OnMenuWeb(wxCommandEvent &event) {
  BookMark *my_book = _get_list_selected_bookmark();
  if (my_book == nullptr) {
    return;
  }
  BookMark bk_copy = *my_book;
  bk_copy.m_type = BKM_WEB;
  bk_copy.DoAction();
}

BookMark *ControlMain::_get_list_selected_bookmark() {
  wxASSERT(m_list);
  wxDataViewItem my_selected_item = m_list->GetSelection();
  if (!my_selected_item.IsOk()) {
    return nullptr;
  }

  if (!m_displayed_id.IsOk()) {
    return nullptr;
  }

  BKTreeItemData *my_data = GetItemData(m_displayed_id);
  if (my_data == nullptr) {
    return nullptr;
  }

  return &my_data->GetBookmarks()[m_list->ItemToRow(my_selected_item)];
}

void ControlMain::OnDragBookMarkStart(wxDataViewEvent &event) {
  int my_book_index = m_list->GetSelectedRow();
  wxCustomDataObject my_data;
  my_data.SetFormat(wxDataFormat(BKDNDFormatId()));
  my_data.SetData(sizeof(int), &my_book_index);

  // BKDNDDataObject my_data( my_book_index);
  wxDropSource drag_source(m_list);
  drag_source.SetData(my_data);
  wxDragResult result = drag_source.DoDragDrop(true);
  switch (result) {
    case wxDragCopy:
      wxLogDebug("Data copied!");
      break;
    case wxDragMove:
      wxLogDebug("Data moved");
      break;
    default:
      wxLogDebug("Nothing done!");
      break;
  }
  event.Veto();
}

void ControlMain::DropData(wxCoord x, wxCoord y, const int bookmark_index) {
  wxLogDebug("Bookmark dropped: %d", bookmark_index);
  wxTreeItemId my_hitted_id = m_tree->HitTest(wxPoint(x, y));
  if (!_is_item(my_hitted_id) || my_hitted_id == m_displayed_id) {
    return;
  }

  if (bookmark_index == wxNOT_FOUND) {
    wxLogDebug("Bookmark index = -1");
    return;
  }

  // move the item from one tree data to the other
  BKTreeItemData *my_origin_data = GetItemData(m_displayed_id);
  BookMark my_moved_book;
  if (!my_origin_data->RemoveBookMarkAtIndex(bookmark_index, &my_moved_book)) {
    return;
  }

  BKTreeItemData *my_dest_data = GetItemData(my_hitted_id);
  my_dest_data->GetBookmarks().push_back(my_moved_book);

  _display_bookmarks_for_item(m_displayed_id);
  SetProjectModified(true);
}

bool ControlMain::IsDropPossible(wxCoord x, wxCoord y) {
  wxTreeItemId my_hitted_id = m_tree->HitTest(wxPoint(x, y));
  // dropping is not on an item!
  if (!my_hitted_id.IsOk()) {
    return false;
  }

  // dropping on the displayed item... nothing move
  if (my_hitted_id == m_displayed_id) {
    return false;
  }

  // dropping on a folder ... not possible
  if (_is_folder(my_hitted_id)) {
    return false;
  }
  return true;
}

bool ControlMain::IsProjectModified() const {
    return m_project_is_modified;
}

void ControlMain::SetProjectModified(bool mProjectIsModified) {
  m_project_is_modified = mProjectIsModified;
}
