#ifndef BOOKON_BOOKDND_H
#define BOOKON_BOOKDND_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/dataobj.h>
#include <wx/dnd.h>
#include <wx/treectrl.h>

#include "bookmark.h"

class ControlMain;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Special string for dnd data format
////////////////////////////////////////////////////////////////////////////////////////////////////
static wxString BKDNDFormatId() {
  return "com.terranum.bookmark";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Drop target class (tree control)
////////////////////////////////////////////////////////////////////////////////////////////////////
class BKDNDDropTarget : public wxDropTarget {
 public:
  BKDNDDropTarget(ControlMain* control);
  virtual wxDragResult OnData(wxCoord x, wxCoord y, wxDragResult def);

 private:
  ControlMain* m_control;
};

#endif  // BOOKON_BOOKDND_H
