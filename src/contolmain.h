#ifndef BOOKON_CONTOLMAIN_H
#define BOOKON_CONTOLMAIN_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "bookmark.h"
#include "wx/wx.h"
#include "bookmark.pb.h"
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

    BKTreeItemDataType GetType() const;
    void SetBookmarks(const wxVector<BookMark> &mBookmarks);
    wxVector<BookMark> &GetBookmarks() ;

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

    bool SaveFile(const wxString& pathname);
    void OpenFile(const wxString& pathname);

   private:
    wxTreeCtrl* m_tree;
    wxDataViewListCtrl* m_list;

    wxTreeItemId m_dragged_item;
    wxTreeItemId m_root;
    bool _move_tree_item(wxTreeItemId origin, wxTreeItemId destination);
    bool _is_folder(const wxTreeItemId & id);
    bool _is_item(const wxTreeItemId & id);
    bool _has_item_selected();
    void _display_bookmarks_for_item(const wxTreeItemId& my_sel_id);
    void _iterate_tree(const wxTreeItemId& idParent, bk::FolderList* folder_list, bk::Folder* actual_folder = nullptr,
                       wxTreeItemIdValue cookie = 0);
    void _iterate_tree_write(const wxTreeItemId idParent, const bk::Folder & folder);
};

#endif  // BOOKON_CONTOLMAIN_H
