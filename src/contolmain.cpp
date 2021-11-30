
#include "contolmain.h"

ControlMain::ControlMain(wxDataViewTreeCtrl *tree, wxDataViewListCtrl *list) {
  m_tree = tree;
  m_list = list;

  m_tree->EnableDragSource(wxDF_UNICODETEXT);
  m_tree->EnableDropTarget(wxDF_UNICODETEXT);

  m_tree->Bind(wxEVT_DATAVIEW_ITEM_BEGIN_DRAG, &ControlMain::OnBeginDrag, this, m_tree->GetId());
  m_tree->Bind(wxEVT_DATAVIEW_ITEM_DROP, &ControlMain::OnBeginDrop, this, m_tree->GetId());
  m_tree->Bind(wxEVT_DATAVIEW_ITEM_DROP_POSSIBLE, &ControlMain::OnDropPossible, this, m_tree->GetId());
  wxLogDebug("Testing debug!");
}

void ControlMain::AddGroup(const wxString &group_name) {
  wxASSERT(m_tree);
  wxDataViewItem my_selected_item = m_tree->GetSelection();
  if (my_selected_item.IsOk() && m_tree->IsContainer(my_selected_item)) {
    m_tree->AppendContainer(my_selected_item, "Test");
    return;
  }
  wxDataViewItem root = m_tree->AppendContainer(wxDataViewItem(0), "The Root");
}

void ControlMain::RemoveGroup() {
  wxASSERT(m_tree);
  wxDataViewItem my_selected_item = m_tree->GetSelection();
  // cannot remove, not a container
  if (!m_tree->IsContainer(my_selected_item)) {
    wxLogError("Selected item isn't a group!");
    return;
  }

  // check if container not empty
  wxDataViewItemArray my_children;
  int number_child = m_tree->GetModel()->GetChildren(my_selected_item, my_children);
  if (number_child > 0) {
    wxLogError("Unable to remove entry, %d children!", number_child);
    return;
  }
  m_tree->DeleteItem(my_selected_item);
}

void ControlMain::AddGroupItem(const wxString &string) {
  wxASSERT(m_tree);
  wxDataViewItem my_selected_item = m_tree->GetSelection();
  if (!my_selected_item.IsOk()) {
    wxLogError("Please select a group entry first!");
    return;
  }

  // append item into a container
  if (m_tree->IsContainer(my_selected_item)) {
    m_tree->AppendItem(my_selected_item, string);
  }
  // append an item after the current item
  else {
    wxDataViewItem my_parent = m_tree->GetModel()->GetParent(my_selected_item);
    m_tree->AppendItem(my_parent, string);
  }
}

void ControlMain::RemoveGroupItem() {
  wxASSERT(m_tree);
  wxDataViewItem my_selected_item = m_tree->GetSelection();
  if (!my_selected_item.IsOk()) {
    return;
  }
  m_tree->DeleteItem(my_selected_item);
}

void ControlMain::OnBeginDrag(wxDataViewEvent &event) {
  m_dragged_item.Unset();
  if (!event.GetItem().IsOk()) {
    return;
  }
  m_dragged_item = event.GetItem();
  wxLogDebug("Begin drag %s", m_tree->GetItemText(event.GetItem()));

  wxTextDataObject *obj = new wxTextDataObject;
  obj->SetText( m_tree->GetItemText(event.GetItem()));
  event.SetDataObject( obj );
  event.SetDragFlags(wxDrag_CopyOnly); // allows both copy and move
}

void ControlMain::OnBeginDrop(wxDataViewEvent &event) {
  wxLogDebug("dropping");
  if (!m_dragged_item.IsOk() || !event.GetItem().IsOk()) {
    return;
  }
  wxLogDebug("Dropping on: %s", m_tree->GetItemText(event.GetItem()));
  wxTextDataObject obj;
  obj.SetData( wxDF_UNICODETEXT, event.GetDataSize(), event.GetDataBuffer() );

  if ( event.GetItem().IsOk() )
    wxLogDebug( "Text dropped on item %s: %s", m_tree->GetStore()->GetItemText(event.GetItem()), obj.GetText() );
  else
    wxLogDebug( "Text dropped on background: %s", obj.GetText() );

  _move_tree_item(m_dragged_item, event.GetItem());
  m_dragged_item.Unset();
}

void ControlMain::OnDropPossible(wxDataViewEvent &event) {
  wxLogDebug("Drop possible!");
  // event.GetDropEffect()

}

bool ControlMain::_move_tree_item(wxDataViewItem origin, wxDataViewItem destination) {
  // TODO: Add support for moving containers
  if (m_tree->IsContainer(origin)){
    wxLogError("Moving group not supported for now!");
    return false;
  }

  // moving items (dropping to containers)
  if (m_tree->IsContainer(destination)){
    wxLogDebug("item text: %s", m_tree->GetItemText(origin));
    wxDataViewItem my_new_item = m_tree->GetStore()->AppendItem(destination, m_tree->GetItemText(origin), m_tree->GetItemIcon(origin), m_tree->GetItemData(origin));
    wxDataViewItem my_origin_parent = m_tree->GetStore()->GetParent(origin);
     m_tree->DeleteItem(origin);
    m_tree->Collapse(destination);
    m_tree->Expand(destination);
    m_tree->Collapse(my_origin_parent);
    m_tree->Expand(my_origin_parent);
    m_tree->Select(my_new_item);
  }
  // moving items (dropping to item)
  else {
    wxDataViewItem my_new_item = m_tree->GetStore()->InsertItem(m_tree->GetStore()->GetParent(destination), destination, m_tree->GetItemText(origin), m_tree->GetItemIcon(origin), m_tree->GetItemData(origin));
    wxDataViewItem my_origin_parent = m_tree->GetStore()->GetParent(origin);
    wxDataViewItem my_dest_parent = m_tree->GetStore()->GetParent(destination);
    m_tree->DeleteItem(origin);
    m_tree->Collapse(my_dest_parent);
    m_tree->Expand(my_dest_parent);
    m_tree->Collapse(my_origin_parent);
    m_tree->Expand(my_origin_parent);
     m_tree->Select(my_new_item);
  }
  m_tree->Refresh();
  m_tree->Update();
  return true;
}
