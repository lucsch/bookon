#include "treedata.h"

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
  m_bookmarks.clear();
  for (wxVector<BookMark>::const_iterator iter = data->m_bookmarks.begin(); iter != data->m_bookmarks.end(); ++iter) {
    m_bookmarks.push_back(*iter);
  }
}

wxVector<BookMark> &BKTreeItemData::GetBookmarks() {
  return m_bookmarks;
}

void BKTreeItemData::SetBookmarks(const wxVector<BookMark> &mBookmarks) {
  m_bookmarks = mBookmarks;
}

bool BKTreeItemData::RemoveBookMarkAtIndex(int index, BookMark *removed_bookmark) {
  if (index >= m_bookmarks.size()) {
    wxLogError("Index :%d is out of bound!", index);
    return false;
  }

  if (removed_bookmark != nullptr) {
    *removed_bookmark = m_bookmarks[index];
  }

  wxVector<BookMark>::iterator iter = m_bookmarks.begin();
  m_bookmarks.erase(iter + index);
  return true;
}



