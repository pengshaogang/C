#include "TChain.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TMath.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TLatex.h"
#include "TLine.h"
#include "TPad.h"
#include "TStyle.h"

#include <iostream>
#include <cmath>

#include "reader.h"

const double GeV = 1000.0;
const Double_t PDG_B0 = 5279.66; // MeV
const Double_t PDG_Ks = 497.611; // MeV

Float_t dR_B0Ks = 0.0;
Float_t tq_rf_m_cor = 0.0;

// helper: per-period truth filling. Returns # of fiducial events filled.
static Long64_t fillTruthCos(TChain* ch, TH1F* h, bool isRun3)
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
  std::cout << (isRun3 ? "Truth (run3) entries = " : "Truth (run2) entries = ") << nTruth << std::endl;

  Long64_t nFid = 0;
  for(Long64_t i=0; i<nTruth; i++) {
    ch->GetEntry(i);

    int n = truth_n_t;
    if(n < 0) continue;
    if(n > MAXTRUTH) n = MAXTRUTH;

    // require non-B*0 decay (Bs2 high peak)
    Bool_t isBstarDecay = false;
    for(Int_t k=0; k<n; k++) {
      if(std::abs(truth_pdgId_t[k]) == 513 && truth_motherID_t[k] == 35) {
        isBstarDecay = true;
        break;
      }
    }
    if(isBstarDecay) continue;

    // find Bs2 (pdgId=35) and the B0 (|pdgId|=511) daughter directly from Bs2 (motherID=35)
    Int_t iBs2 = -1, iB0 = -1;
    for(Int_t k=0; k<n; k++) {
      if(truth_pdgId_t[k] == 35 && iBs2 < 0) iBs2 = k;
      if(std::abs(truth_pdgId_t[k]) == 511 && truth_motherID_t[k] == 35 && iB0 < 0) iB0 = k;
    }
    if(iBs2 < 0 || iB0 < 0) continue;

    // fiducial cut: pT(Bs2) > 15 GeV, |eta(Bs2)| < 2.5
    if(!(truth_pT_t[iBs2]/GeV > 15 && fabs(truth_eta_t[iBs2]) < 2.5)) continue;

    // truth-pt re-weight at fiducial level (period-dependent)
    float wt = 1.0;
    if(isRun3) {
      wt *= exp(1.09007 - 0.0433953*truth_pT_t[iBs2]/GeV);
    } else {
      // run2 fiducial truth-pt weight (matches plot_Bs2B0cos.cc convention)
      wt *= exp(1.5716 - 0.0640473*truth_pT_t[iBs2]/GeV);
      wt *= 1.30932 - 0.264317*fabs(truth_eta_t[iBs2]);
    }

    // cos(theta*) at truth level
    TLorentzVector Bs2_truth, B0_truth;
    Bs2_truth.SetPtEtaPhiM(truth_pT_t[iBs2], truth_eta_t[iBs2], truth_phi_t[iBs2], truth_mass_t[iBs2]);
    B0_truth .SetPtEtaPhiM(truth_pT_t[iB0],  truth_eta_t[iB0],  truth_phi_t[iB0],  truth_mass_t[iB0]);
    TVector3 bv_truth = -Bs2_truth.BoostVector();
    B0_truth.Boost(bv_truth);
    double cosxita_truth = Bs2_truth.Vect().Unit().Dot(B0_truth.Vect().Unit());

    h->Fill(cosxita_truth, wt);
    ++nFid;
  }
  return nFid;
}

