#ifndef BOOKON_CONTOLMAIN_H
#define BOOKON_CONTOLMAIN_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/dataview.h>
#include <wx/treectrl.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Store bookmark type
////////////////////////////////////////////////////////////////////////////////////////////////////
enum BookMarkType {BKM_OPEN, BKM_COPY, BKM_WEB};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Store bookmark information
////////////////////////////////////////////////////////////////////////////////////////////////////
class BookMark {
public:
    BookMark();
    BookMark(const wxString & description, const wxString & path);
    wxString m_description = wxEmptyString;
    wxString m_path = wxEmptyString;
    BookMarkType m_type = BKM_OPEN;
    const wxVector<wxVariant>  GetBookMarkData() const;
};


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

    BKTreeItemDataType GetType() const;
    void SetBookmarks(const wxVector<BookMark> &mBookmarks);
    const wxVector<BookMark> &GetBookmarks() const;

private:
    BKTreeItemDataType m_type;
    wxVector<BookMark> m_bookmarks;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Main control class for managing treectrl and listctrl
////////////////////////////////////////////////////////////////////////////////////////////////////
class ControlMain {
public:
    ControlMain(wxTreeCtrl* tree, wxDataViewListCtrl* list);
    wxTreeItemId AddGroup(const wxString& group_name);
    void RemoveGroup();

    wxTreeItemId AddGroupItem(const wxString& string);
    void RemoveGroupItem();

    void BookMarkAdd();
    void BookMarkEdit();
    void BookMarkDel();

    void OnBeginDrag(wxTreeEvent& event);
    void OnBeginDrop(wxTreeEvent& event);
    void OnEditLabelBegin(wxTreeEvent & event);
    void OnEditLabelEnd(wxTreeEvent & event);
    void OnDoubleClick(wxTreeEvent & event);

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
    bool _has_item_selected();
};

#endif  // BOOKON_CONTOLMAIN_H
