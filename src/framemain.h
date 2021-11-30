#ifndef _MAINFRAME_H
#define _MAINFRAME_H

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include <wx/splitter.h>
#include <wx/srchctrl.h>

#include <wx/systhemectrl.h>
#include "contolmain.h"

enum { ID_QUIT = wxID_EXIT, ID_ABOUT = wxID_ABOUT, ID_GROUP_ADD, ID_GROUP_REMOVE, ID_GROUP_ENTRY_ADD, ID_GROUP_ENTRY_REMOVE };

class FrameMain : public wxFrame {
 public:
  FrameMain(const wxString& title);

  void OnQuit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);
  void OnGroupNew(wxCommandEvent& event);
  void OnGroupRemove(wxCommandEvent& event);
  void OnGroupEntryNew(wxCommandEvent & event);
  void OnGroupEntryRemove(wxCommandEvent & event);

 private:
  void _create_menubar();
  void _create_statusbar();
  void _connect_events();
  void _create_controls();

  wxDataViewTreeCtrl* m_tree_ctrl;
  wxDataViewListCtrl* m_list_ctrl;
  wxSearchCtrl* m_search_ctrl;

  ControlMain* m_control = nullptr;
};

#endif
