
#include "treectrl.h"

TreeCtrl::TreeCtrl(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style,
                   const wxValidator &validator, const wxString &name)
    : wxDataViewCtrl(parent, id, pos, size, style, validator, name) {
  // add column description
  wxDataViewTextRenderer *tr = new wxDataViewTextRenderer("string", wxDATAVIEW_CELL_INERT);
  wxDataViewColumn *column0 = new wxDataViewColumn("description", tr, 0, FromDIP(200), wxALIGN_LEFT,
                                                   wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE);

  // add column action
  wxDataViewBitmapRenderer *br = new wxDataViewBitmapRenderer();
  wxDataViewColumn *column1 = new wxDataViewColumn("action", br, 1, FromDIP(200), wxALIGN_CENTER,
                                                   wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE);

  // add column path
  wxDataViewTextRenderer *tr2 = new wxDataViewTextRenderer("string", wxDATAVIEW_CELL_INERT);
  wxDataViewColumn *column2 = new wxDataViewColumn("Path", tr2, 2, FromDIP(400), wxALIGN_LEFT,
                                                   wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE);

  AppendColumn(column0);
  AppendColumn(column1);
  AppendColumn(column2);

}
