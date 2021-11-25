#include "app.h"

#include "framemain.h"

IMPLEMENT_APP(MainApp)

bool MainApp::OnInit() {
  if (!wxApp::OnInit()) {
    return false;
  }

  FrameMain *frame = new FrameMain("Bookon");
  frame->Show(true);
  return true;
}