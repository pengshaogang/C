#include "TChain.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TMath.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TString.h"
#include "TSystem.h"

#include <iostream>
#include <cmath>

#include "reader.h"

const double GeV = 1000.0;
Int_t isRun3 = -1;  // -1: all, 0: run2, 1: run3
Int_t use802395 = 0;
Float_t dR_B0Ks = 0.0;
Float_t tq_rf_m_cor;
const Double_t PDG_B0 = 5279.66; // MeV
const Double_t PDG_Ks = 497.611; // MeV

bool IsRun3Event() { return (run_number > 364485); }

bool PassPreSel()
{
  bool isOS = (trk_charge[0] * trk_charge[1] < 0);
  if(pt_mu1/GeV>4.0 && pt_mu2/GeV>4.0
     && pt_trk1_X/GeV>0.8 && pt_trk2_X/GeV>0.8
     && jpsi_m/GeV>2.96 && jpsi_m/GeV<3.22
     && isOS
     && x_rf_m/GeV>0.84 && x_rf_m/GeV<0.95
     && jx_m_calc/GeV>5.180 && jx_m_calc/GeV<5.375
     && tq_rf_fitChi2NDF<1.8
     && ld_m/GeV>0.475 && ld_m/GeV<0.520
     && tq_rf_ratio>0.20
     && tq_rf_m_cor/GeV<5.90
     && jx_Lxy>0.2 ) return true;
  return false;
}

Bool_t passFiducial() {
  if(cat==100) return true;
  if(cat==1) return truth_Bs1_pt/GeV>15 && fabs(truth_Bs1_eta)<2.5;
  else if(cat==2) return truth_Bs2_pt/GeV>15 && fabs(truth_Bs2_eta)<2.5;
  else return false;
}

Bool_t passFilter() {
  if(cat==100) return true;
  else if(cat==1 || cat==2) return truth_mu_pt[0]>=3400. && fabs(truth_mu_eta[0])<=2.6 && truth_mu_pt[1]>=3400. && fabs(truth_mu_eta[1])<=2.6 && truth_K_pt>=760. && fabs(truth_K_eta)<=2.6 && truth_pi1_pt>=760. && fabs(truth_pi1_eta)<=2.6;
  else return false;
}

