#include "addFit.cpp"
#include "realFit.cpp"
#include "tuneFit.cpp"
#include "splitFit.cpp"

//Fitting *test_fit;
  
void doAddFit() {
  Fitting *test_fit = new addFit( "addFit");
  if (!test_fit) {
    std::cout << "BAD" << std::endl;
    return;
  }
  test_fit->theFit();
  test_fit->drawHists();
  delete test_fit;
}

void doTuneFit() {
  Fitting *test_fit = new tuneFit( "tuneFit");
  if (!test_fit) {
    std::cout << "BAD" << std::endl;
    return;
  }
  test_fit->theFit();
  test_fit->drawHists();
  delete test_fit;
}

void doRealFit() {
  Fitting *test_fit = new realFit( "realFit");
  if (!test_fit) {
    std::cout << "BAD" << std::endl;
    return;
  }
  test_fit->theFit();
  test_fit->drawHists();
  //delete test_fit;
}

void doSplitFit() {
  Fitting *test_fit = new splitFit( "splitFit");
  if (!test_fit) {
    std::cout << "BAD" << std::endl;
    return;
  }
  test_fit->theFit();
  test_fit->drawHists();
  delete test_fit;
}

