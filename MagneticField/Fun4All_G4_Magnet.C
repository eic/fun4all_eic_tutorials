#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDummyInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <g4detectors/PHG4DetectorSubsystem.h>
#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4histos/G4HitNtuple.h>
#include <g4main/PHG4ParticleGeneratorBase.h>
#include <g4main/PHG4ParticleGenerator.h>
#include <g4main/PHG4SimpleEventGenerator.h>
#include <g4main/PHG4ParticleGun.h>
#include <g4main/PHG4Reco.h>
#include <phfield/PHFieldConfig.h>
#include <phool/recoConsts.h>
#include <eicdetectors/BeastMagnetSubsystem.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libg4detectors.so)
R__LOAD_LIBRARY(libg4histos.so)
R__LOAD_LIBRARY(libeicdetectors.so)
#endif

void Fun4All_G4_Magnet(int nEvents = -1)
{

  ///////////////////////////////////////////
  // Make the Server
  //////////////////////////////////////////
  Fun4AllServer *se = Fun4AllServer::instance();
  recoConsts *rc = recoConsts::instance();

// set magnet you want to run to true
  bool use_babar = true;
  bool use_beast = false;
  bool use_cleo = false;

// replace fieldmap with constant solenoidal field from x/y/z = 0
  bool use_solenoid_field = false;
// make magnet active volume if you want to study the hits
  bool magnet_active=false;

// if you want to fix the random seed to reproduce results
// set this flag
// nail this down so I know what the first event looks like...
//  rc->set_IntFlag("RANDOMSEED",12345); 

  double pz = 1.; // some forward momentum to prevent geantino loopers

// ParticleGun shoots right into the original MyDetector volume
  PHG4ParticleGun *gun = new PHG4ParticleGun();
//   gun->set_name("pi-");
  gun->set_name("chargedgeantino");
  gun->set_vtx(0, 0, 0); 
  gun->set_mom(0, 0.5, pz);
  gun->AddParticle("chargedgeantino",0,1,pz);
  gun->AddParticle("chargedgeantino",0,2,pz);
  gun->AddParticle("chargedgeantino",0,3,pz);
  se->registerSubsystem(gun);

//
// Geant4 setup
//
  PHG4Reco* g4Reco = new PHG4Reco();
// setup of G4: 
//   no saving of geometry: it takes time and we do not do tracking
//   so we do not need the geometry
  g4Reco->save_DST_geometry(false);

  if (use_beast)
  {
    if (use_solenoid_field)
    {
      g4Reco->set_field(3);
    }
    else
    {
      g4Reco->set_field_map(string(getenv("CALIBRATIONROOT")) + string("/Field/Map/mfield.4col.dat"), PHFieldConfig::kFieldBeast);
//      g4Reco->set_field_rescale(0.5);
    }
    BeastMagnetSubsystem *beast = new BeastMagnetSubsystem();
    beast->set_string_param("GDMPath",(string(getenv("CALIBRATIONROOT")) + string("/Magnet/BeastSolenoid.gdml")));
    beast->set_string_param("TopVolName","SOLENOID");
    beast->SetActive(magnet_active);
    beast->SuperDetector("MAGNET");
    g4Reco->registerSubsystem(beast);
  }
  else if (use_cleo)
  {
    if (use_solenoid_field)
    {
      g4Reco->set_field(2.02);
    }
    else
    {
      g4Reco->set_field_map(string(getenv("CALIBRATIONROOT")) + string("/Field/Map/SolenoidMag3D.TABLE"), PHFieldConfig::kFieldCleo);
//      g4Reco->set_field_rescale(0.5);
    }
    double magnet_inner_radius = 137.;
    double magnet_thickness = 20.;
    double magnet_length = 350.;
    PHG4CylinderSubsystem *cyl = new PHG4CylinderSubsystem("MAGNET",0);
    cyl->set_double_param("radius",magnet_inner_radius);
    cyl->set_double_param("length",magnet_length);
    cyl->set_double_param("thickness",magnet_thickness);
    cyl->set_string_param("material","G4_Al");
    cyl->set_color(0,1,0,1);
    cyl->SuperDetector("MAGNET");
    cyl->SetActive(magnet_active);
    g4Reco->registerSubsystem( cyl );
  }
  else if (use_babar)
  {
    if (use_solenoid_field)
    {
      g4Reco->set_field(1.4);
    }
    else
    {
      g4Reco->set_field_map(string(getenv("CALIBRATIONROOT")) + string("/Field/Map/sPHENIX.2d.root"), PHFieldConfig::kField2D);
      g4Reco->set_field_rescale(-1.4/1.5);
    }
    double magnet_inner_cryostat_wall_radius = 142;
    double magnet_inner_cryostat_wall_thickness = 1;
    double magnet_outer_cryostat_wall_radius = 174.5;
    double magnet_outer_cryostat_wall_thickness = 2.5;
    double magnet_coil_radius = 150.8;
    double magnet_coil_thickness = 9.38;
    double magnet_length = 379.;
    double coil_length = 361.5;
    PHG4CylinderSubsystem *cyl = new PHG4CylinderSubsystem("MAGNET",0);
    cyl->set_double_param("radius",magnet_inner_cryostat_wall_radius);
    cyl->set_int_param("lengthviarapidity",0);
    cyl->set_double_param("length",magnet_length);
    cyl->set_double_param("thickness",magnet_inner_cryostat_wall_thickness);
    cyl->set_string_param("material","Al5083"); // use 1 radiation length Al for magnet thickness
    cyl->SuperDetector("MAGNET");
    cyl->SetActive(magnet_active);
    cyl->set_color(0,0,1,1);
    g4Reco->registerSubsystem( cyl );

    cyl = new PHG4CylinderSubsystem("MAGNET", 1);
    cyl->set_double_param("radius",magnet_coil_radius);
    cyl->set_int_param("lengthviarapidity",0);
    cyl->set_double_param("length",coil_length);
    cyl->set_double_param("thickness",magnet_coil_thickness);
    cyl->set_string_param("material","Al5083"); // use 1 radiation length Al for magnet thickness
    cyl->SuperDetector("MAGNET");
    cyl->SetActive(magnet_active);
    cyl->set_color(0,0,1,1);
    g4Reco->registerSubsystem( cyl );

    cyl = new PHG4CylinderSubsystem("MAGNET", 2);
    cyl->set_double_param("radius",magnet_outer_cryostat_wall_radius);
    cyl->set_int_param("lengthviarapidity",0);
    cyl->set_double_param("length",magnet_length);
    cyl->set_double_param("thickness",magnet_outer_cryostat_wall_thickness);
    cyl->set_string_param("material","Al5083"); // use 1 radiation length Al for magnet thickness
    cyl->SuperDetector("MAGNET");
    cyl->SetActive(magnet_active);
    cyl->set_color(0,0,1,1);
    g4Reco->registerSubsystem(cyl);
  }

  se->registerSubsystem( g4Reco );

  ///////////////////////////////////////////
  // Fun4All modules
  ///////////////////////////////////////////

  G4HitNtuple *hits = new G4HitNtuple("Hits");
  hits->AddNode("MAGNET",0);
  se->registerSubsystem(hits);

// this (dummy) input manager just drives the event loop
  Fun4AllInputManager *in = new Fun4AllDummyInputManager( "Dummy");
  se->registerInputManager( in );
// events = 0 => run forever, default is -1, do not run event loop
  if (nEvents <= 0)
  {
    return 0;
  }
  se->run(nEvents);
  se->End();
  delete se;
  gSystem->Exit(0);
}
