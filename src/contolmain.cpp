
#include "contolmain.h"

ControlMain::ControlMain(wxDataViewTreeCtrl *tree, wxDataViewListCtrl *list) {
    m_tree = tree;
    m_list = list;
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
    if (!m_tree->IsContainer(my_selected_item)){
        wxLogError("Selected item isn't a group!");
        return;
    }

    // check if container not empty
    wxDataViewItemArray my_children;
    int number_child = m_tree->GetModel()->GetChildren(my_selected_item, my_children);
    if (number_child > 0){
        wxLogError("Unable to remove entry, %d children!", number_child);
        return;
    }
    m_tree->DeleteItem(my_selected_item);
}

void ControlMain::AddGroupItem(const wxString &string) {
    wxASSERT(m_tree);
    wxDataViewItem my_selected_item = m_tree->GetSelection();
    if (!my_selected_item.IsOk()){
        wxLogError("Please select a group entry first!");
        return;
    }

    // append item into a container
    if (m_tree->IsContainer(my_selected_item)){
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
    if (!my_selected_item.IsOk()){
        return;
    }
    m_tree->DeleteItem(my_selected_item);
}
