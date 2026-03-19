#include "TROOT.h"
#include "TObject.h"
#include "TChain.h"
#include "TFile.h"
#include "TH1D.h"
#include "TMinuit.h"
#include "THStack.h"
#include "TMath.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TLorentzVector.h"
#include "TLine.h"
#include "TRandom3.h"
#include "TArrow.h"
#include "reader_truth.h"
#include "reader.h"

#define GeV 1000

// newly defined variables
Int_t   event;
Float_t dR_B0Ks;
Float_t score1; // run-2
Float_t score2; // run-3

// config variable
Float_t BDTcut_run2 = -0.37;
Float_t BDTcut_run3 = -0.44;
Bool_t isRun3 = 0;
Bool_t isBs2 = 1;
Bool_t use802395 = 0;

TMVA::Reader* reader1; // run-2
TMVA::Reader* reader2; // run-3

Bool_t isBstarDecay(Bool_t isAnaCodeVersion=false) {
  if(isAnaCodeVersion) {
    if(cat==1) return true;
    else if(cat==2) return truth_isBstarDecay;
    else return false;
  }
  else {
    Bool_t tmp_isBstarDecay = false;
    for(Int_t i=0; i<truth_n; i++) {
      if(std::abs(truth_pdgId[i])==513 && truth_motherID[i]==35) {
	tmp_isBstarDecay = true;
	break;
      }
    }
    return tmp_isBstarDecay;
  }
}

Bool_t passFiducial(Bool_t isAnaCodeVersion=false) {
  Int_t pass_fiducial = false;
  if(isAnaCodeVersion) {
    if(cat==1)      pass_fiducial = truth_Bs1_pt/GeV>15 && fabs(truth_Bs1_eta)<2.5;
    else if(cat==2) pass_fiducial = truth_Bs2_pt/GeV>15 && fabs(truth_Bs2_eta)<2.5;
  }
  else {
    for(Int_t i=0; i<truth_n; i++) {
      if(truth_pdgId[i]==35) {
	if(truth_pT[i]/GeV>15 && fabs(truth_eta[i])<2.5) pass_fiducial = true;
	break;
      }
    }
  }
  return pass_fiducial;
}

Bool_t passFilter(Bool_t isAnaCodeVersion=false) {
  if(isAnaCodeVersion) {
    return truth_mu_pt[0]>=3400. && fabs(truth_mu_eta[0])<=2.6 && truth_mu_pt[1]>=3400. && fabs(truth_mu_eta[1])<=2.6 && truth_K_pt>=760. && fabs(truth_K_eta)<=2.6 && truth_pi1_pt>=760. && fabs(truth_pi1_eta)<=2.6;
  }
  else {
    Bool_t pass_mu1 = false;
    Bool_t pass_mu2 = false;
    Bool_t pass_kaon = false;
    Bool_t pass_pion = false;
    for(Int_t i=0; i<truth_n; i++) {
      if(truth_pdgId[i]==13 && truth_motherID[i]==443 && truth_pT[i]>=3400. && fabs(truth_eta[i])<=2.6) pass_mu1 = true;
      if(truth_pdgId[i]==-13 && truth_motherID[i]==443 && truth_pT[i]>=3400. && fabs(truth_eta[i])<=2.6) pass_mu2 = true;
      if(std::abs(truth_pdgId[i])==321 && (std::abs(truth_motherID[i])==313 || std::abs(truth_motherID[i])==999313) && truth_pT[i]>=760. && fabs(truth_eta[i])<=2.6) pass_kaon = true;
      if(std::abs(truth_pdgId[i])==211 && (std::abs(truth_motherID[i])==313 || std::abs(truth_motherID[i])==999313) && truth_pT[i]>=760. && fabs(truth_eta[i])<=2.6) pass_pion = true;
    }
    if(pass_mu1 && pass_mu2 && pass_kaon && pass_pion) return true;
    else return false;
  }
}

