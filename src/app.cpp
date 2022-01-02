#include "app.h"

#include "framemain.h"

IMPLEMENT_APP(MainApp)

bool MainApp::OnInit() {
  if (!wxApp::OnInit()) {
    return false;
  }

  FrameMain *frame = new FrameMain("Bookon");
  if (m_open_file != wxEmptyString){
    frame->do_open_file(m_open_file);
  }
  frame->Show(true);
  return true;
}

void MainApp::OnInitCmdLine(wxCmdLineParser &parser) {
  wxApp::OnInitCmdLine(parser);
  parser.AddParam("document-file", wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
}

bool MainApp::OnCmdLineParsed(wxCmdLineParser &parser) {
  m_open_file = wxEmptyString;
  if (parser.GetParamCount() > 0){
    m_open_file = parser.GetParam(0);

    if (!wxFileExists(m_open_file)){
      wxLogError(_("Unable to open: '%s'"), m_open_file);
      m_open_file = wxEmptyString;
    }
  }
  return wxApp::OnCmdLineParsed(parser);
}
