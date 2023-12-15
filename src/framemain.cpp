#include "framemain.h"

#include "bitmap.h"
#include "frameabout.h"

const int ID_GROUP_ADD = wxNewId();
const int ID_GROUP_ADD_INSIDE = wxNewId();
const int ID_GROUP_REMOVE = wxNewId();
const int ID_GROUP_ENTRY_ADD = wxNewId();
const int ID_GROUP_ENTRY_REMOVE = wxNewId();
const int ID_BOOK_ADD = wxNewId();
const int ID_BOOK_EDIT = wxNewId();
const int ID_BOOK_REMOVE = wxNewId();

extern const char *bookon_MAJOR_VERSION;
extern const char *bookon_MINOR_VERSION;
extern const char *GIT_REV;
extern const char *GIT_TAG;
extern const char *GIT_BRANCH;
extern const char *GIT_NUMBER;

FrameMain::FrameMain(const wxString &title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(900, 700)) {
  wxInitAllImageHandlers();
  initialize_images();
  wxIcon frame_icon;
  frame_icon.CopyFromBitmap(*_img_bookon_icon);
  SetIcon(frame_icon);

  // open the config file, will be saved OnClosing
  m_file_history = new wxFileHistory(9, wxID_FILE);
  wxConfigBase::Set(new wxFileConfig(title));
  wxConfigBase *my_config = wxConfigBase::Get();
  if (my_config) {
    m_file_history->Load(*my_config);
  }

  _create_controls();
  _create_menubar();
  _create_statusbar();
  _connect_events();
  _create_toolbar();

  _assign_image_to_treectrl();

  m_control = new ControlMain(m_tree_ctrl, m_list_ctrl);
  // m_tree_ctrl->EnableSystemTheme(false);
}