void run() {
  // nofilter truth sample
  TChain* ch1 = new TChain("truthTree");
  // Bs2
  if(isBs2) {
    if(!use802395) {
      if(!isRun3) ch1->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_nofilter/double/mc20_802829/*root*");
      else ch1->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_nofilter/double/mc23_802829/*root*");
    }
    else {
      if(!isRun3) ch1->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_nofilter/double/mc20_802395/*root*");
      else ch1->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_nofilter/double/mc23_802395/*root*");
    }
  }
  // Bs1
  else {
    if(!isRun3) ch1->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_nofilter/double/mc20_802827/*root*");
    else ch1->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_nofilter/double/mc23_802827/*root*");
  }
  Init_t(ch1);

  Float_t num_Bs2_B0[]     = {0,0,0,0,0,0,0,0,0,0,0};
  Float_t num_Bs2_Bstar0[] = {0,0,0,0,0,0,0,0,0,0,0};
  Float_t num_Bs1_Bstar0[] = {0,0,0,0,0,0,0,0,0,0,0};

  Long64_t nentries1 = ch1->GetEntries();
  printf("%llu entries to be processed...\n",nentries1);
  for(Long64_t entry=0; entry<nentries1; entry++) {

    ch1->GetEntry(entry);

    if(!passFiducial()) continue;

    Float_t wt = mc_event_weight;
    for(Int_t i=0; i<truth_n; i++) {
      if(truth_pdgId[i]==35) {
	if(!isRun3) {
	  wt *= exp(1.5716-0.0640473*truth_pT[i]/GeV);
	  wt *= 1.30932-0.264317*fabs(truth_eta[i]);
	}
	else {
	  wt *= exp(1.36741-0.0539602*truth_pT[i]/GeV);
	}
	break;
      }
    }

    if(isBstarDecay()) {
      if(isBs2) num_Bs2_Bstar0[0] += wt;
      else num_Bs1_Bstar0[0] += wt;
    }
    else num_Bs2_B0[0] += wt;

    if(passFilter()) {
      if(isBstarDecay()) {
	if(isBs2) num_Bs2_Bstar0[1] += wt;
	else num_Bs1_Bstar0[1] += wt;
      }
      else num_Bs2_B0[1] += wt;
    }
  }

  TChain* ch2 = new TChain("truthTree");
  // Bs2
  if(isBs2) {
    if(!isRun3) {
      if(!use802395) {
	ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc20_13TeV.802829.P8BEG_A14_NNPDF23LO_Bs2_B0Ks_massshift.BPHY25_Truth0.e8566_a907_r14859_MYSTREAM/*root*");
	ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc20_13TeV.802829.P8BEG_A14_NNPDF23LO_Bs2_B0Ks_massshift.BPHY25_Truth0.e8566_a907_r14860_MYSTREAM/*root*");
	ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc20_13TeV.802829.P8BEG_A14_NNPDF23LO_Bs2_B0Ks_massshift.BPHY25_Truth0.e8566_a907_r14861_MYSTREAM/*root*");
	ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc20_13TeV.802830.P8BEG_A14_NNPDF23LO_Bs2_B0Ks_bar_massshift.BPHY25_Truth0.e8566_a907_r14859_MYSTREAM/*root*");
	ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc20_13TeV.802830.P8BEG_A14_NNPDF23LO_Bs2_B0Ks_bar_massshift.BPHY25_Truth0.e8566_a907_r14860_MYSTREAM/*root*");
	ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc20_13TeV.802830.P8BEG_A14_NNPDF23LO_Bs2_B0Ks_bar_massshift.BPHY25_Truth0.e8566_a907_r14861_MYSTREAM/*root*");
      }
      else {
	ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc20_13TeV.802395.P8B_A14NNPDF23LO_BSM_B0Ks.BPHY25_Truth0.e8566_a907_r14859_MYSTREAM/*root*");
	ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc20_13TeV.802395.P8B_A14NNPDF23LO_BSM_B0Ks.BPHY25_Truth0.e8566_a907_r14860_MYSTREAM/*root*");
	ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc20_13TeV.802395.P8B_A14NNPDF23LO_BSM_B0Ks.BPHY25_Truth0.e8566_a907_r14861_MYSTREAM/*root*");
	ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc20_13TeV.802396.P8B_A14NNPDF23LO_BSM_B0Ksbar.BPHY25_Truth0.e8566_a907_r14859_MYSTREAM/*root*");
	ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc20_13TeV.802396.P8B_A14NNPDF23LO_BSM_B0Ksbar.BPHY25_Truth0.e8566_a907_r14860_MYSTREAM/*root*");
	ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc20_13TeV.802396.P8B_A14NNPDF23LO_BSM_B0Ksbar.BPHY25_Truth0.e8566_a907_r14861_MYSTREAM/*root*");
      }
    }
    else {
      if(!use802395) {
	ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc23_13p6TeV.802829.P8BEG_A14_NNPDF23LO_Bs2_B0Ks_massshift.BPHY25_Truth0.e8566_a910_r15540_MYSTREAM/*root*");
	ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc23_13p6TeV.802829.P8BEG_A14_NNPDF23LO_Bs2_B0Ks_massshift.BPHY25_Truth0.e8566_a911_r15530_MYSTREAM/*root*");
	ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc23_13p6TeV.802829.P8BEG_A14_NNPDF23LO_Bs2_B0Ks_massshift.BPHY25_Truth0.e8566_a934_r16083_MYSTREAM/*root*");
	ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc23_13p6TeV.802830.P8BEG_A14_NNPDF23LO_Bs2_B0Ks_bar_massshift.BPHY25_Truth0.e8566_a910_r15540_MYSTREAM/*root*");
	ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc23_13p6TeV.802830.P8BEG_A14_NNPDF23LO_Bs2_B0Ks_bar_massshift.BPHY25_Truth0.e8566_a911_r15530_MYSTREAM/*root*");
	ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc23_13p6TeV.802830.P8BEG_A14_NNPDF23LO_Bs2_B0Ks_bar_massshift.BPHY25_Truth0.e8566_a934_r16083_MYSTREAM/*root*");
      }
      else {
	ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc23_13p6TeV.802395.P8B_A14NNPDF23LO_BSM_B0Ks.BPHY25_Truth0.e8566_a910_r15540_MYSTREAM/*root*");
	ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc23_13p6TeV.802395.P8B_A14NNPDF23LO_BSM_B0Ks.BPHY25_Truth0.e8566_a911_r15530_MYSTREAM/*root*");
	ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc23_13p6TeV.802395.P8B_A14NNPDF23LO_BSM_B0Ks.BPHY25_Truth0.e8566_a934_r16083_MYSTREAM/*root*");
	ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc23_13p6TeV.802396.P8B_A14NNPDF23LO_BSM_B0Ksbar.BPHY25_Truth0.e8566_a910_r15540_MYSTREAM/*root*");
	ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc23_13p6TeV.802396.P8B_A14NNPDF23LO_BSM_B0Ksbar.BPHY25_Truth0.e8566_a911_r15530_MYSTREAM/*root*");
	ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc23_13p6TeV.802396.P8B_A14NNPDF23LO_BSM_B0Ksbar.BPHY25_Truth0.e8566_a934_r16083_MYSTREAM/*root*");
      }
    }
  }
  // Bs1
  else {
    if(!isRun3) {
      ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc20_13TeV.802827.P8BEG_A14_NNPDF23LO_Bs1_Bst0Ks_massshift.BPHY25_Truth0.e8566_a907_r14859_MYSTREAM/*root*");
      ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc20_13TeV.802827.P8BEG_A14_NNPDF23LO_Bs1_Bst0Ks_massshift.BPHY25_Truth0.e8566_a907_r14860_MYSTREAM/*root*");
      ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc20_13TeV.802827.P8BEG_A14_NNPDF23LO_Bs1_Bst0Ks_massshift.BPHY25_Truth0.e8566_a907_r14861_MYSTREAM/*root*");
      ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc20_13TeV.802828.P8BEG_A14_NNPDF23LO_Bs1_Bst0Ks_bar_massshift.BPHY25_Truth0.e8566_a907_r14859_MYSTREAM/*root*");
      ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc20_13TeV.802828.P8BEG_A14_NNPDF23LO_Bs1_Bst0Ks_bar_massshift.BPHY25_Truth0.e8566_a907_r14860_MYSTREAM/*root*");
      ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc20_13TeV.802828.P8BEG_A14_NNPDF23LO_Bs1_Bst0Ks_bar_massshift.BPHY25_Truth0.e8566_a907_r14861_MYSTREAM/*root*");
    }
    else {
      ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc23_13p6TeV.802827.P8BEG_A14_NNPDF23LO_Bs1_Bst0Ks_massshift.BPHY25_Truth0.e8566_a910_r15540_MYSTREAM/*root*");
      ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc23_13p6TeV.802827.P8BEG_A14_NNPDF23LO_Bs1_Bst0Ks_massshift.BPHY25_Truth0.e8566_a911_r15530_MYSTREAM/*root*");
      ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc23_13p6TeV.802827.P8BEG_A14_NNPDF23LO_Bs1_Bst0Ks_massshift.BPHY25_Truth0.e8566_a934_r16083_MYSTREAM/*root*");
      ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc23_13p6TeV.802828.P8BEG_A14_NNPDF23LO_Bs1_Bst0Ks_bar_massshift.BPHY25_Truth0.e8566_a910_r15540_MYSTREAM/*root*");
      ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc23_13p6TeV.802828.P8BEG_A14_NNPDF23LO_Bs1_Bst0Ks_bar_massshift.BPHY25_Truth0.e8566_a911_r15530_MYSTREAM/*root*");
      ch2->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/user.xchen.mc23_13p6TeV.802828.P8BEG_A14_NNPDF23LO_Bs1_Bst0Ks_bar_massshift.BPHY25_Truth0.e8566_a934_r16083_MYSTREAM/*root*");
    }
  }
  Init_t(ch2);

  Long64_t nentries2 = ch2->GetEntries();
  printf("%llu entries to be processed...\n",nentries2);
  for(Long64_t entry=0; entry<nentries2; entry++) {

    ch2->GetEntry(entry);

    if(!passFiducial() || !passFilter()) continue;

    Float_t wt = mc_event_weight;
    for(Int_t i=0; i<truth_n; i++) {
      if(truth_pdgId[i]==35) {
	if(!isRun3) {
	  wt *= exp(1.5716-0.0640473*truth_pT[i]/GeV);
	  wt *= 1.30932-0.264317*fabs(truth_eta[i]);
	}
	else {
	  wt *= exp(1.36741-0.0539602*truth_pT[i]/GeV);
	}
	break;
      }
    }

    if(isBstarDecay()) {
      if(isBs2) num_Bs2_Bstar0[2] += wt;
      else num_Bs1_Bstar0[2] += wt;
    }
    else num_Bs2_B0[2] += wt;
  }

  TChain* ch3 = new TChain("BPHY25");
  // Bs2
  if(isBs2) {
    if(!isRun3) {
      if(!use802395) {
	ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc20a_Bs2_802829.root");
	ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc20a_Bs2_802830.root");
	ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc20d_Bs2_802829.root");
	ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc20d_Bs2_802830.root");
	ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc20e_Bs2_802829.root");
	ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc20e_Bs2_802830.root");
      }
      else {
	ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc20a_Bs2_802395.root");
	ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc20a_Bs2_802396.root");
	ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc20d_Bs2_802395.root");
	ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc20d_Bs2_802396.root");
	ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc20e_Bs2_802395.root");
	ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc20e_Bs2_802396.root");
      }
    }
    else {
      if(!use802395) {
	ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc23a_Bs2_802829.root");
	ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc23a_Bs2_802830.root");
	ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc23d_Bs2_802829.root");
	ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc23d_Bs2_802830.root");
	ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc23e_Bs2_802829.root");
	ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc23e_Bs2_802830.root");
      }
      else {
	ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc23a_Bs2_802395.root");
	ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc23a_Bs2_802396.root");
	ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc23d_Bs2_802395.root");
	ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc23d_Bs2_802396.root");
	ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc23e_Bs2_802395.root");
	ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc23e_Bs2_802396.root");
      }
    }
  }
  // Bs1
  else {
    if(!isRun3) {
      ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc20a_Bs1_802827.root");
      ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc20a_Bs1_802828.root");
      ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc20d_Bs1_802827.root");
      ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc20d_Bs1_802828.root");
      ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc20e_Bs1_802827.root");
      ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc20e_Bs1_802828.root");
    }
    else {
      ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc23a_Bs1_802827.root");
      ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc23a_Bs1_802828.root");
      ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc23d_Bs1_802827.root");
      ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc23d_Bs1_802828.root");
      ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc23e_Bs1_802827.root");
      ch3->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_after_PV_constr/mc23e_Bs1_802828.root");
    }
  }   
  Init(ch3);

  reader1 = new TMVA::Reader("!Color:!Silent");
  reader1->AddVariable("ld_Lxy", &ld_Lxy);
  reader1->AddVariable("jx_Lxy", &jx_Lxy);
  reader1->AddVariable("tq_rf_ratio", &tq_rf_ratio);
  reader1->AddVariable("tq_rf_fitChi2NDF", &tq_rf_fitChi2NDF);
  reader1->AddVariable("ld_rf_pt", &ld_rf_pt);
  reader1->AddVariable("x_rf_pt", &x_rf_pt);
  reader1->AddSpectator("event", &event);
  reader1->BookMVA("BDTG", "BDT_run2/weights/TMVACrossValidation_BDTG.weights.xml");

  reader2 = new TMVA::Reader("!Color:!Silent");
  reader2->AddVariable("ld_Lxy", &ld_Lxy);
  reader2->AddVariable("jx_Lxy", &jx_Lxy);
  reader2->AddVariable("tq_rf_ratio", &tq_rf_ratio);
  reader2->AddVariable("tq_rf_fitChi2NDF", &tq_rf_fitChi2NDF);
  reader2->AddVariable("ld_rf_pt", &ld_rf_pt);
  reader2->AddVariable("x_rf_pt", &x_rf_pt);
  reader2->AddSpectator("event", &event);
  reader2->BookMVA("BDTG", "BDT_run3/weights/TMVACrossValidation_BDTG.weights.xml");
  
  Long64_t nentries3 = ch3->GetEntries();
  printf("%llu entries to be processed...\n",nentries3);
  for(Long64_t entry=0; entry<nentries3; entry++) {

    ch3->GetEntry(entry);

    if(!passFiducial(true) || !passFilter(true)) continue;

    Float_t wt = 1;
    // signal truth corr. (fiducial level)
    if(cat==1 || cat==2) {
      Float_t tmp1 = cat==1 ? truth_Bs1_pt/GeV : truth_Bs2_pt/GeV;
      Float_t tmp2 = cat==1 ? truth_Bs1_eta : truth_Bs2_eta;
      if(run_number<=364485) {
	wt *= exp(1.5716-0.0640473*tmp1);
	wt *= 1.30932-0.264317*fabs(tmp2);
      }
      else {
	wt *= exp(1.36741-0.0539602*tmp1);
      }
    }

    if(isBstarDecay(true)) {
      if(isBs2) num_Bs2_Bstar0[3] += wt;
      else num_Bs1_Bstar0[3] += wt;
    }
    else num_Bs2_B0[3] += wt;

    // pre-selection
    if(pt_mu1/GeV>4.0 && pt_mu2/GeV>4.0
       && pt_trk1_X/GeV>0.8 && pt_trk2_X/GeV>0.8
       && jpsi_m/GeV>2.96 && jpsi_m/GeV<3.22
       && trk_charge[0]*trk_charge[1]<0 && x_rf_m/GeV>0.84 && x_rf_m/GeV<0.95
       && jx_m_calc/GeV>5.180 && jx_m_calc/GeV<5.375 && tq_rf_fitChi2NDF<1.8
       && ld_m/GeV>0.475 && ld_m/GeV<0.520 && tq_rf_ratio>0.20
       && tq_rf_m/GeV<5.90 && jx_Lxy>0.2
       ) {
      TLorentzVector tmpJX; TLorentzVector tmpKs;
      tmpJX.SetPtEtaPhiM(jx_rf_pt,jx_rf_eta,jx_rf_phi,jx_rf_m);
      tmpKs.SetPtEtaPhiM(ld_rf_pt,ld_rf_eta,ld_rf_phi,ld_rf_m);
      dR_B0Ks = tmpJX.DeltaR(tmpKs);

      event = event_number % 100000;
      bool passBDT = false;
      if(run_number<=364485) {
	score1 = reader1->EvaluateMVA("BDTG");
	passBDT = (score1 > BDTcut_run2);
      }
      else {
	score2 = reader2->EvaluateMVA("BDTG");
	passBDT = (score2 > BDTcut_run3);
      }

      Float_t wt_muID = 1;
      if(run_number>=431810) { // run3
	wt_muID *= mu_pt[0]/GeV>5 ? mu_medium_eff_sf[0] : mu_lowpt_eff_sf[0];
	wt_muID *= mu_pt[1]/GeV>5 ? mu_medium_eff_sf[1] : mu_lowpt_eff_sf[1];
      }
      else {
	wt_muID *= mu_medium_eff_sf[0]*mu_medium_eff_sf[1];
      }


      float wt_trig_cor = 1;
      if(run_number>=266904 && run_number<=284484) {
	wt_trig_cor *= 38.1/20.1;
      }
      else if(run_number>=297730 && run_number<=302393) {
	wt_trig_cor *= 20.7/24.7;
      }
      else if(run_number>=302737 && run_number<=303560) {
	wt_trig_cor *= 14.6/21.7;
      }
      else if(run_number>=303638 && run_number<=311481) {
	wt_trig_cor *= 127.9/103.6;
      }
      else if(run_number>=325713 && run_number<=328393) {
	wt_trig_cor *= 6.6/21.3;
      }
      else if(run_number>=329385 && run_number<=340453) {
	wt_trig_cor *= 120.2/165.4;
      }
      else if(run_number>=348885 && run_number<=364292) {
	wt_trig_cor *= 284.5/300.3;
      }
      else if(run_number>=431810 && run_number<=440613) {
	wt_trig_cor *= 62.3/70.6;
      }
      else if(run_number>=451587 && run_number<=456749) {
	wt_trig_cor *= 102.4/111.2;
      }
      else if(run_number>=473235 && run_number<=486706) {
	wt_trig_cor *= 400.9/384.8;
      }



      Float_t wt_recoCor = 1;
      Float_t tmp;
      if(run_number<=364485) {
	tmp = pt_mu2/GeV;
	if(tmp<6) wt_recoCor *= 0.907558;
	else wt_recoCor *= 1.03618;
      }
      else {
	tmp = pt_mu2/GeV;
	if(tmp>15) tmp = 15;
	wt_recoCor *= (0.707543+0.0225414*tmp+0.00132893*pow(tmp,2))*1.02651;
	tmp = pt_mu1/GeV;
	if(tmp>20) tmp = 20;
	wt_recoCor *= (0.475828+0.0402479*tmp)*1;
	tmp = tq_rf_eta;
	if(fabs(tmp)<0.8) wt_recoCor *= (0.829035+0.0468434*tmp-0.382025*pow(tmp,2))*1.08259;
	else wt_recoCor *= 1.20288;
      }

      tmp = tq_rf_ratio;
      if(tmp<0.2) tmp = 0.2; if(tmp>0.75) tmp = 0.75;
      wt_recoCor *= (2.4395-3.08002*tmp)*1.00890;

      tmp = tq_rf_fitChi2NDF;
      if(tmp>1.8) tmp = 1.8;
      wt_recoCor *= (0.734235+0.28194*tmp)*1.02518;

      tmp = pt_LRT_trk2/GeV;
      if(tmp>2) tmp = 2;
      if(tmp<0.9) wt_recoCor *= (-7.34283+25.8847*tmp-19.553*pow(tmp,2))*1.01762;
      else wt_recoCor *= (1.48474-1.55385*tmp+1.18987*pow(tmp,2)-0.233051*pow(tmp,3))*1.01762;

      if(isBstarDecay(true)) {
	if(isBs2) {
	  num_Bs2_Bstar0[4] += wt;
	  num_Bs2_Bstar0[5] += wt*trig_wt;
	  num_Bs2_Bstar0[6] += wt*trig_wt*pu_wt;
	  num_Bs2_Bstar0[7] += wt*trig_wt*pu_wt*wt_muID;
	  num_Bs2_Bstar0[8] += wt*trig_wt*pu_wt*wt_muID*wt_recoCor;
	  if(passBDT) num_Bs2_Bstar0[9] += wt*trig_wt*pu_wt*wt_muID*wt_recoCor,num_Bs2_Bstar0[10] += wt*trig_wt*pu_wt*wt_muID*wt_recoCor*wt_trig_cor;
	}
	else {
	  num_Bs1_Bstar0[4] += wt;
	  num_Bs1_Bstar0[5] += wt*trig_wt;
	  num_Bs1_Bstar0[6] += wt*trig_wt*pu_wt;
	  num_Bs1_Bstar0[7] += wt*trig_wt*pu_wt*wt_muID;
	  num_Bs1_Bstar0[8] += wt*trig_wt*pu_wt*wt_muID*wt_recoCor;
	  if(passBDT) num_Bs1_Bstar0[9] += wt*trig_wt*pu_wt*wt_muID*wt_recoCor,num_Bs1_Bstar0[10] += wt*trig_wt*pu_wt*wt_muID*wt_recoCor*wt_trig_cor;
	}
      }
      else {
	num_Bs2_B0[4] += wt;
	num_Bs2_B0[5] += wt*trig_wt;
	num_Bs2_B0[6] += wt*trig_wt*pu_wt;
	num_Bs2_B0[7] += wt*trig_wt*pu_wt*wt_muID;
	num_Bs2_B0[8] += wt*trig_wt*pu_wt*wt_muID*wt_recoCor;
	if(passBDT) num_Bs2_B0[9] += wt*trig_wt*pu_wt*wt_muID*wt_recoCor,num_Bs2_B0[10] += wt*trig_wt*pu_wt*wt_muID*wt_recoCor*wt_trig_cor;
      }
    }
  }

  if(isBs2) {
    printf(" Fiducial: %.2f, %.2f\n",num_Bs2_B0[0],num_Bs2_Bstar0[0]);
    printf("Filter(1): %.2f, %.2f\n",num_Bs2_B0[1],num_Bs2_Bstar0[1]);
    printf("Filter(2): %.2f, %.2f\n",num_Bs2_B0[2],num_Bs2_Bstar0[2]);
    printf("  anaCode: %.2f, %.2f\n",num_Bs2_B0[3],num_Bs2_Bstar0[3]);
    printf("   presel: %.2f, %.2f\n",num_Bs2_B0[4],num_Bs2_Bstar0[4]);
    printf("   trigWt: %.2f, %.2f\n",num_Bs2_B0[5],num_Bs2_Bstar0[5]);
    printf("     PuWt: %.2f, %.2f\n",num_Bs2_B0[6],num_Bs2_Bstar0[6]);
    printf("   muIDWt: %.2f, %.2f\n",num_Bs2_B0[7],num_Bs2_Bstar0[7]);
    printf("recoCorWt: %.2f, %.2f\n",num_Bs2_B0[8],num_Bs2_Bstar0[8]);
    printf("      BDT: %.2f, %.2f\n",num_Bs2_B0[9],num_Bs2_Bstar0[9]);
    printf("   trigSF: %.2f, %.2f\n",num_Bs2_B0[10],num_Bs2_Bstar0[10]);
  }
  else {
    printf(" Fiducial: %.2f\n",num_Bs1_Bstar0[0]);
    printf("Filter(1): %.2f\n",num_Bs1_Bstar0[1]);
    printf("Filter(2): %.2f\n",num_Bs1_Bstar0[2]);
    printf("  anaCode: %.2f\n",num_Bs1_Bstar0[3]);
    printf("   presel: %.2f\n",num_Bs1_Bstar0[4]);
    printf("   trigWt: %.2f\n",num_Bs1_Bstar0[5]);
    printf("     PuWt: %.2f\n",num_Bs1_Bstar0[6]);
    printf("   muIDWt: %.2f\n",num_Bs1_Bstar0[7]);
    printf("recoCorWt: %.2f\n",num_Bs1_Bstar0[8]);
    printf("      BDT: %.2f\n",num_Bs1_Bstar0[9]);
    printf("   trigSF: %.2f\n",num_Bs1_Bstar0[10]);
  }
}

