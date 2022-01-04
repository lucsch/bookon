#ifndef _MAINFRAME_H
#define _MAINFRAME_H

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include <wx/filename.h>
#include <wx/splitter.h>
#include <wx/srchctrl.h>
#include <wx/systhemectrl.h>

#include "contolmain.h"

enum {
  ID_QUIT = wxID_EXIT,
  ID_ABOUT = wxID_ABOUT,
  ID_FILE_NEW = wxID_NEW,
  ID_FILE_OPEN = wxID_OPEN,
  ID_FILE_SAVE = wxID_SAVE,
  ID_FILE_SAVE_AS = wxID_SAVEAS,
  ID_GROUP_ADD,
  ID_GROUP_ADD_INSIDE,
  ID_GROUP_REMOVE,
  ID_GROUP_ENTRY_ADD,
  ID_GROUP_ENTRY_REMOVE,
  ID_BOOK_ADD,
  ID_BOOK_EDIT,
  ID_BOOK_REMOVE
};

class FrameMain : public wxFrame {
 public:
  FrameMain(const wxString& title);

  void OnQuit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);
  void OnGroupNew(wxCommandEvent& event);
  void OnGroupNewInside(wxCommandEvent& event);
  void OnGroupRemove(wxCommandEvent& event);
  void OnGroupEntryNew(wxCommandEvent& event);
  void OnGroupEntryRemove(wxCommandEvent& event);

  void OnBookmarkAdd(wxCommandEvent& event);
  void OnBookmarkEdit(wxCommandEvent& event);
  void OnBookmarkRemove(wxCommandEvent& event);

  void OnNew(wxCommandEvent& event);
  void OnOpen(wxCommandEvent& event);
  void OnSave(wxCommandEvent& event);
  void OnSaveAs(wxCommandEvent& event);

  void do_open_file(const wxString& filename);

 private:
  void _create_menubar();
  void _create_statusbar();
  void _connect_events();
  void _create_controls();

  wxTreeCtrl* m_tree_ctrl;
  wxDataViewListCtrl* m_list_ctrl;
  wxSearchCtrl* m_search_ctrl;

  ControlMain* m_control = nullptr;
  wxString m_document_name = wxEmptyString;
  wxString m_soft_name = _("Bookon");
  void _update_title();
};

#endif