void FrameMain::_create_controls() {
  this->SetSizeHints(wxDefaultSize, wxDefaultSize);

  wxBoxSizer *bSizer1;
  bSizer1 = new wxBoxSizer(wxVERTICAL);

  wxPanel *m_panel1;
  m_panel1 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
  wxBoxSizer *bSizer2;
  bSizer2 = new wxBoxSizer(wxVERTICAL);

  wxBoxSizer *bSizer4;
  bSizer4 = new wxBoxSizer(wxHORIZONTAL);

  wxSplitterWindow *m_splitter1;
  m_splitter1 = new wxSplitterWindow(m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D | wxSP_LIVE_UPDATE);
  m_splitter1->SetMinimumPaneSize(200);

  wxPanel *m_panel2;
  m_panel2 = new wxPanel(m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
  wxBoxSizer *bSizer6;
  bSizer6 = new wxBoxSizer(wxVERTICAL);

  m_tree_ctrl =
      new wxTreeCtrl(m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                     wxTR_DEFAULT_STYLE | wxTR_NO_LINES | wxTR_HIDE_ROOT | wxTR_TWIST_BUTTONS | wxTR_EDIT_LABELS);
  bSizer6->Add(m_tree_ctrl, 1, wxEXPAND, 5);

  m_panel2->SetSizer(bSizer6);
  m_panel2->Layout();
  bSizer6->Fit(m_panel2);
  wxPanel *m_panel3;
  m_panel3 = new wxPanel(m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
  wxBoxSizer *bSizer7;
  bSizer7 = new wxBoxSizer(wxVERTICAL);

  m_list_ctrl = new wxDataViewListCtrl(m_panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_HORIZ_RULES);
  bSizer7->Add(m_list_ctrl, 1, wxEXPAND, 5);

  m_panel3->SetSizer(bSizer7);
  m_panel3->Layout();
  bSizer7->Fit(m_panel3);
  m_splitter1->SplitVertically(m_panel2, m_panel3, 300);
  bSizer4->Add(m_splitter1, 1, wxEXPAND, 5);

  bSizer2->Add(bSizer4, 1, wxEXPAND, 5);

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

  m_splitter1->SetSashPosition(300);
}

void FrameMain::_connect_events() {
  Bind(wxEVT_MENU, &FrameMain::OnQuit, this, ID_QUIT);
  Bind(wxEVT_CLOSE_WINDOW, &FrameMain::OnClose, this, wxID_ANY);
  Bind(wxEVT_MENU, &FrameMain::OnNew, this, ID_FILE_NEW);
  Bind(wxEVT_MENU, &FrameMain::OnOpen, this, ID_FILE_OPEN);
  Bind(wxEVT_MENU, &FrameMain::OnSave, this, ID_FILE_SAVE);
  Bind(wxEVT_MENU, &FrameMain::OnSaveAs, this, ID_FILE_SAVE_AS);
  Bind(wxEVT_MENU, &FrameMain::OnAbout, this, ID_ABOUT);
  Bind(wxEVT_MENU, &FrameMain::OnGroupNew, this, ID_GROUP_ADD);
  Bind(wxEVT_MENU, &FrameMain::OnGroupNewInside, this, ID_GROUP_ADD_INSIDE);
  Bind(wxEVT_MENU, &FrameMain::OnGroupRemove, this, ID_GROUP_REMOVE);
  Bind(wxEVT_MENU, &FrameMain::OnGroupEntryNew, this, ID_GROUP_ENTRY_ADD);
  Bind(wxEVT_MENU, &FrameMain::OnGroupEntryRemove, this, ID_GROUP_ENTRY_REMOVE);
  Bind(wxEVT_MENU, &FrameMain::OnBookmarkAdd, this, ID_BOOK_ADD);
  Bind(wxEVT_MENU, &FrameMain::OnBookmarkEdit, this, ID_BOOK_EDIT);
  Bind(wxEVT_MENU, &FrameMain::OnBookmarkRemove, this, ID_BOOK_REMOVE);
  Bind(wxEVT_MENU, &FrameMain::OnBookmarkFind, this, ID_BOOK_FIND);
  Bind(wxEVT_UPDATE_UI, &FrameMain::OnUpdateIdleTitle, this, this->GetId());
  Bind(wxEVT_MENU, &FrameMain::OnOpenRecent, this, m_file_history->GetBaseId(), wxID_FILE9);
}

void FrameMain::OnOpenRecent(wxCommandEvent &event) {
  int menu_index = event.GetId() - m_file_history->GetBaseId();
  wxFileName myName(m_file_history->GetHistoryFile(menu_index));
  wxASSERT(myName.IsOk());
  if (!myName.Exists()) {
    wxLogError("'%s' didn't exists!", myName.GetFullPath());
    m_file_history->RemoveFileFromHistory(menu_index);
  }
  do_open_file(myName.GetFullPath());
}

void FrameMain::_create_statusbar() {
  CreateStatusBar(2);
  SetStatusBarPane(-1);
  SetStatusText(
      wxString::Format(_("Version: %s.%s.%s (%s)"), bookon_MAJOR_VERSION, bookon_MINOR_VERSION, GIT_NUMBER, GIT_REV),
      1);
}

void FrameMain::_create_menubar() {
  wxMenuBar *menuBar = new wxMenuBar();

  // FILE
  wxMenu *fileMenu = new wxMenu;
  fileMenu->Append(ID_FILE_NEW, _("New") + "\tCtrl+N");
  fileMenu->AppendSeparator();
  fileMenu->Append(ID_FILE_OPEN, _("Open...") + "\tCtrl+O");

  wxMenu *m_menu_recent = new wxMenu();
  m_file_history->UseMenu(m_menu_recent);
  m_file_history->AddFilesToMenu();
  fileMenu->AppendSubMenu(m_menu_recent, _("Recent files"));

  fileMenu->Append(ID_FILE_SAVE, _("Save") + "\tCtrl+S");
  fileMenu->Append(ID_FILE_SAVE_AS, _("Save as...") + "\tCtrl+Alt+S");
  fileMenu->AppendSeparator();
  fileMenu->Append(ID_QUIT, "E&xit\tAlt-X", "Quit this program");
  menuBar->Append(fileMenu, "&File");

  // GROUP
  wxMenu *groupMenu = new wxMenu;
  groupMenu->Append(ID_GROUP_ADD, _("New group") + _T("\t") + _T("Ctrl+G"));
  groupMenu->Append(ID_GROUP_ADD_INSIDE, _("New group inside") + _T("\t") + _T("Ctrl+Alt+G"));
  groupMenu->Append(ID_GROUP_REMOVE, _("Remove group"));
  groupMenu->AppendSeparator();
  groupMenu->Append(ID_GROUP_ENTRY_ADD, _("New entry") + _T("\t") + _T("Ctrl+E"));
  groupMenu->Append(ID_GROUP_ENTRY_REMOVE, _("Remove entry"));
  menuBar->Append(groupMenu, _("&Group"));

  // BOOKMARK
  wxMenu *bookmarkMenu = new wxMenu;
  bookmarkMenu->Append(ID_BOOK_ADD, _("Add bookmark") + _T("\t") + _T("Ctrl+B"));
  bookmarkMenu->Append(ID_BOOK_EDIT, _("Edit bookmark"));
  bookmarkMenu->Append(ID_BOOK_REMOVE, _("Remove bookmark"));
  bookmarkMenu->AppendSeparator();
  bookmarkMenu->Append(ID_BOOK_FIND, _("Search...") + _T("\tCtrl+F"));
  menuBar->Append(bookmarkMenu, _("&Bookmarks"));

  // HELP
  wxMenu *helpMenu = new wxMenu;
  helpMenu->Append(ID_ABOUT, "&About\tF1", "Show about dialog");
  menuBar->Append(helpMenu, "&Help");

  SetMenuBar(menuBar);
}

void FrameMain::OnQuit(wxCommandEvent &WXUNUSED(event)) {
  Close(false);
}

void FrameMain::OnAbout(wxCommandEvent &WXUNUSED(event)) {
  FrameAbout my_dlg(this);
  my_dlg.ShowModal();
}

void FrameMain::OnGroupNew(wxCommandEvent &event) {
  m_control->AddGroup("New Folder", true);
}

void FrameMain::OnGroupNewInside(wxCommandEvent &event) {
  m_control->AddGroup("New folder", false);
}

void FrameMain::OnGroupRemove(wxCommandEvent &event) {
  m_control->RemoveGroup();
}

void FrameMain::OnGroupEntryNew(wxCommandEvent &event) {
  m_control->AddGroupItem("New Item");
}

void FrameMain::OnGroupEntryRemove(wxCommandEvent &event) {
  m_control->RemoveGroupItem();
}

void FrameMain::OnBookmarkAdd(wxCommandEvent &event) {
  m_control->BookMarkAdd();
}

void FrameMain::OnBookmarkEdit(wxCommandEvent &event) {
  m_control->BookMarkEdit();
}

void FrameMain::OnBookmarkRemove(wxCommandEvent &event) {
  m_control->BookMarkDel();
}

void FrameMain::OnBookmarkFind(wxCommandEvent &event) {
  m_search_ctrl->SetFocus();
}

void FrameMain::OnNew(wxCommandEvent &event) {
  m_document_name = _("UNTITLED");
  _update_title();
}

void FrameMain::_update_title() {
  wxString my_star = "";
  if (m_control->IsProjectModified()) {
    my_star = "*";
  }
  SetTitle(m_soft_name + " - " + m_document_name + my_star);
}

void FrameMain::OnOpen(wxCommandEvent &event) {
  wxFileDialog my_dlg(this, _("Open file"), "", "", "bkdoc files (*.bkdoc)|*.bkdoc", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
  if (my_dlg.ShowModal() == wxID_CANCEL) {
    return;
  }
  do_open_file(my_dlg.GetPath());
  m_file_history->AddFileToHistory(my_dlg.GetPath());
}

void FrameMain::OnSave(wxCommandEvent &event) {
  if (m_document_name.IsEmpty()) {
    OnSaveAs(event);
    return;
  }
  m_control->SaveFile(m_document_name);
}

void FrameMain::OnSaveAs(wxCommandEvent &event) {
  wxString default_dir = wxEmptyString;
  if (!m_document_name.IsEmpty()) {
    default_dir = wxFileName(m_document_name).GetPath();
  }

  wxFileDialog my_dlg(this, _("Save file"), default_dir, "", "bkdoc files (*.bkdoc)|*.bkdoc",
                      wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
  if (my_dlg.ShowModal() == wxID_CANCEL) {
    return;
  }
  wxASSERT(m_control);
  m_control->SaveFile(my_dlg.GetPath());
}

void FrameMain::do_open_file(const wxString &filename) {
  wxASSERT(m_control);
  m_control->OpenFile(filename);
  m_document_name = filename;
  _update_title();
}

void FrameMain::_create_toolbar() {
  long my_toolbar_style = wxTB_DEFAULT_STYLE;
#ifdef __WXOSX__
  my_toolbar_style = my_toolbar_style | wxTB_TEXT;
#endif
  wxToolBar *my_toolbar = FrameMain::CreateToolBar(my_toolbar_style);
  wxASSERT(my_toolbar);

  int ids[] = {ID_FILE_OPEN, ID_GROUP_ADD, ID_GROUP_ENTRY_ADD, ID_BOOK_ADD, ID_BOOK_EDIT, ID_BOOK_FIND};
  wxString labels[] = {_("Open"), _("Add group"), _("Add entry"), _("Add bookmark"), _("Edit bookmark"), _("Find")};
  std::vector<wxBitmap *> my_bitmaps = {_img_tb_open,         _img_tb_add_folder,    _img_tb_add_item,
                                        _img_tb_add_bookmark, _img_tb_edit_bookmark, _img_tb_find};

  // support for dark theme
  wxSystemAppearance s = wxSystemSettings::GetAppearance();
  if (s.IsDark()) {
    my_bitmaps = {_img_tb_w_open,         _img_tb_w_add_folder,    _img_tb_w_add_item,
                  _img_tb_w_add_bookmark, _img_tb_w_edit_bookmark, _img_tb_w_find};
  }

  for (int i = 0; i < (sizeof(ids) / sizeof(int)); ++i) {
    my_toolbar->AddTool(ids[i], labels[i], *(my_bitmaps[i]));
  }
  my_toolbar->Realize();
}

void FrameMain::_assign_image_to_treectrl() {
  wxASSERT(m_tree_ctrl);
  auto my_image_list = new wxImageList(18, 18);
  std::vector<wxBitmap *> my_bitmaps = {_img_tree_folder, _img_tree_book};

  // support for dark theme
  wxSystemAppearance sys_app = wxSystemSettings::GetAppearance();
  if (sys_app.IsDark()) {
    my_bitmaps = {_img_w_tree_folder, _img_w_tree_book};
  }

  my_image_list->Add(*my_bitmaps[0]);
  my_image_list->Add(*my_bitmaps[1]);
  m_tree_ctrl->SetImageList(my_image_list);
}

void FrameMain::OnUpdateIdleTitle(wxUpdateUIEvent &event) {
  _update_title();
}

void FrameMain::OnClose(wxCloseEvent &event) {
  if (m_control != nullptr && m_control->IsProjectModified()) {
    if (wxMessageBox(_("Project not saved! Close anyway ?"), _("Project not saved"), wxOK | wxCANCEL) != wxOK) {
      event.Veto();
      return;
    }
  }

  wxConfigBase *myconfig = wxConfigBase::Get();
  if (myconfig) {
    m_file_history->Save(*myconfig);
  }
  wxDELETE(m_file_history);
  event.Skip();
}
