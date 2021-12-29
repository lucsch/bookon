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
  Folder fold;
  fold.set_name("test_folder");
  fold.set_is_directory(false);
  ASSERT_EQ(fold.name(), "test_folder");
  ASSERT_EQ(fold.is_directory(), false);
}

TEST(TestSuite, WriteReadOneBookMark){

  // create a list with one folder containing one bookmark
  FolderList my_list;

  Folder * my_fold = my_list.add_folders();
  my_fold->set_name("test_folder");
  my_fold->set_is_directory(false);

  Folder::Bookmark * my_book = my_fold->add_bookmarks();
  my_book->set_description("Open google");
  my_book->set_command("www.google.com");
  my_book->set_action(Folder_Bookmark_Action_WEB);

  // write the list
  wxFileName my_filename(UNIT_TESTING_DATA_OUTPUT_PATH, "test1out.bon");
  if (!wxDirExists(UNIT_TESTING_DATA_OUTPUT_PATH)){
    wxMkdir(UNIT_TESTING_DATA_OUTPUT_PATH);
  }
  if (my_filename.Exists()){
    wxRemoveFile(my_filename.GetFullPath());
  }

  {
    std::fstream output(my_filename.GetFullPath().ToStdString(), std::ios::out | std::ios::trunc | std::ios::binary);
    my_list.SerializeToOstream(&output);
  }

  // read the data back
  ASSERT_TRUE(my_filename.Exists());
  {
    FolderList my_list;
    std::fstream input(my_filename.GetFullPath().ToStdString(), std::ios::in | std::ios::binary);
    my_list.ParseFromIstream(&input);

    ASSERT_EQ(my_list.folders_size(),1); // one folder
    Folder my_fold = my_list.folders(0);
    ASSERT_EQ(my_fold.name(), "test_folder");
    ASSERT_EQ(my_fold.bookmarks_size(), 1); // one bookmark
    ASSERT_EQ(my_fold.bookmarks(0).description(), "Open google");
  }
}
