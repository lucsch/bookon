#include "bookdnd.h"
#include "contolmain.h"

BKDNDDataObject::BKDNDDataObject(BookMark* bookmark, wxTreeItemId parent_tree_id)
    : wxDataObjectSimple(wxDataFormat(BKDNDFormatId())) {
  m_bookmark = bookmark;
  m_bookmark_tree_parent = parent_tree_id;
}

BKDNDDataObject::~BKDNDDataObject() {
  m_bookmark = nullptr;
  m_bookmark_tree_parent = wxTreeItemId();
}

BKDNDDropTarget::BKDNDDropTarget(ControlMain* control) : wxDropTarget(new BKDNDDataObject) {
  m_control = control;
}

wxDragResult BKDNDDropTarget::OnData(wxCoord x, wxCoord y, wxDragResult def) {
  if (!GetData()){
    wxLogError("Failed to get drag an drop data");
    return wxDragNone;
  }

  wxLogDebug("Dropping ok");
  return def;
}
