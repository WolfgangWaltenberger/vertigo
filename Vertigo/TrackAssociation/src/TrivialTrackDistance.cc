#include "Vertigo/TrackAssociation/interface/TrivialTrackDistance.h"
#include "Environment/Configuration/interface/SimpleConfigurable.h"

float 
TrivialTrackDistance::distance(const vertigo::SimTrack& sim, const rave::Track& rec) const
{
  const float p_sim = sim.state().momentum().mag();
  const float p_rec = rec.state().momentum().mag();
  float dist = fabs((p_rec-p_sim)/p_sim);
  static float max = SimpleConfigurable<float> (0.02, "TrivialTrackDistance:Max").value();
  return dist < max ? dist : -1.; // if dist < 0 the tracks are considered not associated
}
