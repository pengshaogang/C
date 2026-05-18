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
#include "TLegend.h"
#include "TRandom3.h"
#include "TArrow.h"
#include "../../../AtlasLabels.h"
#include "../../../AtlasUtils.h"
#include "reader.h"
#include "reader_truth.h"
#include "HelicityAngles.h" 

const Double_t GeV=1000;
const Double_t PI=3.1415926536;
void SetMax(TH1* h1, TH1* h2, Double_t scale=1.0) {
  h1->SetMaximum(scale*TMath::Max(h1->GetMaximum(),h2->GetMaximum()));
  h2->SetMaximum(scale*TMath::Max(h1->GetMaximum(),h2->GetMaximum()));
}
float DR(float eta1, float phi1, float eta2, float phi2) {
  float dphi = fabs(phi1-phi2);
  if(dphi>M_PI) dphi = 2*M_PI-dphi;
  float deta = fabs(eta1-eta2);
  return sqrt(dphi*dphi+deta*deta);
}
// newly defined variables
Int_t   event;
Float_t dR_B0Ks;
// config variable
Int_t isRun3 = -1; // -1,0,1 -> all,run2,run3
Bool_t save_ratio = 1; 


void plot() {
  TChain* ch[3] = {nullptr,nullptr,nullptr};
  //mc
  ch[0] = new TChain("BPHY25");
  if(isRun3==-1 || isRun3==0) {
    ch[0]->Add("../../../../mix3/ntuple/mc20a_Bs2_802829_baseline.root");
    //ch[0]->Add("../../../../mix3/ntuple/mc20a_Bs2_802830_baseline.root");
    ch[0]->Add("../../../../mix3/ntuple/mc20d_Bs2_802829_baseline.root");
    //ch[0]->Add("../../../../mix3/ntuple/mc20d_Bs2_802830_baseline.root");
    ch[0]->Add("../../../../mix3/ntuple/mc20e_Bs2_802829_baseline.root");
    //ch[0]->Add("../../../../mix3/ntuple/mc20e_Bs2_802830_baseline.root");
  }
  if(isRun3==-1 || isRun3==1) {
    ch[0]->Add("../../../../mix3/ntuple/mc23a_Bs2_802829_baseline.root");
    //ch[0]->Add("../../../../mix3/ntuple/mc23a_Bs2_802830_baseline.root");
    ch[0]->Add("../../../../mix3/ntuple/mc23d_Bs2_802829_baseline.root");
    //ch[0]->Add("../../../../mix3/ntuple/mc23d_Bs2_802830_baseline.root");
    ch[0]->Add("../../../../mix3/ntuple/mc23e_Bs2_802829_baseline.root");
    //ch[0]->Add("../../../../mix3/ntuple/mc23e_Bs2_802830_baseline.root");
  }
  Init(ch[0]);  

  ch[1] = new TChain("truthTree");
  if(isRun3==-1 || isRun3==0) {
    ch[1]->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_nofilter/all/mc20_802829/mytruth.root.1");
    ch[1]->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_nofilter/all/mc20_802829/mytruth.root.2");
    ch[1]->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_nofilter/all/mc20_802829/mytruth.root.3");
    ch[1]->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_nofilter/all/mc20_802829/mytruth.root.4");
    //ch[1]->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/mc20a_Bs2_802829.root");
    //ch[1]->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/mc20a_Bs2_802830.root");
    //ch[1]->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/mc20d_Bs2_802829.root");
    //ch[1]->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/mc20d_Bs2_802830.root");
    //ch[1]->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/mc20e_Bs2_802829.root");
    //ch[1]->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/mc20e_Bs2_802830.root");
 
  }  
  if(isRun3==-1 || isRun3==1) {
    ch[1]->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_nofilter/all/mc23_802829/mytruth.root.1");
    ch[1]->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_nofilter/all/mc23_802829/mytruth.root.2");
    //ch[1]->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/mc23a_Bs2_802829.root");
    //ch[1]->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/mc23a_Bs2_802830.root");
    //ch[1]->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/mc23d_Bs2_802829.root");
    //ch[1]->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/mc23d_Bs2_802830.root");
    //ch[1]->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/mc23e_Bs2_802829.root");
    //ch[1]->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_truth/mc23e_Bs2_802830.root");
  
  }
  Init_t(ch[1]);

  //data
  ch[2] = new TChain("BPHY25");
  ch[2]->Add("../../../mix3/ntuple/data15_Main_baseline.root");
  ch[2]->Add("../../../mix3/ntuple/data16_All_baseline.root");
  ch[2]->Add("../../../mix3/ntuple/data17_All_baseline.root");
  ch[2]->Add("../../../mix3/ntuple/data18_All_baseline.root");
  ch[2]->Add("../../../mix3/ntuple/data22_BphysDelayed_baseline.root");
  ch[2]->Add("../../../mix3/ntuple/data23_BphysDelayed_baseline.root");
  ch[2]->Add("../../../mix3/ntuple/data24_BphysDelayed_baseline.root");
  Init(ch[2]);  

  char str[100];
  TH1F* h[3][2];
  TH1F* h_ratio[2];
  TLegend* leg[2];

  for(Int_t i=0; i<2; i++) {
    snprintf(str,100,"h1_%d",i+1);
    h[0][i] = new TH1F(str,"",20,-1,1);
    h[0][i]->Sumw2(true);
    snprintf(str,100,"h2_%d",i+1);
    h[1][i] = new TH1F(str,"",20,-1,1);
    h[1][i]->Sumw2(true);
    snprintf(str,100,"h3_%d",i+1);
    h[2][i] = new TH1F(str,"",20,-1,1);
    h[2][i]->Sumw2(true);
  }
 
  for(Int_t ich=0; ich<1; ich++) {
    if(ich==1) continue;
    if(!ch[ich]) continue;
    Long64_t nentries = ch[ich]->GetEntries();
    printf("%llu entries to be processed...\n",nentries);
    for(Long64_t entry=0; entry<nentries; entry++) {
      ch[ich]->GetEntry(entry);
      if(isRun3==0 && run_number>364485) continue;    
      else if(isRun3==1 && run_number<=364485) continue;
      Bool_t isOS = trk_charge[0]*trk_charge[1]<0;
      // pre-selection
      if(pt_mu1/GeV>4.0 && pt_mu2/GeV>4.0
	 && pt_trk1_X/GeV>0.8 && pt_trk2_X/GeV>0.8
	 && jpsi_m/GeV>2.96 && jpsi_m/GeV<3.22
	 && isOS && x_rf_m/GeV>0.84 && x_rf_m/GeV<0.95
	 && jx_m_calc/GeV>5.180 && jx_m_calc/GeV<5.375 && tq_rf_fitChi2NDF<1.8
	 && ld_m/GeV>0.475 && ld_m/GeV<0.520 && tq_rf_ratio>0.20
	 && tq_rf_m/GeV<5.90 && jx_Lxy>0.2
	 ) {
	int idx(-1);
	if(ich==0) {
	  if(truth_isBstarDecay) idx = 1; // Bs2 low peak
	  else idx = 2; // Bs2 high peak
	}
	else if(ich==2) {
	  idx = 3;
	}
	else continue;
	TLorentzVector tmpJX; TLorentzVector tmpKs;
	tmpJX.SetPtEtaPhiM(jx_rf_pt,jx_rf_eta,jx_rf_phi,jx_rf_m);
	tmpKs.SetPtEtaPhiM(ld_rf_pt,ld_rf_eta,ld_rf_phi,ld_rf_m);
	dR_B0Ks = tmpJX.DeltaR(tmpKs);

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


	wt *= trig_wt*pu_wt;
	if(run_number>=431810) { // run3
	  wt *= mu_pt[0]/GeV>5 ? mu_medium_eff_sf[0] : mu_lowpt_eff_sf[0];
	  wt *= mu_pt[1]/GeV>5 ? mu_medium_eff_sf[1] : mu_lowpt_eff_sf[1];
	}
	else {
	  wt *= mu_medium_eff_sf[0]*mu_medium_eff_sf[1];
	}
	wt *= run_number<=364485 ? 0.0547 : 0.0363; // nominal
        Float_t tmp;
	if(run_number<=364485) {
	  tmp = pt_mu2/GeV;
	  if(tmp<6) wt *= 0.907558;
	  else wt *= 1.03618;
	}
        else {
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


        TLorentzVector tmpJX1, tmpKs1;
        tmpJX1.SetPtEtaPhiM(jx_rf_pt, jx_rf_eta, jx_rf_phi, 5279.66);
        tmpKs1.SetPtEtaPhiM(ld_rf_pt, ld_rf_eta, ld_rf_phi, 497.611);
        Float_t tq_rf_m_cor = (tmpJX1 + tmpKs1).M();
        TLorentzVector Bs_t, B0_t;
        Bs_t.SetPtEtaPhiM(tq_rf_pt, tq_rf_eta, tq_rf_phi, tq_rf_m);
        B0_t.SetPtEtaPhiM(jx_rf_pt, jx_rf_eta, jx_rf_phi, jx_rf_m);
        HelicityAngles ht = computeHelicityAnglesBsToB0(Bs_t, B0_t);
        if(ht.ok) {
          const double ct_reco = cos(ht.theta);
          if(run_number<=364485) h[0][0]->Fill(ct_reco, wt);
          if(run_number>=431810) h[1][0]->Fill(ct_reco, wt);
        }
      }
    }
  }

  Long64_t nentries3 = ch[1]->GetEntries();
  printf("%llu entries to be processed...\n",nentries3);
  for(Long64_t entry=0; entry<nentries3; entry++) {
    ch[1]->GetEntry(entry);
    TLorentzVector P_Bs12, P_B0;
    Bool_t truth_isBstarDecay = false;
    for(Int_t i=0; i<truth_n; i++) {
      if(std::abs(truth_pdgId[i])==513 && truth_motherID[i]==35) { 
        truth_isBstarDecay = true;
	break;
      }
    }

    if(truth_isBstarDecay) continue;



    Int_t pass_fiducial = false; 
    Bool_t foundBs = false, foundB0 = false;
    Float_t weight = 1.0;
    Float_t pt(-999), eta(-999); 
    for(Int_t i=0; i<truth_n; i++) {
      if(truth_pdgId[i]==35) {
        if(truth_pT[i]/GeV>15 && fabs(truth_eta[i])<2.5) { 
       	  weight = mc_event_weight;
	  //weight *= exp(1.77456-0.0540623*truth_pT[i]/GeV);

	  Float_t tmp1 = truth_pT[i]/GeV;
	  Float_t tmp2 = truth_eta[i];
	  if(run_number<=364485) {
	    weight *= exp(1.5716-0.0640473*tmp1);
	    weight *= 1.30932-0.264317*fabs(tmp2);
	  }
	  else {
	    weight *= exp(1.36741-0.0539602*tmp1);
	  }
	  //weight *= 1.176-0.166729*fabs(truth_eta[i]);
	  P_Bs12.SetPtEtaPhiM(truth_pT[i], truth_eta[i], truth_phi[i], truth_mass[i]);
          foundBs = true;
	}	
      }
      if (std::abs(truth_pdgId[i]) == 511 && truth_motherID[i] == 35) {
        P_B0.SetPtEtaPhiM(truth_pT[i], truth_eta[i], truth_phi[i], truth_mass[i]);
        foundB0 = true;
      }
    }



    if (!foundBs || !foundB0) continue;
    HelicityAngles ht = computeHelicityAnglesBsToB0(P_Bs12, P_B0);
    if(!ht.ok) continue;
    const double ct_truth = cos(ht.theta);
    if(ch[1]->GetTreeNumber()<=3) h[0][1]->Fill(ct_truth, weight);//mc20
    else h[1][1]->Fill(ct_truth, weight); //mc23
  }

  for(Int_t i=0; i<2; i++) {
    snprintf(str,100,"hratio%d",i+1);
    h_ratio[i] = new TH1F(str,"",h[i][0]->GetNbinsX(),h[i][0]->GetXaxis()->GetXmin(),h[i][0]->GetXaxis()->GetXmax());
    if(i==0) leg[i] = new TLegend(0.7, 0.8, 0.9, 0.9);
    else leg[i] = new TLegend(0.7, 0.7, 0.9, 0.8);
  }
 

  TCanvas* c1;

  
  
  snprintf(str,100,"c%d",1);  
  c1 = new TCanvas(str,str,600,600);


  for(Int_t i=0; i<2; i++) { 
    for(Int_t j=1; j<=h_ratio[i]->GetNbinsX(); j++) {
      Float_t tmp1 = h[i][0]->GetBinContent(j)>0 ? h[i][0]->GetBinError(j)/h[i][0]->GetBinContent(j) : 0;
      Float_t tmp2 = h[i][1]->GetBinContent(j)>0 ? h[i][1]->GetBinError(j)/h[i][1]->GetBinContent(j) : 0;
      Float_t tmp = h[i][1]->GetBinContent(j)>0 ? h[i][0]->GetBinContent(j)/h[i][1]->GetBinContent(j) : 0;
      h_ratio[i]->SetBinContent(j,tmp); 
      h_ratio[i]->SetBinError(j,tmp*sqrt(pow(tmp1,2)+pow(tmp2,2)));
    }
  } 

  for(Int_t i=0; i<2; i++) {
      //h_ratio[i]->GetXaxis()->SetLabelFont(42);
      //h_ratio[i]->GetXaxis()->SetLabelSize(0.12);
      //h_ratio[i]->GetXaxis()->SetLabelOffset(0.02);
      //h_ratio[i]->GetXaxis()->SetTitleOffset(1.0);
      //h_ratio[i]->GetXaxis()->SetTitleSize(0.15);
      //h_ratio[i]->GetYaxis()->SetLabelFont(42);
      //h_ratio[i]->GetYaxis()->SetLabelSize(0.12);
      //h_ratio[i]->GetYaxis()->SetTitleOffset(0.45);
      //h_ratio[i]->GetYaxis()->SetTitleSize(0.12);
      if(i==0) h_ratio[i]->SetLineColor(kBlue);
      else h_ratio[i]->SetLineColor(kRed);
      h_ratio[i]->SetLineWidth(3);
      h_ratio[i]->SetMarkerStyle(8);
      h_ratio[i]->SetMarkerSize(0.6);
      h_ratio[i]->SetMinimum(0);
      h_ratio[i]->SetMaximum(0.044);
      //h_ratio[i]->SetMaximum(1.0-1e-3);
      //h_ratio[i]->SetMinimum(0.0+1e-3);
      //h_ratio[i]->GetYaxis()->SetNdivisions(505);
      //h_ratio[i]->GetYaxis()->SetTitle("Ratio");
      //h_ratio[i]->GetYaxis()->CenterTitle();
      h_ratio[i]->GetXaxis()->SetTitle("cos(#theta)");
      if(i==0) h_ratio[i]->Draw("EP1");
      else h_ratio[i]->Draw("EP SAME");
  
      leg[i]->SetFillStyle(0);              
      leg[i]->SetBorderSize(0);             
      leg[i]->SetTextSize(0.03);             
      if(i==0) leg[i]->AddEntry(h_ratio[i], "Run2 Eff.", "lep");     
      else leg[i]->AddEntry(h_ratio[i], "Run3 Eff.", "lep");
      leg[i]->Draw();

      if(save_ratio) {
	TFile *fout;
        if(i==0) {
          fout = new TFile("eff_run2.root", "RECREATE");
	  h_ratio[i]->Write("h_eff_run2");
        } else {
	  fout = new TFile("eff_run3.root", "RECREATE");
          h_ratio[i]->Write("h_eff_run3");
        }
        fout = new TFile("eff_all.root", "RECREATE");
        h_ratio[i]->Write("h_eff_all");
	fout->Close();
      }
  }
}
 
