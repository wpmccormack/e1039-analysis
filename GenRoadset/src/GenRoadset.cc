#include <iostream>
#include <iomanip>
#include "RoadInfo.h"
#include "UtilRoad.h"
#include "AnaSignal.h"
#include "GenRoadset.h"
using namespace std;

GenRoadset::GenRoadset(const std::string label)
  : AnaBase       (label)
  , m_map_pos_top (0)
  , m_map_pos_bot (0)
  , m_map_neg_top (0)
  , m_map_neg_bot (0)

{
  m_list_pos_top.SetPNTB(+1, +1);
  m_list_pos_bot.SetPNTB(+1, -1);
  m_list_neg_top.SetPNTB(-1, +1);
  m_list_neg_bot.SetPNTB(-1, -1);
}

GenRoadset::~GenRoadset()
{
  ;
}

void GenRoadset::SetMap(AnaSignal* ana_signal)
{
  m_map_pos_top = ana_signal->GetRoadMapPosTop();
  m_map_pos_bot = ana_signal->GetRoadMapPosBot();
  m_map_neg_top = ana_signal->GetRoadMapNegTop();
  m_map_neg_bot = ana_signal->GetRoadMapNegBot();

  m_list_pos_top.Add(m_map_pos_top);
  m_list_pos_bot.Add(m_map_pos_bot);
  m_list_neg_top.Add(m_map_neg_top);
  m_list_neg_bot.Add(m_map_neg_bot);
}

void GenRoadset::GenerateRoadSetBySignal(const double frac_cut, const std::string rs_id)
{
  if (frac_cut < 0 || frac_cut > 1) return;
  DisableRoadsByWeightFraction(&m_list_pos_top, frac_cut);
  DisableRoadsByWeightFraction(&m_list_pos_bot, frac_cut);
  DisableRoadsByWeightFraction(&m_list_neg_top, frac_cut);
  DisableRoadsByWeightFraction(&m_list_neg_bot, frac_cut);
  UtilRoad::WriteToFile(m_map_pos_top, m_map_pos_bot, m_map_neg_top, m_map_neg_bot, rs_id);
}

void GenRoadset::GenerateRoadSetByFoM(const double frac_cut, const std::string rs_id)
{
  if (frac_cut < 0 || frac_cut > 1) return;
  DisableRoadsByFoM(&m_list_pos_top, frac_cut);
  DisableRoadsByFoM(&m_list_pos_bot, frac_cut);
  DisableRoadsByFoM(&m_list_neg_top, frac_cut);
  DisableRoadsByFoM(&m_list_neg_bot, frac_cut);
  UtilRoad::WriteToFile(m_map_pos_top, m_map_pos_bot, m_map_neg_top, m_map_neg_bot, rs_id);
}

void GenRoadset::DisableRoadsByWeightFraction(RoadList* road_list, const double frac)
{
  road_list->SortBySignal();

  m_ofs << "RoadList: " << road_list->GetPosNegStr() << " " << road_list->GetTopBotStr() << "\n"
        << "  First Five\n";
  UtilRoad::PrintList(road_list, +1, +5, m_ofs);
  m_ofs << "  Last Five\n";
  UtilRoad::PrintList(road_list, -5, -1, m_ofs);

  int n_ent = road_list->Size();
  double w_tot = 0;
  for (int ii = 0; ii < n_ent; ii++) w_tot += road_list->Get(ii)->GetWeight();
  double w_dis = 0;
  int i_ent = n_ent - 1;
  while (i_ent >= 0) {
    RoadInfo* info = road_list->Get(i_ent);
    info->SetOnOff(false);
    w_dis += info->GetWeight();
    if (w_dis / w_tot >= frac) break;
    i_ent--;
  }
  int n_dis = n_ent - i_ent;
  m_ofs << "  Total   : weight = " << w_tot << ", N = " << n_ent << "\n"
        << "  Disabled: weight = " << w_dis << ", N = " << n_dis << "\n"
        << "                     " << w_dis/w_tot << ", " << 1.0*n_dis/n_ent << endl;
}

void GenRoadset::DisableRoadsByFoM(RoadList* road_list, const double frac)
{
  road_list->SortByFoM();

  m_ofs << "RoadList: " << road_list->GetPosNegStr() << " " << road_list->GetTopBotStr() << "\n"
        << "  First Five\n";
  UtilRoad::PrintList(road_list, +1, +5, m_ofs);
  m_ofs << "  Last Five\n";
  UtilRoad::PrintList(road_list, -5, -1, m_ofs);

  int n_ent = road_list->Size();
  double w_tot = 0;
  for (int ii = 0; ii < n_ent; ii++) w_tot += road_list->Get(ii)->GetFoM();
  double w_dis = 0;
  int i_ent = n_ent - 1;
  while (i_ent >= 0) {
    RoadInfo* info = road_list->Get(i_ent);
    info->SetOnOff(false);
    w_dis += info->GetFoM();
    if (w_dis / w_tot >= frac) break;
    i_ent--;
  }
  int n_dis = n_ent - i_ent;
  m_ofs << "  Total   : weight = " << w_tot << ", N = " << n_ent << "\n"
        << "  Disabled: weight = " << w_dis << ", N = " << n_dis << "\n"
        << "                     " << w_dis/w_tot << ", " << 1.0*n_dis/n_ent << endl;
}