void plot()
{
  // ======================
  // chains
  // ======================
  TChain* chMC       = new TChain("BPHY25");
  TChain* chData     = new TChain("BPHY25");
  TChain* chTruth_r2 = new TChain("truthTree"); // MC20 truth
  TChain* chTruth_r3 = new TChain("truthTree"); // MC23 truth

  // --- MC reco (run2 + run3, baseline)
  chMC->Add("../../../../mix3/ntuple/mc20a_Bs2_802829_baseline.root");
  chMC->Add("../../../../mix3/ntuple/mc20d_Bs2_802829_baseline.root");
  chMC->Add("../../../../mix3/ntuple/mc20e_Bs2_802829_baseline.root");
  chMC->Add("../../../../mix3/ntuple/mc23a_Bs2_802829_baseline.root");
  chMC->Add("../../../../mix3/ntuple/mc23d_Bs2_802829_baseline.root");
  chMC->Add("../../../../mix3/ntuple/mc23e_Bs2_802829_baseline.root");

  // --- truth ntuples (fiducial)  -- one chain per run period
  // run2 truth (MC20)  -- adjust the file paths if your MC20 truth has different names
  chTruth_r2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_nofilter/all/mc20_802829/mytruth.root.1");
  chTruth_r2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_nofilter/all/mc20_802829/mytruth.root.2");
  chTruth_r2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_nofilter/all/mc20_802829/mytruth.root.3");
  chTruth_r2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_nofilter/all/mc20_802829/mytruth.root.4");
  // run3 truth (MC23)
  chTruth_r3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_nofilter/all/mc23_802829/mytruth.root.1");
  chTruth_r3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_nofilter/all/mc23_802829/mytruth.root.2");
  //chTruth_r3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_nofilter/all/mc23_802829/mytruth.root.3");
  //chTruth_r3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_nofilter/all/mc23_802829/mytruth.root.4");

  // --- data (run2 + run3, baseline) ---
  chData->Add("../../../../mix3/ntuple/data15_Main_baseline.root");
  chData->Add("../../../../mix3/ntuple/data16_All_baseline.root");
  chData->Add("../../../../mix3/ntuple/data17_All_baseline.root");
  chData->Add("../../../../mix3/ntuple/data18_All_baseline.root");
  chData->Add("../../../../mix3/ntuple/data22_BphysDelayed_baseline.root");
  chData->Add("../../../../mix3/ntuple/data23_BphysDelayed_baseline.root");
  chData->Add("../../../../mix3/ntuple/data24_BphysDelayed_baseline.root");

  // ======================
  // histograms: cos(theta*)  -- one set per run period (i=0:run2, i=1:run3)
  // ======================
  const int    nBins  = 20;
  const double cosMin = -1.0;
  const double cosMax =  1.0;
  const double binW   = (cosMax - cosMin) / nBins;
  TString yTitle = Form("Events / %.2f", binW);
  const char* tag[2] = {"run2","run3"};

  // MC histograms split per run period (because the efficiency must be per period)
  TH1F* h_sig   [2];
  TH1F* h_truth [2];
  for(int i=0;i<2;i++) {
    h_sig  [i] = new TH1F(Form("h_sig_%s",   tag[i]), Form(";cos#theta;%s", yTitle.Data()), nBins, cosMin, cosMax);
    h_truth[i] = new TH1F(Form("h_truth_%s", tag[i]), Form(";cos#theta;%s", yTitle.Data()), nBins, cosMin, cosMax);
    h_sig  [i]->Sumw2();
    h_truth[i]->Sumw2();
  }

  // data : single chain, single pair of histograms; period is decided per event from run_number
  TH1F* h_data_peak     = new TH1F("h_data_peak",     Form(";cos#theta;%s", yTitle.Data()), nBins, cosMin, cosMax); // raw counts (pre-unfold)
  TH1F* h_data_bkg      = new TH1F("h_data_bkg",      Form(";cos#theta;%s", yTitle.Data()), nBins, cosMin, cosMax); // raw sideband (pre-unfold)
  TH1F* h_data_peak_unf = new TH1F("h_data_peak_unf", Form(";cos#theta;%s", yTitle.Data()), nBins, cosMin, cosMax); // peak     / eff[period]
  TH1F* h_data_bkg_unf  = new TH1F("h_data_bkg_unf",  Form(";cos#theta;%s", yTitle.Data()), nBins, cosMin, cosMax); // sideband / eff[period]
  h_data_peak    ->Sumw2();
  h_data_bkg     ->Sumw2();
  h_data_peak_unf->Sumw2();
  h_data_bkg_unf ->Sumw2();

  // ======================
  // MC reco (preselection)  -- fills h_sig[0] (run2) and h_sig[1] (run3)
  // ======================
  Init(chMC);
  Long64_t nMC = chMC->GetEntries();
  std::cout << "MC entries = " << nMC << std::endl;

  for(Long64_t i=0; i<nMC; i++) {
    chMC->GetEntry(i);

    bool isOS = trk_charge[0]*trk_charge[1] < 0;

    if(!( pt_mu1/GeV>4.0 && pt_mu2/GeV>4.0
          && pt_trk1_X/GeV>0.8 && pt_trk2_X/GeV>0.8
          && jpsi_m/GeV>2.96 && jpsi_m/GeV<3.22
          && isOS && x_rf_m/GeV>0.84 && x_rf_m/GeV<0.95
          && jx_m_calc/GeV>5.180 && jx_m_calc/GeV<5.375 && tq_rf_fitChi2NDF<1.8
          && ld_m/GeV>0.475 && ld_m/GeV<0.520 && tq_rf_ratio>0.20
          && tq_rf_m/GeV<5.90 && jx_Lxy>0.2 && tq_rf_pt/GeV>15 )) continue;

    int idx = truth_isBstarDecay ? 1 : 2;
    if(idx != 2) continue; // keep Bs2 high peak only

    const bool isRun2 = (run_number<=364485);
    const int  ip     = isRun2 ? 0 : 1;

    // ---- weights ----
    Float_t wt = trig_wt*pu_wt;
    // truth-pt (and eta for run2) re-weight at fiducial level
    if(isRun2) {
      wt *= exp(1.5716 - 0.0640473*truth_Bs2_pt/GeV);
      wt *= 1.30932 - 0.264317*fabs(truth_Bs2_eta);
    } else {
      wt *= exp(1.42096 - 0.0570117*truth_Bs2_pt/GeV);
    }

    if(run_number>=431810) {
      wt *= mu_pt[0]/GeV>5 ? mu_medium_eff_sf[0] : mu_lowpt_eff_sf[0];
      wt *= mu_pt[1]/GeV>5 ? mu_medium_eff_sf[1] : mu_lowpt_eff_sf[1];
    } else {
      wt *= mu_medium_eff_sf[0]*mu_medium_eff_sf[1];
    }

    //wt *= isRun2 ? 0.0207 : 0.0169; // signal MC normalization

    Float_t tmp;
    if(isRun2) {
      tmp = pt_mu2/GeV;
      if(tmp<6) wt *= 0.907558;
      else      wt *= 1.03618;
    } else {
      tmp = pt_mu2/GeV;
      if(tmp>15) tmp = 15;
      wt *= (0.707543+0.0225414*tmp+0.00132893*pow(tmp,2))*1.02651;

      tmp = pt_mu1/GeV;
      if(tmp>20) tmp = 20;
      wt *= (0.475828+0.0402479*tmp)*1;

      tmp = tq_rf_eta;
      if(fabs(tmp)<0.8) wt *= (0.829035+0.0468434*tmp-0.382025*pow(tmp,2))*1.08259;
      else              wt *= 1.20288;
    }

    tmp = tq_rf_ratio;
    if(tmp<0.2)  tmp = 0.2;
    if(tmp>0.75) tmp = 0.75;
    wt *= (2.4395-3.08002*tmp)*1.00890;

    tmp = tq_rf_fitChi2NDF;
    if(tmp>1.8) tmp = 1.8;
    wt *= (0.734235+0.28194*tmp)*1.02518;

    tmp = pt_LRT_trk2/GeV;
    if(tmp>2) tmp = 2;
    if(tmp<0.9) wt *= (-7.34283+25.8847*tmp-19.553*pow(tmp,2))*1.01762;
    else        wt *= (1.48474-1.55385*tmp+1.18987*pow(tmp,2)-0.233051*pow(tmp,3))*1.01762;

    // dR(B0, Ks) and corrected Bs2 mass (B0 set to PDG mass)
    TLorentzVector tmpJX, tmpKs;
    tmpJX.SetPtEtaPhiM(jx_rf_pt, jx_rf_eta, jx_rf_phi, PDG_B0);
    tmpKs.SetPtEtaPhiM(ld_rf_pt, ld_rf_eta, ld_rf_phi, PDG_Ks);
    dR_B0Ks     = tmpJX.DeltaR(tmpKs);
    tq_rf_m_cor = (tmpJX + tmpKs).M();

    // cos(theta*) between Bs2 (lab) and B0 (Bs2 rest frame)
    TLorentzVector Bs2_reco, B0_reco;
    Bs2_reco.SetPtEtaPhiM(tq_rf_pt, tq_rf_eta, tq_rf_phi, tq_rf_m_cor);
    B0_reco .SetPtEtaPhiM(jx_rf_pt, jx_rf_eta, jx_rf_phi, PDG_B0);
    TVector3 bv_reco = -Bs2_reco.BoostVector();
    B0_reco.Boost(bv_reco);
    double cosxita_reco = Bs2_reco.Vect().Unit().Dot(B0_reco.Vect().Unit());
    h_sig[ip]->Fill(cosxita_reco, wt);
  }

  // ======================
  // TRUTH (fiducial)  -- per period
  // ======================
  fillTruthCos(chTruth_r2, h_truth[0], /*isRun3=*/false);
  fillTruthCos(chTruth_r3, h_truth[1], /*isRun3=*/true);

  // ======================
  // EFFICIENCY (per period, bin-by-bin) -- computed BEFORE the data loop so we can
  // apply it event-by-event to data
  // ======================
  TH1F* h_eff[2];
  for(int i=0;i<2;i++) {
    h_eff[i] = (TH1F*)h_sig[i]->Clone(Form("h_eff_%s", tag[i]));
    h_eff[i]->SetTitle(Form(";cos#theta;Efficiency (%s)", tag[i]));
    h_eff[i]->Divide(h_truth[i]);
    for(int ib=1; ib<=h_eff[i]->GetNbinsX(); ++ib) {
      if(h_truth[i]->GetBinContent(ib) <= 0.0) {
        h_eff[i]->SetBinContent(ib, 0.0);
        h_eff[i]->SetBinError(ib, 0.0);
      }
    }
  }



  // total truth (for overlay/comparison only)
  TH1F* h_truth_total = (TH1F*)h_truth[0]->Clone("h_truth_total");
  h_truth_total->Add(h_truth[1], +1.0);

  // ======================
  // DATA  -- single chain. For each event:
  //   1) classify run period from run_number
  //   2) compute cos(theta), find its bin
  //   3) look up eff[period][bin]; fill h_data_*_unf with weight (sb*1)/eff
  //   peak: 5.832 < tq_rf_m_cor/GeV < 5.848
  //   sideband: > 5.80 outside peak (sb-weight: run2=0.194, run3=0.187)
  // ======================
  Init(chData);
  Long64_t nData = chData->GetEntries();
  std::cout << "Data entries = " << nData << std::endl;

  Long64_t nDropEffZero = 0;
  for(Long64_t i=0; i<nData; i++) {
    chData->GetEntry(i);

    bool isOS = trk_charge[0]*trk_charge[1] < 0;
    if(!( pt_mu1/GeV>4.0 && pt_mu2/GeV>4.0
          && pt_trk1_X/GeV>0.8 && pt_trk2_X/GeV>0.8
          && jpsi_m/GeV>2.96 && jpsi_m/GeV<3.22
          && isOS && x_rf_m/GeV>0.84 && x_rf_m/GeV<0.95
          && jx_m_calc/GeV>5.180 && jx_m_calc/GeV<5.375 && tq_rf_fitChi2NDF<1.8
          && ld_m/GeV>0.475 && ld_m/GeV<0.520 && tq_rf_ratio>0.20
          && tq_rf_m/GeV<5.90 && jx_Lxy>0.2 && tq_rf_pt/GeV>15 )) continue;

    const bool isRun2 = (run_number<=364485);
    const int  ip     = isRun2 ? 0 : 1;

    TLorentzVector tmpJX, tmpKs;
    tmpJX.SetPtEtaPhiM(jx_rf_pt, jx_rf_eta, jx_rf_phi, PDG_B0);
    tmpKs.SetPtEtaPhiM(ld_rf_pt, ld_rf_eta, ld_rf_phi, PDG_Ks);
    tq_rf_m_cor = (tmpJX + tmpKs).M();

    TLorentzVector Bs2_data, B0_data;
    Bs2_data.SetPtEtaPhiM(tq_rf_pt, tq_rf_eta, tq_rf_phi, tq_rf_m_cor);
    B0_data .SetPtEtaPhiM(jx_rf_pt, jx_rf_eta, jx_rf_phi, PDG_B0);
    TVector3 bv_data = -Bs2_data.BoostVector();
    B0_data.Boost(bv_data);
    const double cos_data = Bs2_data.Vect().Unit().Dot(B0_data.Vect().Unit());

    const double m_cor = tq_rf_m_cor/GeV;
    const bool inPeak     = (m_cor > 5.832 && m_cor < 5.848);
    const bool inSideband = (!inPeak && m_cor > 5.80);
    if(!inPeak && !inSideband) continue;

    // look up the bin-by-bin efficiency for this event's run period
    const int    bin     = h_eff[ip]->FindBin(cos_data);
    const double effVal  = h_eff[ip]->GetBinContent(bin);
    if(effVal <= 0.0) {
      ++nDropEffZero;
      continue; // can't unfold this bin
    }
    const double invEff = 1.0/effVal;

    if(inPeak) {
      h_data_peak    ->Fill(cos_data, 1.0);
      h_data_peak_unf->Fill(cos_data, invEff);
    } else { // sideband
      const Float_t wsb = isRun2 ? 0.194 : 0.187;
      h_data_bkg    ->Fill(cos_data, wsb);
      h_data_bkg_unf->Fill(cos_data, wsb * invEff);
    }
  }
  if(nDropEffZero > 0) {
    std::cout << "[warning] dropped " << nDropEffZero
              << " data events because eff[period][bin] <= 0\n";
  }

  // ======================
  // SIGNAL at fiducial = unfolded peak - unfolded sideband
  // ======================
  TH1F* h_data_signal_unf = (TH1F*)h_data_peak_unf->Clone("h_data_signal_unf");
  h_data_signal_unf->SetTitle(Form(";cos#theta;%s", yTitle.Data()));
  h_data_signal_unf->Add(h_data_bkg_unf, -1.0);

  // raw (pre-unfold) signal for comparison
  TH1F* h_data_signal = (TH1F*)h_data_peak->Clone("h_data_signal");
  h_data_signal->SetTitle(Form(";cos#theta;%s", yTitle.Data()));
  h_data_signal->Add(h_data_bkg, -1.0);

  TH1F* h_data_initial = h_data_signal_unf; // alias for the "initial" cos distribution
  
  // ======================
  // CANVAS 1 : preselection (h_sig) vs fiducial (h_truth) + ratio  -- per period
  // ======================
  for(int i=0;i<2;i++) {
    TCanvas* c = new TCanvas(Form("c_presel_vs_fid_%s",tag[i]),"",600,700);
    TPad* p1 = new TPad(Form("p1_%s",tag[i]),"",0,0.30,1,1);
    TPad* p2 = new TPad(Form("p2_%s",tag[i]),"",0,0.00,1,0.30);
    p1->SetLeftMargin(0.13);  p2->SetLeftMargin(0.13);
    p1->SetRightMargin(0.05); p2->SetRightMargin(0.05);
    p1->SetBottomMargin(0.02);
    p2->SetTopMargin(0.02);   p2->SetBottomMargin(0.35);
    p1->Draw(); p2->Draw();

    // visualization-only rescaling
    TH1F* h_truth_scaled = (TH1F*)h_truth[i]->Clone(Form("h_truth_%s_scaled",tag[i]));
    if(h_truth_scaled->Integral()>0 && h_sig[i]->Integral()>0)
      h_truth_scaled->Scale(h_sig[i]->Integral() * (cosMax-cosMin) / h_truth_scaled->Integral());

    p1->cd();
    double ymax = TMath::Max(h_sig[i]->GetMaximum(), h_truth_scaled->GetMaximum());
    h_sig[i]->SetMaximum(1.3*ymax);
    h_sig[i]->SetMinimum(0.0);
    h_sig[i]->SetLineColor(kBlue);  h_sig[i]->SetLineWidth(4);
    h_sig[i]->Draw("hist");
    h_truth_scaled->SetLineColor(kBlack); h_truth_scaled->SetLineWidth(4);
    h_truth_scaled->Draw("hist same");
    h_sig[i]->GetXaxis()->SetLabelSize(0);
    h_sig[i]->GetXaxis()->SetTitleSize(0);

    TLegend* lg = new TLegend(0.58,0.70,0.88,0.88);
    lg->SetBorderSize(0);
    lg->AddEntry(h_sig[i],       "Preselection","l");
    lg->AddEntry(h_truth_scaled, "Fiducial",    "l");
    lg->Draw();

    TLatex lat; lat.SetNDC(); lat.SetTextFont(42); lat.SetTextSize(0.045);
    lat.DrawLatex(0.16, 0.88, Form("MC simulation (%s)", tag[i]));

    p2->cd();
    TH1F* h_ratio = (TH1F*)h_sig[i]->Clone(Form("h_ratio_%s",tag[i]));
    h_ratio->SetTitle("");
    h_ratio->Divide(h_truth_scaled);
    h_ratio->SetLineColor(kBlue); h_ratio->SetLineWidth(4);
    h_ratio->SetMinimum(0.0); h_ratio->SetMaximum(1.0);
    h_ratio->GetYaxis()->SetTitle("Sig / Truth");
    h_ratio->GetYaxis()->SetNdivisions(505);
    h_ratio->GetYaxis()->SetTitleSize(0.10);
    h_ratio->GetYaxis()->SetLabelSize(0.09);
    h_ratio->GetYaxis()->SetTitleOffset(0.55);
    h_ratio->GetXaxis()->SetTitle("cos#theta");
    h_ratio->GetXaxis()->SetTitleSize(0.12);
    h_ratio->GetXaxis()->SetLabelSize(0.10);
    h_ratio->GetXaxis()->SetTitleOffset(1.10);
    h_ratio->Draw("hist");
    c->Update();
    c->SaveAs(Form("/Users/xnw/Desktop/thesis/fiducial_cos_%s.png", tag[i]));
  }

  // ======================
  // CANVAS 2 : efficiency overlay  (run2 vs run3)
  // ======================
  TCanvas* c_eff = new TCanvas("c_eff", "efficiency", 900, 700);
  h_eff[0]->SetMarkerStyle(20); h_eff[0]->SetMarkerColor(kBlue+1); h_eff[0]->SetLineColor(kBlue+1);  h_eff[0]->SetLineWidth(2);
  h_eff[1]->SetMarkerStyle(21); h_eff[1]->SetMarkerColor(kRed+1);  h_eff[1]->SetLineColor(kRed+1);   h_eff[1]->SetLineWidth(2);
  const double effmax = TMath::Max(h_eff[0]->GetMaximum(), h_eff[1]->GetMaximum());
  h_eff[0]->SetMinimum(0.0);
  h_eff[0]->SetMaximum(effmax > 0 ? 1.40*effmax : 1.0);
  h_eff[0]->Draw("E1");
  h_eff[1]->Draw("E1 SAME");
  TLegend* lg_eff = new TLegend(0.65,0.74,0.88,0.88);
  lg_eff->SetBorderSize(0); lg_eff->SetFillStyle(0);
  lg_eff->AddEntry(h_eff[0], "run2 efficiency", "PE");
  lg_eff->AddEntry(h_eff[1], "run3 efficiency", "PE");
  lg_eff->Draw();
  c_eff->SaveAs("/Users/xnw/Desktop/thesis/fiducial_cos_efficiency.png");

  // ======================
  // CANVAS 3 : data peak / unfolded peak / unfolded bkg / unfolded signal (combined, single hist)
  // ======================
  TCanvas* c2 = new TCanvas("c_data_unf", "data unfolded (run2+run3)", 600, 600);
  gStyle->SetEndErrorSize(2);

  const double ymax2 = TMath::Max(h_data_peak_unf->GetMaximum(),
                                  TMath::Max(h_data_bkg_unf->GetMaximum(),
                                             h_data_signal_unf->GetMaximum()));
  h_data_peak_unf->SetMaximum(ymax2 > 0 ? 1.20*ymax2 : 1.0);
  h_data_peak_unf->SetMinimum(0.0);
  h_data_peak_unf->SetMarkerStyle(20);
  h_data_peak_unf->SetMarkerColor(kBlack);
  h_data_peak_unf->SetLineColor(kBlack);
  h_data_peak_unf->SetLineWidth(2);
  h_data_peak_unf->GetYaxis()->SetTitle("Unfolded events");
  h_data_peak_unf->Draw("E1");

  h_data_bkg_unf->SetLineColor(kBlue+1);
  h_data_bkg_unf->SetLineWidth(2);
  h_data_bkg_unf->Draw("HIST SAME");

  h_data_signal_unf->SetMarkerStyle(24);
  h_data_signal_unf->SetMarkerColor(kRed+1);
  h_data_signal_unf->SetLineColor(kRed+1);
  h_data_signal_unf->SetLineWidth(2);
  h_data_signal_unf->Draw("E1 SAME");

  TLegend* lg2 = new TLegend(0.50, 0.70, 0.88, 0.88);
  lg2->SetBorderSize(0); lg2->SetFillStyle(0);
  lg2->AddEntry(h_data_peak_unf,   "Peak / #varepsilon",                  "PE");
  lg2->AddEntry(h_data_bkg_unf,    "Sideband / #varepsilon",              "L");
  lg2->AddEntry(h_data_signal_unf, "(Peak - sideband) / #varepsilon",     "PE");
  lg2->Draw();

  TLatex lat2; lat2.SetNDC(); lat2.SetTextFont(42); lat2.SetTextSize(0.040);
  lat2.DrawLatex(0.16, 0.88, "Data : unfolded (per-event 1/#varepsilon), then sideband-subtracted");
  c2->SaveAs("/Users/xnw/Desktop/thesis/fiducial_cos_data_unf.png");

  // ======================
  // CANVAS 4 : final "initial" cos distribution (data signal at fiducial level)
  // ======================
  TCanvas* c3 = new TCanvas("c_data_initial", "initial cos distribution from data", 600, 600);
  h_data_initial->SetMarkerStyle(20);
  h_data_initial->SetMarkerColor(kBlack);
  h_data_initial->SetLineColor(kBlack);
  h_data_initial->SetLineWidth(2);
  const double ymax3 = h_data_initial->GetMaximum();
  h_data_initial->SetMaximum(ymax3 > 0 ? 1.30*ymax3 : 1.0);
  h_data_initial->SetMinimum(0.0);
  h_data_initial->GetYaxis()->SetTitle(Form("Unfolded events / %.2f", binW));
  h_data_initial->Draw("E1");

  // overlay total truth (scaled to total unfolded integral) for shape comparison
  TH1F* h_truth_overlay = (TH1F*)h_truth_total->Clone("h_truth_overlay");
  if(h_truth_overlay->Integral()>0 && h_data_initial->Integral()>0)
    h_truth_overlay->Scale(h_data_initial->Integral() / h_truth_overlay->Integral());
  h_truth_overlay->SetLineColor(kGreen+2);
  h_truth_overlay->SetLineWidth(2);
  h_truth_overlay->Draw("hist same");

  TLegend* lg3 = new TLegend(0.55, 0.74, 0.88, 0.88);
  lg3->SetBorderSize(0); lg3->SetFillStyle(0);
  lg3->AddEntry(h_data_initial,  "Data signal / #varepsilon (run2+run3)", "PE");
  lg3->AddEntry(h_truth_overlay, "MC truth (scaled)",                     "L");
  lg3->Draw();

  TLatex lat3; lat3.SetNDC(); lat3.SetTextFont(42); lat3.SetTextSize(0.038);
  lat3.DrawLatex(0.16, 0.88, "Initial cos#theta distribution from data");
  c3->SaveAs("/Users/xnw/Desktop/thesis/fiducial_cos_data_initial.png");

  // ===================
  // SAVE to ROOT file
  // ===================
  TFile* fout = new TFile("ratio_fiducial_cos_runsplit.root", "RECREATE");
  for(int i=0;i<2;i++) {
    h_sig  [i]->Write(Form("h_sig_%s",   tag[i]));
    h_truth[i]->Write(Form("h_truth_%s", tag[i]));
    h_eff  [i]->Write(Form("h_eff_%s",   tag[i]));
  }
  h_data_peak     ->Write("h_data_peak");
  h_data_bkg      ->Write("h_data_bkg");
  h_data_signal   ->Write("h_data_signal");
  h_data_peak_unf ->Write("h_data_peak_unf");
  h_data_bkg_unf  ->Write("h_data_bkg_unf");
  h_data_signal_unf->Write("h_data_signal_unf");
  h_truth_total   ->Write("h_truth_total");
  fout->Close();

  // ===================
  // print summary
  // ===================
  std::cout << "------------------------------------------------------------\n";
  std::cout << "MC integrals per period (h_sig / h_truth):\n";
  for(int i=0;i<2;i++) {
    std::cout << "  " << tag[i] << "  :  h_sig=" << h_sig[i]->Integral()
                                << "  h_truth=" << h_truth[i]->Integral() << "\n";
  }
  std::cout << "Data (single chain, classified by run_number):\n";
  std::cout << "  h_data_peak       = " << h_data_peak     ->Integral() << "\n";
  std::cout << "  h_data_bkg        = " << h_data_bkg      ->Integral() << "\n";
  std::cout << "  h_data_signal     = " << h_data_signal   ->Integral() << " (raw)\n";
  std::cout << "  h_data_peak_unf   = " << h_data_peak_unf ->Integral() << "\n";
  std::cout << "  h_data_bkg_unf    = " << h_data_bkg_unf  ->Integral() << "\n";
  std::cout << "  h_data_signal_unf = " << h_data_signal_unf->Integral() << " (initial cos distribution)\n";
  std::cout << "------------------------------------------------------------\n";


}

