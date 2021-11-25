#include "gtest/gtest.h"
#include <wx/filename.h>
#include "test_param.h"
#include "bookmark.pb.h"
using namespace bk;

TEST(TestSuite, Empty){
  ASSERT_TRUE(true); // testing unit test
}

TEST(TestSuite, InitBookMark){
  Bookmark bk;
  bk.set_id(123);
  bk.set_description("test");
  bk.set_action(Bookmark_Action_WEB);
  ASSERT_TRUE(bk.has_id());
  ASSERT_EQ(bk.id(), 123);
}