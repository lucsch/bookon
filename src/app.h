#ifndef TEST_WXWIDGETS_MAINAPP_H
#define TEST_WXWIDGETS_MAINAPP_H

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include <wx/cmdline.h>

class MainApp : public wxApp {
public:
  virtual bool OnInit();

  virtual void OnInitCmdLine(wxCmdLineParser& parser);
  virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

 private:
  wxString m_open_file = wxEmptyString;
};

#endif
