#ifndef _MAINFRAME_H
#define _MAINFRAME_H

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include "treectrl.h"
#include <wx/srchctrl.h>
#include <wx/splitter.h>

enum { ID_QUIT = wxID_EXIT, ID_ABOUT = wxID_ABOUT };

class FrameMain : public wxFrame {
public:
 FrameMain(const wxString &title);

  void OnQuit(wxCommandEvent &event);
  void OnAbout(wxCommandEvent &event);

private:
  void _create_menubar();
  void _create_statusbar();
  void _connect_events();
  void _create_controls();

protected:
 wxDataViewListCtrl* m_tree_ctrl;
 wxDataViewTreeCtrl* m_list_ctrl;
 wxSearchCtrl* m_search_ctrl;
};

#endif
