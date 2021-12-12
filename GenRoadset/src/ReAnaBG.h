#ifndef _RE_ANA_FULL_BG__H_
#define _RE_ANA_FULL_BG__H_
#include "AnaBG.h"

class ReAnaBG : public AnaBG {
 protected:

  RoadMap  m_road_map_top;
  RoadMap  m_road_map_bot;

  const RoadMap* m_road_map_pos_top_on;
  const RoadMap* m_road_map_pos_bot_on;
  const RoadMap* m_road_map_neg_top_on;
  const RoadMap* m_road_map_neg_bot_on;

 public:
  ReAnaBG(const std::string label="re_ana_bg");
  virtual ~ReAnaBG();
  virtual void Init();
  virtual void End();
  virtual void Analyze();

  void SetEnabledRoads(const RoadMap* pos_top, const RoadMap* pos_bot, const RoadMap* neg_top, const RoadMap* neg_bot);

  RoadMap * GetRoadMapTop () { return &m_road_map_top ; }
  RoadMap * GetRoadMapBot () { return &m_road_map_bot ; }

 protected:
  virtual void ProcessOneEvent();
};

#endif // _RE_ANA_FULL_BG__H_
