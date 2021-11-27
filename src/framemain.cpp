#include "framemain.h"

#include "bitmap.h"
#include "frameabout.h"

extern const char *bookon_MAJOR_VERSION;
extern const char *bookon_MINOR_VERSION;
extern const char *GIT_REV;
extern const char *GIT_TAG;
extern const char *GIT_BRANCH;
extern const char *GIT_NUMBER;

FrameMain::FrameMain(const wxString &title) : wxFrame(NULL, wxID_ANY, title) {
  wxInitAllImageHandlers();
  initialize_images();
  wxIcon frame_icon;
  frame_icon.CopyFromBitmap(*_img_bookon_icon);
  SetIcon(frame_icon);

  _create_controls();
  _create_menubar();
  _create_statusbar();
  _connect_events();
}

void FrameMain::_create_controls() {
  this->SetSizeHints(wxDefaultSize, wxDefaultSize);

  wxBoxSizer *bSizer1;
  bSizer1 = new wxBoxSizer(wxVERTICAL);

  wxPanel *m_panel1;
  m_panel1 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
  wxBoxSizer *bSizer2;
  bSizer2 = new wxBoxSizer(wxVERTICAL);

  m_data_ctrl = new wxDataViewCtrl(m_panel1, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), 0);
  bSizer2->Add(m_data_ctrl, 1, wxEXPAND, 5);

  wxBoxSizer *bSizer3;
  bSizer3 = new wxBoxSizer(wxHORIZONTAL);

  bSizer3->Add(0, 0, 1, wxEXPAND, 5);

  m_search_ctrl = new wxSearchCtrl(m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(250, -1), 0);
#ifndef __WXMAC__
  m_search_ctrl->ShowSearchButton(true);
#endif
  m_search_ctrl->ShowCancelButton(true);
  bSizer3->Add(m_search_ctrl, 0, wxALL, 5);

  bSizer2->Add(bSizer3, 0, wxEXPAND, 5);

  m_panel1->SetSizer(bSizer2);
  m_panel1->Layout();
  bSizer2->Fit(m_panel1);
  bSizer1->Add(m_panel1, 1, wxEXPAND, 5);

  this->SetSizer(bSizer1);
  this->Layout();
}

void FrameMain::_connect_events() {
  Bind(wxEVT_MENU, &FrameMain::OnQuit, this, ID_QUIT);
  Bind(wxEVT_MENU, &FrameMain::OnAbout, this, ID_ABOUT);
}

void FrameMain::_create_statusbar() {
  CreateStatusBar(2);
  SetStatusBarPane(-1);
  SetStatusText(
      wxString::Format(_("Version: %s.%s.%s (%s)"), bookon_MAJOR_VERSION, bookon_MINOR_VERSION, GIT_NUMBER, GIT_REV),
      1);
}

void FrameMain::_create_menubar() {
  wxMenu *fileMenu = new wxMenu;
  wxMenu *helpMenu = new wxMenu;
  helpMenu->Append(ID_ABOUT, "&About\tF1", "Show about dialog");
  fileMenu->Append(ID_QUIT, "E&xit\tAlt-X", "Quit this program");

  wxMenuBar *menuBar = new wxMenuBar();
  menuBar->Append(fileMenu, "&File");
  menuBar->Append(helpMenu, "&Help");
  SetMenuBar(menuBar);
}

void FrameMain::OnQuit(wxCommandEvent &WXUNUSED(event)) {
  Close(true);
}

void FrameMain::OnAbout(wxCommandEvent &WXUNUSED(event)) {
  FrameAbout my_dlg(this);
  my_dlg.ShowModal();
}