// -----------------------------------------------------------------------------
// Fiducial TRUTH: fill cos(theta*) from truthTree (generator-level),
// per period (run2/run3) with the same fiducial definition as fiducial_cos.cc.
// -----------------------------------------------------------------------------
static Long64_t fillTruthCos(TChain* ch, TH1F* h, bool isRun3Period)
{
  static const int MAXTRUTH = 500;
  Int_t    truth_n_t = 0;
  Int_t    truth_pdgId_t [MAXTRUTH];
  Int_t    truth_motherID_t[MAXTRUTH];
  Double_t truth_mass_t  [MAXTRUTH];
  Double_t truth_pT_t    [MAXTRUTH];
  Double_t truth_eta_t   [MAXTRUTH];
  Double_t truth_phi_t   [MAXTRUTH];

  ch->SetBranchStatus("*", 1);
  ch->SetBranchAddress("truth_n",       &truth_n_t);
  ch->SetBranchAddress("truth_pdgId",    truth_pdgId_t);
  ch->SetBranchAddress("truth_motherID", truth_motherID_t);
  ch->SetBranchAddress("truth_mass",     truth_mass_t);
  ch->SetBranchAddress("truth_pT",       truth_pT_t);
  ch->SetBranchAddress("truth_eta",      truth_eta_t);
  ch->SetBranchAddress("truth_phi",      truth_phi_t);

  Long64_t nTruth = ch->GetEntries();
  std::cout << (isRun3Period ? "Truth (run3) entries = " : "Truth (run2) entries = ") << nTruth << std::endl;

  Long64_t nFid = 0;
  for(Long64_t i=0; i<nTruth; i++) {
    ch->GetEntry(i);

    int n = truth_n_t;
    if(n < 0) continue;
    if(n > MAXTRUTH) n = MAXTRUTH;

    // remove B*0 decay contributions (keep Bs2 high peak definition consistent)
    Bool_t isBstarDecay = false;
    for(Int_t k=0; k<n; k++) {
      if(std::abs(truth_pdgId_t[k]) == 513 && truth_motherID_t[k] == 35) {
        isBstarDecay = true;
        break;
      }
    }
    if(isBstarDecay) continue;

    Int_t iBs2 = -1, iB0 = -1;
    for(Int_t k=0; k<n; k++) {
      if(truth_pdgId_t[k] == 35 && iBs2 < 0) iBs2 = k;
      if(std::abs(truth_pdgId_t[k]) == 511 && truth_motherID_t[k] == 35 && iB0 < 0) iB0 = k;
    }
    if(iBs2 < 0 || iB0 < 0) continue;

    // fiducial definition
    if(!(truth_pT_t[iBs2]/GeV > 15.0 && fabs(truth_eta_t[iBs2]) < 2.5)) continue;

    // truth-pt (and eta for run2) weights at fiducial level
    float wt = 1.0;
    if(isRun3Period) {
      wt *= exp(1.36741 - 0.0539602*truth_pT_t[iBs2]/GeV);
    } else {
      wt *= exp(1.5716  - 0.0640473*truth_pT_t[iBs2]/GeV);
      wt *= 1.30932 - 0.264317*fabs(truth_eta_t[iBs2]);
    }

    TLorentzVector Bs2_truth, B0_truth;
    Bs2_truth.SetPtEtaPhiM(truth_pT_t[iBs2], truth_eta_t[iBs2], truth_phi_t[iBs2], truth_mass_t[iBs2]);
    B0_truth .SetPtEtaPhiM(truth_pT_t[iB0],  truth_eta_t[iB0],  truth_phi_t[iB0],  truth_mass_t[iB0]);
    TVector3 bv_truth = -Bs2_truth.BoostVector();
    B0_truth.Boost(bv_truth);
    const double cos_truth = Bs2_truth.Vect().Unit().Dot(B0_truth.Vect().Unit());

    h->Fill(cos_truth, wt);
    ++nFid;
  }
  return nFid;
}

