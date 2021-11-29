#ifndef _MAINFRAME_H
#define _MAINFRAME_H

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include "treectrl.h"
#include "treemodel.h"
#include <wx/srchctrl.h>

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
  TreeCtrl *m_data_ctrl = nullptr;
  // wxDataViewCtrl* m_data_ctrl;
  wxSearchCtrl * m_search_ctrl;

  wxObjectDataPtr<TreeModel> m_tree_model;
};

#endif
