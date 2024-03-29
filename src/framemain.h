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
#include <wx/filehistory.h>
#include <wx/fileconf.h>

#include "contolmain.h"

const int ID_QUIT = wxID_EXIT;
const int ID_ABOUT = wxID_ABOUT;
const int ID_FILE_NEW = wxID_NEW;
const int ID_FILE_OPEN = wxID_OPEN;
const int ID_FILE_SAVE = wxID_SAVE;
const int ID_FILE_SAVE_AS = wxID_SAVEAS;
const int ID_BOOK_FIND = wxID_FIND;
extern const int ID_GROUP_ADD;
extern const int ID_GROUP_ADD_INSIDE;
extern const int ID_GROUP_REMOVE;
extern const int ID_GROUP_ENTRY_ADD;
extern const int ID_GROUP_ENTRY_REMOVE;
extern const int ID_BOOK_ADD;
extern const int ID_BOOK_EDIT;
extern const int ID_BOOK_REMOVE;

class FrameMain : public wxFrame {
 public:
  FrameMain(const wxString& title);

  void OnQuit(wxCommandEvent& event);
  void OnClose(wxCloseEvent & event);
  void OnAbout(wxCommandEvent& event);
  void OnGroupNew(wxCommandEvent& event);
  void OnGroupNewInside(wxCommandEvent& event);
  void OnGroupRemove(wxCommandEvent& event);
  void OnGroupEntryNew(wxCommandEvent& event);
  void OnGroupEntryRemove(wxCommandEvent& event);

  void OnBookmarkAdd(wxCommandEvent& event);
  void OnBookmarkEdit(wxCommandEvent& event);
  void OnBookmarkRemove(wxCommandEvent& event);
  void OnBookmarkFind(wxCommandEvent& event);

  void OnNew(wxCommandEvent& event);
  void OnOpen(wxCommandEvent& event);
  void OnOpenRecent(wxCommandEvent &event);

  void OnSave(wxCommandEvent& event);
  void OnSaveAs(wxCommandEvent& event);

  void OnUpdateIdleTitle(wxUpdateUIEvent & event);

  void do_open_file(const wxString& filename);

 private:
  void _create_menubar();
  void _create_statusbar();
  void _connect_events();
  void _create_controls();
  void _create_toolbar();
  void _assign_image_to_treectrl();
  void _update_title();

  wxTreeCtrl* m_tree_ctrl;
  wxDataViewListCtrl* m_list_ctrl;
  wxSearchCtrl* m_search_ctrl;
  ControlMain* m_control = nullptr;
  wxString m_document_name = _("UNTITLED");
  wxString m_soft_name = _("Bookon");
  wxFileHistory * m_file_history;

};

#endif
