#include "bookmark.h"

BookMark::BookMark() {
  wxString m_description = wxEmptyString;
  wxString m_path = wxEmptyString;
  BookMarkType m_type = BKM_OPEN;
}

BookMark::BookMark(const wxString &description, const wxString &path) {
  m_description = description;
  m_path = path;
}

const wxVector<wxVariant> BookMark::GetBookMarkDataForList() const {
  wxVector<wxVariant> book_data;
  book_data.push_back(m_description);
  book_data.push_back(m_path);
  switch (m_type) {
    case BKM_OPEN:
      book_data.push_back(_T("Open"));
      break;
    case BKM_COPY:
      book_data.push_back(_T("Copy"));
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

void BookMark::SaveToProto(bk::Folder::Bookmark * pbk) {
  pbk->set_description(m_description);
  pbk->set_command(m_path);
  switch (m_type) {
    case BKM_OPEN:
      pbk->set_action(bk::Folder_Bookmark_Action_OPEN);
      break;
    case BKM_COPY:
      pbk->set_action(bk::Folder_Bookmark_Action_COPY);
      break;
    case BKM_WEB:
      pbk->set_action(bk::Folder_Bookmark_Action_WEB);
      break;
  }
}

void BookMark::LoadFromProto(const Folder::Bookmark &proto_book) {
  m_description = proto_book.description();
  m_path = proto_book.command();
  switch (proto_book.action()) {
    case bk::Folder_Bookmark_Action_OPEN:
      m_type = BKM_OPEN;
      break;
    case bk::Folder_Bookmark_Action_COPY:
      m_type = BKM_COPY;
      break;
    case bk::Folder_Bookmark_Action_WEB:
      m_type = BKM_WEB;
      break;
  }
}

void BookMark::DoAction() {
  if (m_path.IsEmpty()){
    wxLogError("No action defined for this bookmark!");
    return;
  }

  switch (m_type) {
    case BKM_OPEN:
      wxLaunchDefaultApplication(m_path);
      break;
    case BKM_WEB:
      wxLaunchDefaultBrowser(m_path);
      break;
    case BKM_COPY:
      if (wxTheClipboard->Open()) {
        wxTheClipboard->SetData(new wxTextDataObject(m_path));
        wxTheClipboard->Close();
      }
  }
}
