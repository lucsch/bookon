#include "framebookmark.h"

FrameBookmark::FrameBookmark(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos,
                             const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style) {
  _create_controls();
}

const BookMark& FrameBookmark::GetBookmark() const {
  return m_bookmark;
}

void FrameBookmark::SetBookmark(const BookMark& mBookmark) {
  m_bookmark = mBookmark;
}

bool FrameBookmark::TransferDataToWindow() {
  m_description_ctrl->SetValue(m_bookmark.m_description);
  m_path_ctrl->SetValue(m_bookmark.m_path);
  switch (m_bookmark.m_type) {
    case BKM_OPEN:
      m_opt_open_ctrl->SetValue(true);
      break;
    case BKM_COPY:
      m_opt_copy_ctrl->SetValue(true);
      break;
    case BKM_WEB:
      m_opt_web_ctrl->SetValue(true);
      break;
    default:
      wxLogError("Bookmark type not managed!");
  }
  return true;
}

bool FrameBookmark::TransferDataFromWindow() {
  m_bookmark.m_description = m_description_ctrl->GetValue();
  m_bookmark.m_path = m_path_ctrl->GetValue();
  m_bookmark.m_type = BKM_OPEN;
  if (m_opt_copy_ctrl->GetValue()){
    m_bookmark.m_type = BKM_COPY;
  }else if (m_opt_web_ctrl->GetValue()){
    m_bookmark.m_type = BKM_WEB;
  }
  return true;
}

void FrameBookmark::_create_controls() {
  SetSizeHints(wxDefaultSize, wxDefaultSize);

  wxBoxSizer* bSizer8;
  bSizer8 = new wxBoxSizer(wxVERTICAL);

  wxStaticBoxSizer* sbSizer1;
  sbSizer1 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("Description")), wxVERTICAL);

  m_description_ctrl =
      new wxTextCtrl(sbSizer1->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(300, -1), 0);
  sbSizer1->Add(m_description_ctrl, 0, wxALL | wxEXPAND, 5);

  bSizer8->Add(sbSizer1, 0, wxEXPAND | wxALL, 5);

  wxStaticBoxSizer* sbSizer2;
  sbSizer2 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("Path / bookmark")), wxVERTICAL);

  m_path_ctrl = new wxTextCtrl(sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
  sbSizer2->Add(m_path_ctrl, 0, wxALL | wxEXPAND, 5);

  wxBoxSizer* bSizer9;
  bSizer9 = new wxBoxSizer(wxHORIZONTAL);

  m_btn_paste_path =
      new wxButton(sbSizer2->GetStaticBox(), wxID_ANY, wxT("Paste path"), wxDefaultPosition, wxDefaultSize, 0);
  bSizer9->Add(m_btn_paste_path, 0, wxALL, 5);

  m_btn_paste_text =
      new wxButton(sbSizer2->GetStaticBox(), wxID_ANY, wxT("Paste text"), wxDefaultPosition, wxDefaultSize, 0);
  bSizer9->Add(m_btn_paste_text, 0, wxALL, 5);

  sbSizer2->Add(bSizer9, 0, wxEXPAND, 5);

  bSizer8->Add(sbSizer2, 1, wxEXPAND | wxALL, 5);

  wxStaticBoxSizer* sbSizer3;
  sbSizer3 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("Action")), wxVERTICAL);

  m_opt_open_ctrl =
      new wxRadioButton(sbSizer3->GetStaticBox(), wxID_ANY, wxT("Open"), wxDefaultPosition, wxDefaultSize, 0);
  sbSizer3->Add(m_opt_open_ctrl, 0, wxALL, 5);

  m_opt_copy_ctrl =
      new wxRadioButton(sbSizer3->GetStaticBox(), wxID_ANY, wxT("Copy"), wxDefaultPosition, wxDefaultSize, 0);
  sbSizer3->Add(m_opt_copy_ctrl, 0, wxALL, 5);

  m_opt_web_ctrl =
      new wxRadioButton(sbSizer3->GetStaticBox(), wxID_ANY, wxT("Web"), wxDefaultPosition, wxDefaultSize, 0);
  sbSizer3->Add(m_opt_web_ctrl, 0, wxALL, 5);

  bSizer8->Add(sbSizer3, 1, wxEXPAND | wxALL, 5);

  wxStdDialogButtonSizer* m_sdbSizer2;
  wxButton* m_sdbSizer2OK;
  wxButton* m_sdbSizer2Cancel;
  m_sdbSizer2 = new wxStdDialogButtonSizer();
  m_sdbSizer2OK = new wxButton(this, wxID_OK);
  m_sdbSizer2->AddButton(m_sdbSizer2OK);
  m_sdbSizer2Cancel = new wxButton(this, wxID_CANCEL);
  m_sdbSizer2->AddButton(m_sdbSizer2Cancel);
  m_sdbSizer2->Realize();

  bSizer8->Add(m_sdbSizer2, 0, wxEXPAND | wxALL, 5);

  SetSizer(bSizer8);
  Layout();
  bSizer8->Fit(this);
}
