#include "bookdnd.h"

#include "contolmain.h"

BKDNDDropTarget::BKDNDDropTarget(ControlMain* control)
    : wxDropTarget(new wxCustomDataObject(wxDataFormat(BKDNDFormatId()))) {
  m_control = control;
}

wxDragResult BKDNDDropTarget::OnData(wxCoord x, wxCoord y, wxDragResult def) {
  if (!GetData()) {
    wxLogError("Failed to get drag an drop data");
    return wxDragNone;
  }
  auto* my_obj = dynamic_cast<wxCustomDataObject*>(GetDataObject());
  if (my_obj->GetFormat() != wxDataFormat(BKDNDFormatId())) {
    wxLogError("Incorrect format!");
    return wxDragNone;
  }

  int* my_int = (int*)my_obj->GetData();
  m_control->DropData(x, y, *my_int);
  return def;
}
