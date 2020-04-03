#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDummyInputManager.h>
#include <g4main/PHG4Reco.h>
#include <g4main/HepMCNodeReader.h>
#include <g4main/ReadEICFiles.h>
#include <phool/recoConsts.h>
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libg4testbench.so)
#endif

using namespace std;

PHG4Reco *g4 = nullptr;
const double magfield = 1.5; // in T

int Fun4All_EIC_Generator_Display(
  const std::string &inputFile = "MILOU_5x100_TOTAL_01_20evt.root"
  )
{
  //===============
  // Input options
  //===============
  // read in EIC files
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);

  ReadEICFiles *eicfile = new ReadEICFiles();
  eicfile->OpenInputFile(inputFile);
  se->registerSubsystem(eicfile);
  
  // read-in HepMC events to Geant4 if there is any
  HepMCNodeReader *hr = new HepMCNodeReader();
  se->registerSubsystem(hr);
  g4 = new PHG4Reco();
  g4->set_rapidity_coverage(1.1); // according to drawings
  g4->set_field(magfield); 
  g4->save_DST_geometry(false); // saving the geometry crashes here
  se->registerSubsystem(g4);

  Fun4AllInputManager *in = new Fun4AllDummyInputManager("JADE");
  se->registerInputManager(in);

// Start the display
  g4->InitRun(se->topNode());
  g4->ApplyDisplayAction();
  g4->ApplyCommand("/control/execute vis.mac");
// draw 1m long axis
  g4->ApplyCommand("/vis/scene/add/axes 0 0 0 100 cm");
  se->run(1);
// print some empty lines so the instructions stick out
  for (int i=0; i<5; i++)
  {
    cout << endl;
  }
  cout << "Type displaycmd() to see some commands to change the display" << endl;
  cout << "If you want to run more events, do:" << endl;
  cout << "Fun4AllServer *se = Fun4AllServer::instance();" << endl;
  cout << "se->run(1);" << endl;
  return 0;
}

void displaycmd()
{
  cout << "draw 1m axis: " << endl;
  cout << " g4->ApplyCommand(\"/vis/scene/add/axes 0 0 0 100 cm\")" << endl;
  cout << "zoom" << endl;
  cout << " g4->ApplyCommand(\"/vis/viewer/zoom 1\")" << endl;
  cout << "viewpoint:" << endl;
  cout << " g4->ApplyCommand(\"/vis/viewer/set/viewpointThetaPhi 0 0\")" << endl;
  cout << "panTo:" << endl;
  cout << " g4->ApplyCommand(\"/vis/viewer/panTo 0 0 cm\")" << endl;
  cout << "print to eps:" << endl;
  cout << " g4->ApplyCommand(\"/vis/ogl/printEPS\")" << endl;
  cout << "set background color:" << endl;
  cout << " g4->ApplyCommand(\"/vis/viewer/set/background white\")" << endl;
}
