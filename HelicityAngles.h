// Helicity angles (theta, phi) for B_s -> B0 + X in the B_s rest frame.
// Convention (agreed for Bs2 -> B0 K_S SDM + Eq. 14.6):
//   z_hat  = unit vector along B_s momentum in the lab (helicity axis).
//   x_hat  = unit vector along (e_beam x z_hat) [normal to the production plane
//            spanned by beam and p(B_s)]; phi = 0 along +x_hat.
//   y_hat  = z_hat x x_hat  (right-handed).
//   p*     = B0 3-momentum after Boost(-B_s) in ROOT: same Cartesian axes as lab.
//   cos(theta) = (p* . z_hat) / |p*|
//   phi        = atan2(p* . y_hat, p* . x_hat)
//
// Degeneracy: if e_beam is parallel to p(B_s), beam x z_hat vanishes — use fallback_axis.

#ifndef HELICITY_ANGLES_H
#define HELICITY_ANGLES_H

#include "TMath.h"
#include "TLorentzVector.h"
#include "TVector3.h"

struct HelicityAngles {
  double cosTheta{0.};
  double sinTheta{0.};
  double theta{0.};
  double phi{0.};
  bool ok{false};
};

inline HelicityAngles computeHelicityAnglesBsToB0(
    const TLorentzVector& Bs_lab,
    const TLorentzVector& B0_lab,
    const TVector3& e_beam = TVector3(0.0, 0.0, 1.0),
    const TVector3& fallback_axis = TVector3(0.0, 1.0, 0.0))
{
  HelicityAngles out;

// coordinace 1
  const TVector3 zHat = Bs_lab.Vect().Unit();

  TVector3 xHat = e_beam.Unit().Cross(zHat);
  if (xHat.Mag() <= 1.0e-12) {
    xHat = fallback_axis.Unit().Cross(zHat);
  }
  if (xHat.Mag() <= 1.0e-12) {
    return out; // still degenerate
  }
  xHat = xHat.Unit();

  const TVector3 yHat = zHat.Cross(xHat); // z x x = y (right-handed)



/* coordinace 2
  const TVector3 bsVec = Bs_lab.Vect();
  if (bsVec.Mag() <= 1.0e-12) return out;
  const TVector3 xHat = bsVec.Unit();

  TVector3 zHat = e_beam.Unit().Cross(xHat);
  if (zHat.Mag() <= 1.0e-12) {
    zHat = fallback_axis.Unit().Cross(xHat);
  }
  if (zHat.Mag() <= 1.0e-12) {
    return out; // still degenerate
  }
  zHat = zHat.Unit();

  const TVector3 yHat = zHat.Cross(xHat); 
*/

  TLorentzVector B0star = B0_lab;
  B0star.Boost(-Bs_lab.BoostVector());
  const TVector3 pStar = B0star.Vect();
  const double pMag = pStar.Mag();
  if (pMag <= 0.0) return out;

  out.cosTheta = pStar.Unit().Dot(zHat);
  out.sinTheta = std::sqrt(std::max(0.0, 1.0 - out.cosTheta * out.cosTheta));
  out.theta = std::acos(std::max(-1.0, std::min(1.0, out.cosTheta)));
  out.phi = std::atan2(pStar.Dot(yHat), pStar.Dot(xHat));
  out.ok = true;
  return out;
}

#endif
