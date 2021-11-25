#include "framemain.h"
#include "frameabout.h"

#include "bitmap.h"

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
  SetSizeHints(wxDefaultSize, wxDefaultSize);
  wxBoxSizer *bSizer1;
  bSizer1 = new wxBoxSizer(wxVERTICAL);

  m_text_ctrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
  bSizer1->Add(m_text_ctrl, 1, wxEXPAND, 5);

  SetSizer(bSizer1);
  Layout();
}

void FrameMain::_connect_events() {
  Bind(wxEVT_MENU, &FrameMain::OnQuit, this, ID_QUIT);
  Bind(wxEVT_MENU, &FrameMain::OnAbout, this, ID_ABOUT);
}

void FrameMain::_create_statusbar() {
  CreateStatusBar(2);
  SetStatusBarPane(-1);
  SetStatusText(wxString::Format(_("Version: %s.%s.%s (%s)"),
                                 bookon_MAJOR_VERSION,
                                 bookon_MINOR_VERSION,
                                 GIT_NUMBER, GIT_REV),1);
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
