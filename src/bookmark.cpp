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