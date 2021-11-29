#include "framemain.h"

#include "bitmap.h"
#include "frameabout.h"

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

    _create_controls();
    _create_menubar();
    _create_statusbar();
    _connect_events();

    m_control = new ControlMain(m_tree_ctrl, m_list_ctrl);
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

    m_tree_ctrl = new wxDataViewTreeCtrl(m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_NO_HEADER);
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
    Bind(wxEVT_MENU, &FrameMain::OnAbout, this, ID_ABOUT);
    Bind(wxEVT_MENU, &FrameMain::OnGroupNew, this, ID_GROUP_ADD);
    Bind(wxEVT_MENU, &FrameMain::OnGroupRemove, this, ID_GROUP_REMOVE);
    Bind(wxEVT_MENU, &FrameMain::OnGroupEntryNew, this, ID_GROUP_ENTRY_ADD);
    Bind(wxEVT_MENU, &FrameMain::OnGroupEntryRemove, this, ID_GROUP_ENTRY_REMOVE);

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
    fileMenu->Append(ID_QUIT, "E&xit\tAlt-X", "Quit this program");
    menuBar->Append(fileMenu, "&File");

    // GROUP
    wxMenu *groupMenu = new wxMenu;
    groupMenu->Append(ID_GROUP_ADD, _("New group entry") + _T("\t") + _T("Ctrl+G"));
    groupMenu->Append(ID_GROUP_REMOVE, _("Remove group entry"));
    groupMenu->AppendSeparator();
    groupMenu->Append(ID_GROUP_ENTRY_ADD, _("New entry") + _T("\t") + _T("Ctrl+E"));
    groupMenu->Append(ID_GROUP_ENTRY_REMOVE, _("Remove entry"));
    menuBar->Append(groupMenu, _("&Group"));

    // HELP
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(ID_ABOUT, "&About\tF1", "Show about dialog");
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

void FrameMain::OnGroupNew(wxCommandEvent &event) {
    m_control->AddGroup("Test");
}

void FrameMain::OnGroupRemove(wxCommandEvent &event) {
    m_control->RemoveGroup();
}

void FrameMain::OnGroupEntryNew(wxCommandEvent &event) {
    m_control->AddGroupItem("group item");
}

void FrameMain::OnGroupEntryRemove(wxCommandEvent &event) {
    m_control->RemoveGroupItem();
}
