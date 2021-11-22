#ifndef TEST_WXWIDGETS_MAINAPP_H
#define TEST_WXWIDGETS_MAINAPP_H

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

class MainApp : public wxApp {
public:
  virtual bool OnInit();
};

#endif
