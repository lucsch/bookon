
#include "contolmain.h"

BKTreeItemData::BKTreeItemData(BKTreeItemDataType type) {
    m_type = type;
}
BKTreeItemDataType BKTreeItemData::GetType() const {
    return m_type;
}

BKTreeItemData::BKTreeItemData(const BKTreeItemData *data) {
    if (data == nullptr) {
        wxLogDebug("BKTreeItemData Initialisation failed!");
        return;
    }
    m_type = data->GetType();
}

const wxVector<BookMark> &BKTreeItemData::GetBookmarks() const {
    return m_bookmarks;
}

void BKTreeItemData::SetBookmarks(const wxVector<BookMark> &mBookmarks) {
    m_bookmarks = mBookmarks;
}

BKTreeItemData *ControlMain::GetItemData(const wxTreeItemId &id) {
    return (BKTreeItemData *)m_tree->GetItemData(id);
}

ControlMain::ControlMain(wxTreeCtrl *tree, wxDataViewListCtrl *list) {
    m_tree = tree;
    m_list = list;

    // connecting tree events
    m_tree->Bind(wxEVT_TREE_BEGIN_DRAG, &ControlMain::OnBeginDrag, this, m_tree->GetId());
    m_tree->Bind(wxEVT_TREE_END_DRAG, &ControlMain::OnBeginDrop, this, m_tree->GetId());
    m_tree->Bind(wxEVT_TREE_BEGIN_LABEL_EDIT, &ControlMain::OnEditLabelBegin, this, m_tree->GetId());
    m_tree->Bind(wxEVT_TREE_END_LABEL_EDIT, &ControlMain::OnEditLabelEnd, this, m_tree->GetId());
    m_tree->Bind(wxEVT_TREE_ITEM_ACTIVATED, &ControlMain::OnDoubleClick, this, m_tree->GetId());

    m_root = m_tree->AddRoot("Root");
    m_tree->SetBackgroundColour(m_list->GetBackgroundColour().GetAsString());

    // init list columns
    m_list->AppendTextColumn(_("Description"));
    m_list->AppendTextColumn(_("Path"), wxDATAVIEW_CELL_INERT, 400);
    m_list->AppendTextColumn(_("Type"));

    // TODO: Remove this test code
    wxTreeItemId my_group = AddGroup("Test_group");
    m_tree->SelectItem(my_group);
    wxTreeItemId my_item = AddGroupItem("Test_item");
    m_tree->SelectItem(my_item);
    wxVector<BookMark> bookmarks;
    for (int i = 0; i< 3; i++) {
        BookMark book(wxString::Format("Bookmark-%d", i), wxEmptyString);
        bookmarks.push_back(book);
    }
    BKTreeItemData * my_data = (BKTreeItemData*) m_tree->GetItemData(my_item);
    wxASSERT(my_data);
    my_data->SetBookmarks(bookmarks);
}

wxTreeItemId ControlMain::AddGroup(const wxString &group_name) {
    wxASSERT(m_tree);
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
    wxTreeItemId my_added_group = m_tree->AppendItem(my_parent, group_name, -1, -1, new BKTreeItemData(BK_FOLDER));
    if (my_parent != m_root) {
        m_tree->Expand(my_parent);
    }
    return  my_added_group;
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
}

wxTreeItemId ControlMain::AddGroupItem(const wxString &string) {
    wxASSERT(m_tree);
    wxTreeItemId my_selected_item = m_tree->GetSelection();
    if (!my_selected_item.IsOk()) {
        wxLogError("Please select a group entry first!");
        return wxTreeItemId();
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
    wxTreeItemId my_added_item = m_tree->AppendItem(my_insert_pos, string, -1, -1, new BKTreeItemData(BK_ITEM));
    if (my_insert_pos != m_root) {
        m_tree->Expand(my_insert_pos);
    }
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

    //  wxTextDataObject *obj = new wxTextDataObject;
    //  obj->SetText( m_tree->GetItemText(event.GetItem()));
    //  event.SetDataObject( obj );
    //  event.SetDragFlags(wxDrag_CopyOnly); // allows both copy and move
}

void ControlMain::OnBeginDrop(wxTreeEvent &event) {
    wxTreeItemId origin = m_dragged_item;
    wxTreeItemId destination = event.GetItem();

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

BKTreeItemData *ControlMain::GetItemDataCopy(const wxTreeItemId &id) {
    BKTreeItemData *my_data = new BKTreeItemData((BKTreeItemData *)m_tree->GetItemData(id));
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
}

void ControlMain::OnDoubleClick(wxTreeEvent &event) {
    wxTreeItemId my_sel_id (event.GetItem());
    if (!_is_item(my_sel_id)){
        event.Veto();
        return;
    }
    wxLogDebug("Double-click");
    BKTreeItemData * my_data = (BKTreeItemData*) m_tree->GetItemData(my_sel_id);
    if (!my_data) {
        return;
    }

    wxVector<BookMark> bookmarks = my_data->GetBookmarks();
    m_list->DeleteAllItems();
    for (wxVector<BookMark>::iterator iter = bookmarks.begin(); iter != bookmarks.end(); ++iter){
//        wxVector<wxVariant> data;
//        data.push_back("Test");
//        data.push_back("Test1");
//        data.push_back("Test2");
        m_list->AppendItem(iter->GetBookMarkData());
        // m_list->AppendItem(data);
    }
}

BookMark::BookMark() {
    wxString m_description = wxEmptyString;
    wxString m_path = wxEmptyString;
    BookMarkType m_type = BKM_OPEN;
}

BookMark::BookMark(const wxString &description, const wxString &path) {
    m_description = description;
    m_path = path;
}

const wxVector<wxVariant> BookMark::GetBookMarkData() const {
    wxVector<wxVariant> book_data;
    book_data.push_back(m_description);
    book_data.push_back(m_path);
    switch (m_type) {
        case BKM_OPEN:
            book_data.push_back(_T("Open"));
            break;
        case BKM_COPY:
            book_data.push_back(_T("Copy to clipboard"));
            break;
        case BKM_WEB:
            book_data.push_back(_T("Web"));
            break;
        default:
            book_data.push_back(wxEmptyString);
            break;
    }
    return book_data;
}