void plot()
{
  // Make file paths robust to current working directory.
  // This macro lives in .../xingnaiwen/test/mix3/, so:
  //   ntuples live in        <thisDir>/ntuple/
  //   truth ntuples live in  <thisDir>/../nofilter/
  const TString thisDir   = gSystem->DirName(__FILE__);
  const TString testDir   = gSystem->DirName(thisDir);
  const TString ntupleDir = thisDir + "/ntuple";
  const TString truthDir  = testDir + "/nofilter";

  // ------------------------------------------------------------
  // reco MC/data chains
  // ------------------------------------------------------------
  TChain* ch[2] = {nullptr, nullptr};
  ch[0] = new TChain("BPHY25"); // MC reco

  // MC (non-baseline)
  if(use802395) { // sys
    if(isRun3==-1 || isRun3==0) {
      ch[0]->Add(ntupleDir + "/mc20a_Bs2_802395_baseline.root");
      ch[0]->Add(ntupleDir + "/mc20a_Bs2_802396_baseline.root");
      ch[0]->Add(ntupleDir + "/mc20d_Bs2_802395_baseline.root");
      ch[0]->Add(ntupleDir + "/mc20d_Bs2_802396_baseline.root");
      ch[0]->Add(ntupleDir + "/mc20e_Bs2_802395_baseline.root");
      ch[0]->Add(ntupleDir + "/mc20e_Bs2_802396_baseline.root");
    }
    if(isRun3==-1 || isRun3==1) {
      ch[0]->Add(ntupleDir + "/mc23a_Bs2_802395_baseline.root");
      ch[0]->Add(ntupleDir + "/mc23a_Bs2_802396_baseline.root");
      ch[0]->Add(ntupleDir + "/mc23d_Bs2_802395_baseline.root");
      ch[0]->Add(ntupleDir + "/mc23d_Bs2_802396_baseline.root");
      ch[0]->Add(ntupleDir + "/mc23e_Bs2_802395_baseline.root");
      ch[0]->Add(ntupleDir + "/mc23e_Bs2_802396_baseline.root");
    }
  } else { // nominal
    if(isRun3==-1 || isRun3==0) {
      ch[0]->Add(ntupleDir + "/mc20a_Bs2_802829_baseline.root");
      ch[0]->Add(ntupleDir + "/mc20a_Bs2_802830_baseline.root");
      ch[0]->Add(ntupleDir + "/mc20d_Bs2_802829_baseline.root");
      ch[0]->Add(ntupleDir + "/mc20d_Bs2_802830_baseline.root");
      ch[0]->Add(ntupleDir + "/mc20e_Bs2_802829_baseline.root");
      ch[0]->Add(ntupleDir + "/mc20e_Bs2_802830_baseline.root");
    }
    if(isRun3==-1 || isRun3==1) {
      ch[0]->Add(ntupleDir + "/mc23a_Bs2_802829_baseline.root");
      ch[0]->Add(ntupleDir + "/mc23a_Bs2_802830_baseline.root");
      ch[0]->Add(ntupleDir + "/mc23d_Bs2_802829_baseline.root");
      ch[0]->Add(ntupleDir + "/mc23d_Bs2_802830_baseline.root");
      ch[0]->Add(ntupleDir + "/mc23e_Bs2_802829_baseline.root");
      ch[0]->Add(ntupleDir + "/mc23e_Bs2_802830_baseline.root");
    }
  }
  Init(ch[0]);

  // data (non-baseline)
  ch[1] = new TChain("BPHY25");
  if(isRun3==-1 || isRun3==0) {
    ch[1]->Add(ntupleDir + "/data15_Main_baseline.root");
    ch[1]->Add(ntupleDir + "/data16_All_baseline.root");
    ch[1]->Add(ntupleDir + "/data17_All_baseline.root");
    ch[1]->Add(ntupleDir + "/data18_All_baseline.root");
  }
  if(isRun3==-1 || isRun3==1) {
    ch[1]->Add(ntupleDir + "/data22_BphysDelayed_baseline.root");
    ch[1]->Add(ntupleDir + "/data23_BphysDelayed_baseline.root");
    ch[1]->Add(ntupleDir + "/data24_BphysDelayed_baseline.root");
  }
  Init(ch[1]);

  // ------------------------------------------------------------
  // truth chains for fiducial denominator (run2/run3 split)
  // ------------------------------------------------------------
  TChain* chTruth_r2 = new TChain("truthTree");
  TChain* chTruth_r3 = new TChain("truthTree");
  chTruth_r2->Add(truthDir + "/mc20_802829/mytruth.root.1");
  chTruth_r2->Add(truthDir + "/mc20_802829/mytruth.root.2");
  chTruth_r2->Add(truthDir + "/mc20_802829/mytruth.root.3");
  chTruth_r2->Add(truthDir + "/mc20_802829/mytruth.root.4");
  chTruth_r3->Add(truthDir + "/mc23_802829/mytruth.root.1");
  chTruth_r3->Add(truthDir + "/mc23_802829/mytruth.root.2");

  const int nBins = 20;
  const double cosMin = -1.0;
  const double cosMax = 1.0;
  const double binW = (cosMax - cosMin) / nBins;
  TString yTitle = Form("Events / %.2f", binW);

  // reco (numerator) & truth (denominator) per period
  const char* tag[2] = {"run2","run3"};
  TH1F* h_reco [2];
  TH1F* h_truth[2];
  for(int i=0;i<2;i++) {
    h_reco[i]  = new TH1F(Form("h_cos_reco_%s",  tag[i]), Form(";cos#theta;%s", yTitle.Data()), nBins, cosMin, cosMax);
    h_truth[i] = new TH1F(Form("h_cos_truth_%s", tag[i]), Form(";cos#theta;%s", yTitle.Data()), nBins, cosMin, cosMax);
    h_reco[i]->Sumw2();
    h_truth[i]->Sumw2();
  }

  // data: raw and unfolded (fiducial) peak/sideband + signal
  TH1F* h_data_peak     = new TH1F("h_cos_peak_data",     Form(";cos#theta;%s", yTitle.Data()), nBins, cosMin, cosMax);
  TH1F* h_data_bkg      = new TH1F("h_cos_sideband_bkg",  Form(";cos#theta;%s", yTitle.Data()), nBins, cosMin, cosMax);
  TH1F* h_data_peak_unf = new TH1F("h_cos_peak_data_unf", Form(";cos#theta;%s", yTitle.Data()), nBins, cosMin, cosMax);
  TH1F* h_data_bkg_unf  = new TH1F("h_cos_sideband_bkg_unf", Form(";cos#theta;%s", yTitle.Data()), nBins, cosMin, cosMax);
  h_data_peak->Sumw2();
  h_data_bkg->Sumw2();
  h_data_peak_unf->Sumw2();
  h_data_bkg_unf->Sumw2();

  // --------------------------------------------------------------------------
  // 1) Fill reco MC numerator (run2/run3 split) for Bs2 high peak (idx==2)
  //    This is preselection-level selection.
  // 2) Fill truth fiducial denominator from truthTree.
  // 3) Compute eff[period] = reco / truth and use it to unfold data.
  // --------------------------------------------------------------------------

  for(Int_t ich=0; ich<2; ich++) {
    if(!ch[ich]) continue;
    Long64_t nentries = ch[ich]->GetEntries();
    printf("%llu entries to be processed...\n",nentries);
    for(Long64_t entry=0; entry<nentries; entry++) {
      ch[ich]->GetEntry(entry);
      if(isRun3==0 && IsRun3Event())  continue;
      if(isRun3==1 && !IsRun3Event()) continue;

      {
        TLorentzVector tmpJX, tmpKs;
        tmpJX.SetPtEtaPhiM(jx_rf_pt, jx_rf_eta, jx_rf_phi, PDG_B0);
        tmpKs.SetPtEtaPhiM(ld_rf_pt, ld_rf_eta, ld_rf_phi, PDG_Ks);
        dR_B0Ks = tmpJX.DeltaR(tmpKs);
        tq_rf_m_cor = (tmpJX + tmpKs).M();
      }

      if(!passFiducial()) continue;
      if(!passFilter()) continue;
      if(!PassPreSel()) continue;

      Int_t idx = 0; // Bs2 high peak
      if(ich==0) {
        if(truth_isBstarDecay) idx = 1; // Bs2 low peak
        else idx = 2; // Bs2 high peak
      } else if(ich==1) {
        idx = 3;
      } else continue;

      const bool isRun2 = (run_number<=364485);
      const int  ip = isRun2 ? 0 : 1;

      // signal region
      if(idx<=3 && (tq_rf_m_cor/GeV>5.832 && tq_rf_m_cor/GeV<5.848)) {

        Float_t wt = 1.0;
        if(cat==1 || cat==2) {
          Float_t tmp1 = cat==1 ? truth_Bs1_pt/GeV : truth_Bs2_pt/GeV;
          Float_t tmp2 = cat==1 ? truth_Bs1_eta : truth_Bs2_eta;
          if(run_number<=364485) {
            wt *= exp(1.5716-0.0640473*tmp1);
            wt *= 1.30932-0.264317*fabs(tmp2);
          } else {
            wt *= exp(1.36741-0.0539602*tmp1);
          }
        }

        wt *= trig_wt*pu_wt;
        if(run_number>=431810) {
          wt *= mu_pt[0]/GeV>5 ? mu_medium_eff_sf[0] : mu_lowpt_eff_sf[0];
          wt *= mu_pt[1]/GeV>5 ? mu_medium_eff_sf[1] : mu_lowpt_eff_sf[1];
        } else {
          wt *= mu_medium_eff_sf[0]*mu_medium_eff_sf[1];
        }

        if(idx<=2) { // signal MC weights

          if(idx==0)      wt *= run_number<=364485 ? 0.0185 : 0.0140;
          else if(idx==1) {
            if(use802395) wt *= run_number<=364485 ? 0.0480 : 0.0460;
            else          wt *= run_number<=364485 ? 0.0085 : 0.0072;
          } else if(idx==2) {
            if(use802395) wt *= run_number<=364485 ? 0.0660 : 0.0419;
            else          wt *= run_number<=364485 ? 0.0547 : 0.0363;
          }

          Float_t tmp;
          if(run_number<=364485) {
            tmp = pt_mu2/GeV;
            if(tmp<6) wt *= 0.907558;
            else wt *= 1.03618;
          } else {
            tmp = pt_mu2/GeV;
            if(tmp>15) tmp = 15;
            wt *= (0.707543+0.0225414*tmp+0.00132893*pow(tmp,2))*1.02651;
            tmp = pt_mu1/GeV;
            if(tmp>20) tmp = 20;
            wt *= (0.475828+0.0402479*tmp)*1;
            tmp = tq_rf_eta;
            if(fabs(tmp)<0.8) wt *= (0.829035+0.0468434*tmp-0.382025*pow(tmp,2))*1.08259;
            else wt *= 1.20288;
          }

          tmp = tq_rf_ratio;
          if(tmp<0.2) tmp = 0.2; if(tmp>0.8) tmp = 0.8;
          wt *= (2.98648-5.31825*tmp+2.11922*pow(tmp,2))*1.00817;

          tmp = tq_rf_fitChi2NDF;
          if(tmp>1.8) tmp = 1.8;
          wt *= (0.734235+0.28194*tmp)*1.02518;

          tmp = pt_LRT_trk2/GeV;
          if(tmp>2) tmp = 2;
          if(tmp<0.9) wt *= (-7.34283+25.8847*tmp-19.553*pow(tmp,2))*1.01762;
          else wt *= (1.48474-1.55385*tmp+1.18987*pow(tmp,2)-0.233051*pow(tmp,3))*1.01762;
/*
          if(idx==2) {
            tmp = dR_B0Ks;
            if(tmp>0.15) tmp = 0.15;
            wt *= (2.29929-30.8004*tmp+147.357*pow(tmp,2));
          }
*/
        }

        if(idx==2) {
          TLorentzVector Bs2_reco, B0_reco;
          Bs2_reco.SetPtEtaPhiM(tq_rf_pt, tq_rf_eta, tq_rf_phi, tq_rf_m_cor);
          B0_reco.SetPtEtaPhiM(jx_rf_pt, jx_rf_eta, jx_rf_phi, PDG_B0);
          TVector3 bv_reco = -Bs2_reco.BoostVector();
          B0_reco.Boost(bv_reco);
          const double cos_reco = Bs2_reco.Vect().Unit().Dot(B0_reco.Vect().Unit());
          h_reco[ip]->Fill(cos_reco, wt);
        }

        if(idx==3) {
          TLorentzVector Bs2_data, B0_data;
          Bs2_data.SetPtEtaPhiM(tq_rf_pt, tq_rf_eta, tq_rf_phi, tq_rf_m_cor);
          B0_data.SetPtEtaPhiM(jx_rf_pt, jx_rf_eta, jx_rf_phi, PDG_B0);
          TVector3 bv_data = -Bs2_data.BoostVector();
          B0_data.Boost(bv_data);
          const double cos_data = Bs2_data.Vect().Unit().Dot(B0_data.Vect().Unit());
          h_data_peak->Fill(cos_data, wt);
        }
      }

      // sideband template for background (data only)
      if(idx==3
         && !(tq_rf_m_cor/GeV>5.832 && tq_rf_m_cor/GeV<5.848)
         && !(tq_rf_m_cor/GeV<5.80)) {
        Float_t wt = run_number<=364485 ? 0.194 : 0.187;
        TLorentzVector Bs2_data, B0_data;
        Bs2_data.SetPtEtaPhiM(tq_rf_pt, tq_rf_eta, tq_rf_phi, tq_rf_m_cor);
        B0_data.SetPtEtaPhiM(jx_rf_pt, jx_rf_eta, jx_rf_phi, PDG_B0);
        TVector3 bv_data = -Bs2_data.BoostVector();
        B0_data.Boost(bv_data);
        const double cos_bkg = Bs2_data.Vect().Unit().Dot(B0_data.Vect().Unit());
        h_data_bkg->Fill(cos_bkg, wt);
      }
    }
  }

  // Fill fiducial truth denominator (run2/run3)
  fillTruthCos(chTruth_r2, h_truth[0], /*isRun3Period=*/false);
  fillTruthCos(chTruth_r3, h_truth[1], /*isRun3Period=*/true);

  // Efficiency per period
  TH1F* h_eff[2];
  for(int i=0;i<2;i++) {
    h_eff[i] = (TH1F*)h_reco[i]->Clone(Form("h_eff_%s", tag[i]));
    h_eff[i]->SetTitle(Form(";cos#theta;Efficiency (%s)", tag[i]));
    h_eff[i]->Divide(h_truth[i]);
    for(int ib=1; ib<=h_eff[i]->GetNbinsX(); ++ib) {
      if(h_truth[i]->GetBinContent(ib) <= 0.0) {
        h_eff[i]->SetBinContent(ib, 0.0);
        h_eff[i]->SetBinError(ib, 0.0);
      }
    }
  }

  // Unfold data (peak and sideband) to fiducial level using 1/eff(period,bin)
  Long64_t nDropEffZero = 0;
  for(Int_t ich=1; ich<=1; ++ich) {
    if(!ch[ich]) continue;
    Long64_t nentries = ch[ich]->GetEntries();
    for(Long64_t entry=0; entry<nentries; entry++) {
      ch[ich]->GetEntry(entry);
      if(isRun3==0 && IsRun3Event())  continue;
      if(isRun3==1 && !IsRun3Event()) continue;

      {
        TLorentzVector tmpJX, tmpKs;
        tmpJX.SetPtEtaPhiM(jx_rf_pt, jx_rf_eta, jx_rf_phi, PDG_B0);
        tmpKs.SetPtEtaPhiM(ld_rf_pt, ld_rf_eta, ld_rf_phi, PDG_Ks);
        tq_rf_m_cor = (tmpJX + tmpKs).M();
      }

      if(!PassPreSel()) continue;

      const bool isRun2 = (run_number<=364485);
      const int  ip     = isRun2 ? 0 : 1;

      TLorentzVector Bs2_data, B0_data;
      Bs2_data.SetPtEtaPhiM(tq_rf_pt, tq_rf_eta, tq_rf_phi, tq_rf_m_cor);
      B0_data.SetPtEtaPhiM(jx_rf_pt, jx_rf_eta, jx_rf_phi, PDG_B0);
      TVector3 bv_data = -Bs2_data.BoostVector();
      B0_data.Boost(bv_data);
      const double cos_data = Bs2_data.Vect().Unit().Dot(B0_data.Vect().Unit());

      const double m_cor = tq_rf_m_cor/GeV;
      const bool inPeak     = (m_cor>5.832 && m_cor<5.848);
      const bool inSideband = (!inPeak && m_cor>5.80);
      if(!inPeak && !inSideband) continue;

      const int    bin    = h_eff[ip]->FindBin(cos_data);
      const double effVal = h_eff[ip]->GetBinContent(bin);
      if(effVal <= 0.0) { ++nDropEffZero; continue; }
      const double invEff = 1.0/effVal;

      if(inPeak) {
        h_data_peak_unf->Fill(cos_data, invEff);
      } else {
        const double wsb = isRun2 ? 0.194 : 0.187;
        h_data_bkg_unf->Fill(cos_data, wsb*invEff);
      }
    }
  }
  if(nDropEffZero>0) {
    std::cout << "[warning] dropped " << nDropEffZero << " data events because eff[period][bin] <= 0\n";
  }

  // Signal (raw and unfolded-to-fiducial)
  TH1F* h_cos_signal_unf = (TH1F*)h_data_peak_unf->Clone("h_cos_signal_subtracted_unf");
  h_cos_signal_unf->SetTitle(Form(";%s;%s", "cos#theta", yTitle.Data()));
  h_cos_signal_unf->Add(h_data_bkg_unf, -1.0);

  // Canvas 1: efficiency overlay (run2 vs run3)
  TCanvas* c_eff = new TCanvas("c_eff_Bs2B0cos_sideband", "efficiency", 900, 700);
  gStyle->SetEndErrorSize(2);
  h_eff[0]->SetMarkerStyle(20);
  h_eff[0]->SetMarkerColor(kBlue+1);
  h_eff[0]->SetLineColor(kBlue+1);
  h_eff[0]->SetLineWidth(2);
  h_eff[1]->SetMarkerStyle(21);
  h_eff[1]->SetMarkerColor(kRed+1);
  h_eff[1]->SetLineColor(kRed+1);
  h_eff[1]->SetLineWidth(2);

  const double effmax = TMath::Max(h_eff[0]->GetMaximum(), h_eff[1]->GetMaximum());
  h_eff[0]->SetMinimum(0.0);
  h_eff[0]->SetMaximum(effmax > 0 ? 1.40*effmax : 1.0);
  h_eff[0]->Draw("E1");
  h_eff[1]->Draw("E1 SAME");

  TLegend* lg_eff = new TLegend(0.65,0.74,0.88,0.88);
  lg_eff->SetBorderSize(0);
  lg_eff->SetFillStyle(0);
  lg_eff->AddEntry(h_eff[0], "run2 efficiency", "PE");
  lg_eff->AddEntry(h_eff[1], "run3 efficiency", "PE");
  lg_eff->Draw();
  c_eff->SaveAs("plot_Bs2B0cos_sideband_efficiency.png");
  std::cout << "Saved: plot_Bs2B0cos_sideband_efficiency.png" << std::endl;

  // Canvas 2: unfolded-to-fiducial (peak/sideband/signal)
  TCanvas* c3 = new TCanvas("c_fid_Bs2B0cos_sideband", "fiducial: unfolded data", 900, 700);
  const double ymax3 = TMath::Max(h_data_peak_unf->GetMaximum(),
                                 TMath::Max(h_data_bkg_unf->GetMaximum(), h_cos_signal_unf->GetMaximum()));
  h_data_peak_unf->SetMaximum(ymax3 > 0 ? 1.20 * ymax3 : 1.0);
  h_data_peak_unf->SetMinimum(0.0);
  h_data_peak_unf->SetMarkerStyle(20);
  h_data_peak_unf->SetMarkerColor(kBlack);
  h_data_peak_unf->SetLineColor(kBlack);
  h_data_peak_unf->SetLineWidth(2);
  h_data_peak_unf->GetYaxis()->SetTitle("Unfolded events (fiducial)");
  h_data_peak_unf->Draw("E1");

  h_data_bkg_unf->SetLineColor(kBlue+1);
  h_data_bkg_unf->SetLineWidth(2);
  h_data_bkg_unf->Draw("HIST SAME");

  h_cos_signal_unf->SetMarkerStyle(24);
  h_cos_signal_unf->SetMarkerColor(kRed+1);
  h_cos_signal_unf->SetLineColor(kRed+1);
  h_cos_signal_unf->SetLineWidth(2);
  h_cos_signal_unf->Draw("E1 SAME");

  TLegend* lg3 = new TLegend(0.50, 0.70, 0.88, 0.88);
  lg3->SetBorderSize(0);
  lg3->SetFillStyle(0);
  lg3->AddEntry(h_data_peak_unf, "Peak / #varepsilon", "PE");
  lg3->AddEntry(h_data_bkg_unf,  "Sideband / #varepsilon", "L");
  lg3->AddEntry(h_cos_signal_unf,"(Peak - sideband) / #varepsilon", "PE");
  lg3->Draw();
  c3->SaveAs("plot_Bs2B0cos_sideband_fiducial_unfolded.png");
  std::cout << "Saved: plot_Bs2B0cos_sideband_fiducial_unfolded.png" << std::endl;
}

// ROOT convenience entrypoint: allow `root -q plot_Bs2B0cos_sideband.cc`
void plot_Bs2B0cos_sideband()
{
  plot();
}



