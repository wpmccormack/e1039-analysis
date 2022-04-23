#include <iomanip>
#include <sstream>
#include <fstream>
#include <TFile.h>
#include <TGraph.h>
#include <TSpline.h>
#include <geom_svc/GeomSvc.h>
#include "RTCurve.h"
#include "CalibParam.h"
using namespace std;

const double CalibParam::DT_RT = 2.5;

CalibParam::CalibParam()
  : m_ana_d0 (false)
  , m_ana_d1 (false)
  , m_ana_d2 (false)
  , m_ana_d3p(false)
  , m_ana_d3m(true ) // hard-coded for now
  , fix_time_window(false)
{
  memset(m_rtc, 0, sizeof(m_rtc));
}

CalibParam::~CalibParam()
{
  ;
}

void CalibParam::Init(const int n_rt_pt)
{
  T_MIN[ 0] = 1700;  T_MAX[ 0] = 1820;
  T_MIN[ 1] = 1700;  T_MAX[ 1] = 1820;
  T_MIN[ 2] = 1700;  T_MAX[ 2] = 1820;
  T_MIN[ 3] = 1720;  T_MAX[ 3] = 1840;
  T_MIN[ 4] = 1700;  T_MAX[ 4] = 1820;
  T_MIN[ 5] = 1700;  T_MAX[ 5] = 1820;

  T_MIN[ 6] = 1700;  T_MAX[ 6] = 1800;
  T_MIN[ 7] = 1700;  T_MAX[ 7] = 1800;
  T_MIN[ 8] = 1700;  T_MAX[ 8] = 1800;
  T_MIN[ 9] = 1700;  T_MAX[ 9] = 1800;
  T_MIN[10] = 1700;  T_MAX[10] = 1800;
  T_MIN[11] = 1700;  T_MAX[11] = 1800;

  T_MIN[12] =  800;  T_MAX[12] = 1300;
  T_MIN[13] =  800;  T_MAX[13] = 1300;
  T_MIN[14] =  800;  T_MAX[14] = 1300;
  T_MIN[15] =  800;  T_MAX[15] = 1300;
  T_MIN[16] =  800;  T_MAX[16] = 1300;
  T_MIN[17] =  800;  T_MAX[17] = 1300;

  T_MIN[18] =  700;  T_MAX[18] = 1200;
  T_MIN[19] =  700;  T_MAX[19] = 1200;
  T_MIN[20] =  700;  T_MAX[20] = 1200;
  T_MIN[21] =  700;  T_MAX[21] = 1200;
  T_MIN[22] =  700;  T_MAX[22] = 1200;
  T_MIN[23] =  700;  T_MAX[23] = 1200;

  T_MIN[24] =  800;  T_MAX[24] = 1300;
  T_MIN[25] =  800;  T_MAX[25] = 1300;
  T_MIN[26] =  800;  T_MAX[26] = 1300;
  T_MIN[27] =  800;  T_MAX[27] = 1300;
  T_MIN[28] =  800;  T_MAX[28] = 1300;
  T_MIN[29] =  800;  T_MAX[29] = 1300;

  GeomSvc* geom = GeomSvc::instance();
  for (int ip = 0; ip < N_PL; ip++) {
    R_MAX[ip] = geom->getCellWidth(ip+1) / 2;
    m_rtc[ip] = new RTCurve(n_rt_pt);
    m_gr_t2r_in[ip] = new TGraph();
  }
}

void CalibParam::ReadRTParam(const string fname)
{
  cout << "ReadRTParam(): input = " << fname << "." << endl;
  ifstream ifs(fname.c_str());
  if (! ifs) {
    cerr << "ERROR:  Cannot open the input file.  Abort." << endl;
    exit(1);
  }

  char buf[300];
  while (ifs.getline(buf, 300)) {
    if (buf[0] != 'D') continue; // comment or PT lines
    istringstream iss(buf);
    string det;
    double t, x, dt, dx;
    iss >> det >> t >> x >> dt >> dx;
    int det_id = GeomSvc::instance()->getDetectorID(det);
    TGraph* gr = m_gr_t2r_in[det_id - 1];
    gr->SetPoint(gr->GetN(), t, x);
  }

  ifs.close();
}

