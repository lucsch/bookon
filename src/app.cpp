#include "app.h"
#include "mainframe.h"

IMPLEMENT_APP(MainApp)

bool MainApp::OnInit() {
  if (!wxApp::OnInit()) {
    return false;
  }

  MainFrame *frame = new MainFrame("Minimal wxWidgets App");
  frame->Show(true);
  return true;
}