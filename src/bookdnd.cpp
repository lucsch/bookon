#include "bookdnd.h"

BKDNDDataObject::BKDNDDataObject(BookMark* bookmark, wxTreeItemId parent_tree_id)
    : wxDataObjectSimple(wxDataFormat(BKDNDFormatId())) {
  m_bookmark = bookmark;
  m_bookmark_tree_parent = parent_tree_id;
}

BKDNDDataObject::~BKDNDDataObject() {
  m_bookmark = nullptr;
  m_bookmark_tree_parent = wxTreeItemId();
}