void CalibParam::WriteRTParam(const string dir_name, const string fname)
{
  cout << "WriteRTParam(): output = " << dir_name << "." << endl;
  ostringstream oss;
  oss << dir_name << "/" << fname;
  ofstream ofs1(oss.str().c_str()); 
  ofs1.setf(ios_base::fixed, ios_base::floatfield);

  ofs1 << "#det\tt\tx\tdt\tdx\n";

  GeomSvc* geom = GeomSvc::instance();
  for (int ip = 0; ip < N_PL; ip++) {
    if      (ip <  6) { if (! m_ana_d0 ) continue; }
    else if (ip < 12) { if (! m_ana_d1 ) continue; }
    else if (ip < 18) { if (! m_ana_d2 ) continue; }
    else if (ip < 24) { if (! m_ana_d3p) continue; }
    else              { if (! m_ana_d3m) continue; }

    int n_pt;
    double t_min, t_max;
    if (fix_time_window) {
      t_min = T1[ip];
      t_max = T0[ip];
      n_pt  = (int)((t_max - t_min) / DT_RT) + 1;
    } else {
      n_pt  = m_gr_t2r_in[ip]->GetN();
      t_min = m_gr_t2r_in[ip]->GetX()[0];
      t_max = m_gr_t2r_in[ip]->GetX()[n_pt-1];
      if (t_min > t_max) {
        double t_tmp = t_min;
        t_min = t_max;
        t_max = t_tmp;
      }
    }

    string det = geom->getDetectorName(ip+1);
    double dx  = m_rtc[ip]->GetRWidth();
    double dt  = dx * (t_max - t_min) / R_MAX[ip];

    for (int i_pt = 0; i_pt < n_pt; i_pt++) {
      double t = t_min + DT_RT * i_pt;
      double x = m_rtc[ip]->EvalR(t);
      ofs1 << det << "\t"
           << setprecision(1) << t  << "\t"
           << setprecision(4) << x  << "\t"
           << setprecision(3) << dt << "\t"
           << setprecision(3) << dx << "\n";
    }
  }

  //ifstream ifs( "calibration_prop.txt" );
  //if(!ifs){
  //  cerr <<"!!ERROR!!  Cannot open the input file 'calibration_prop.txt'.  Abort." << endl;
  //  exit(1);
  //}    
  //string buffer;
  //while (getline(ifs, buffer)) ofs1 << buffer << endl;
  //ifs .close();
  ofs1.close();
}

void CalibParam::WriteRTGraph(const string dir_name, const string fname)
{
  ostringstream oss;
  oss << dir_name << "/" << fname;
  TFile* file = new TFile(oss.str().c_str(), "RECREATE");

  GeomSvc* geom = GeomSvc::instance();
  for(int ip = 0; ip < N_PL; ip++){
    if      (ip <  6) { if (! m_ana_d0 ) continue; }
    else if (ip < 12) { if (! m_ana_d1 ) continue; }
    else if (ip < 18) { if (! m_ana_d2 ) continue; }
    else if (ip < 24) { if (! m_ana_d3p) continue; }
    else              { if (! m_ana_d3m) continue; }

    oss.str(""); 
    oss << "gr_t2r_in_" << geom->getDetectorName(ip+1);
    m_gr_t2r_in[ip]->SetName(oss.str().c_str());
    m_gr_t2r_in[ip]->Write();

    TGraph* gr_t2dr = m_rtc[ip]->GetT2DRGraph();
    oss.str("");
    oss << "gr_t2dr_" << geom->getDetectorName(ip+1);
    gr_t2dr->SetName(oss.str().c_str());
    gr_t2dr->Write();

    TSpline3* spl_t2r = m_rtc[ip]->GetT2RSpline();
    oss.str("");
    oss << "spl_t2r_" << geom->getDetectorName(ip+1);
    spl_t2r->SetName(oss.str().c_str());
    spl_t2r->Write();
  }

  file->Write();
  file->Close();
}

void CalibParam::ReadTimeWindow(const std::string fname)
{
  cout << "ReadTimeWindow(): input = " << fname << "." << endl;
  ifstream ifs(fname.c_str());
  if (! ifs) {
    cerr << "ERROR:  Cannot open the input file.  Abort." << endl;
    exit(1);
  }
  int det_id;
  double t1, t0;
  string det_name;
  while (ifs >> det_id >> t1 >> t0 >> det_name) {
    cout << "  " << det_id << "\t" << t1 << "\t" << t0 << "\t" << det_name << "\n";
    T1[det_id-1] = t1;
    T0[det_id-1] = t0;
  }
  ifs.close();
  fix_time_window = true;
}
