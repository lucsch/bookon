#ifndef BOOKON_TREECTRL_H
#define BOOKON_TREECTRL_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include <wx/dataview.h>

class TreeCtrl : public wxDataViewCtrl {
 public:
  TreeCtrl(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize, long style = 0, const wxValidator& validator = wxDefaultValidator,
           const wxString& name = wxDataViewCtrlNameStr);
};

#endif  // BOOKON_TREECTRL_H
