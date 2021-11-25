#include "gtest/gtest.h"
#include <wx/filename.h>
#include "test_param.h"
#include "bookmark.pb.h"
#include <fstream>
#include <iostream>
using namespace bk;

TEST(TestSuite, Empty){
  ASSERT_TRUE(true); // testing unit test
}

TEST(TestSuite, InitBookMark){
  Bookmark bk;
  bk.set_description("test");
  bk.set_action(Bookmark_Action_WEB);
  ASSERT_EQ(bk.description(), "test");
}

TEST(TestSuite, WriteReadOneBookMark){
  Book book;
  Bookmark * bk = book.add_bookmarks();
  bk->set_description("test");
  bk->set_action(Bookmark_Action_WEB);

  wxFileName my_filename(UNIT_TESTING_DATA_OUTPUT_PATH, "test1out.bon");
  if (!wxDirExists(UNIT_TESTING_DATA_OUTPUT_PATH)){
    wxMkdir(UNIT_TESTING_DATA_OUTPUT_PATH);
  }
  if (my_filename.Exists()){
    wxRemoveFile(my_filename.GetFullPath());
  }

  {
    std::fstream output(my_filename.GetFullPath(), std::ios::out | std::ios::trunc | std::ios::binary);
    book.SerializeToOstream(&output);
  }
  ASSERT_TRUE(my_filename.Exists());
  {
    Book my_read_book;
    std::fstream input(my_filename.GetFullPath(), std::ios::in | std::ios::binary);
    my_read_book.ParseFromIstream(&input);
    ASSERT_EQ(my_read_book.bookmarks_size(),1 ); // only one bookmark
    ASSERT_EQ(my_read_book.bookmarks(0).description(), "test");
    ASSERT_EQ(my_read_book.bookmarks(0).action(), Bookmark_Action_WEB);
  }
}