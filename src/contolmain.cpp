
#include "contolmain.h"

ControlMain::ControlMain(wxDataViewTreeCtrl *tree, wxDataViewListCtrl *list) {
  m_tree = tree;
  m_list = list;
}

bool ControlMain::AddTreeItem(wxDataViewItem *root, const wxString &text) {
  wxASSERT(m_tree);

  return false;
}

void ControlMain::AddGroup(const wxString &group_name) {
  wxASSERT(m_tree);
  wxDataViewItem my_selected_item = m_tree->GetSelection();
  if (my_selected_item.IsOk()) {
    m_tree->AppendContainer(my_selected_item, "Test");
    return;
  }
  wxDataViewItem root = m_tree->AppendContainer(wxDataViewItem(0), "The Root");
}
