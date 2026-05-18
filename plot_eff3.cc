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

Bool_t read_ratio = 1;



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
  ch[2]->Add("../../../../mix3/ntuple/data15_Main_baseline.root");
  ch[2]->Add("../../../../mix3/ntuple/data16_All_baseline.root");
  ch[2]->Add("../../../../mix3/ntuple/data17_All_baseline.root");
  ch[2]->Add("../../../../mix3/ntuple/data18_All_baseline.root");
  ch[2]->Add("../../../../mix3/ntuple/data22_BphysDelayed_baseline.root");
  ch[2]->Add("../../../../mix3/ntuple/data23_BphysDelayed_baseline.root");
  ch[2]->Add("../../../../mix3/ntuple/data24_BphysDelayed_baseline.root");
  Init(ch[2]);  

  char str[100];
  TH1F* h[9][2];

  for(Int_t i=0; i<2; i++) {
    for(Int_t j=0; j<9; j++) {
      snprintf(str,100,"h1_%d_%d",i+1,j+1);
      h[j][i] = new TH1F(str,"",20,-1,1);
      h[j][i]->Sumw2(true);
    }
  }


 
  for(Int_t ich=2; ich<3; ich++) {
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
	if(ich==2) idx = 3;
	else continue;
	TLorentzVector tmpJX; TLorentzVector tmpKs;
	tmpJX.SetPtEtaPhiM(jx_rf_pt,jx_rf_eta,jx_rf_phi,jx_rf_m);
	tmpKs.SetPtEtaPhiM(ld_rf_pt,ld_rf_eta,ld_rf_phi,ld_rf_m);
	dR_B0Ks = tmpJX.DeltaR(tmpKs);

        Float_t wt = 1;

        TLorentzVector tmpJX1, tmpKs1;
        tmpJX1.SetPtEtaPhiM(jx_rf_pt, jx_rf_eta, jx_rf_phi, 5279.66);
        tmpKs1.SetPtEtaPhiM(ld_rf_pt, ld_rf_eta, ld_rf_phi, 497.611);
        Float_t tq_rf_m_cor = (tmpJX1 + tmpKs1).M();
        TLorentzVector Bs_t, B0_t;
        Bs_t.SetPtEtaPhiM(tq_rf_pt, tq_rf_eta, tq_rf_phi, tq_rf_m);
        B0_t.SetPtEtaPhiM(jx_rf_pt, jx_rf_eta, jx_rf_phi, jx_rf_m);
        HelicityAngles ht = computeHelicityAnglesBsToB0(Bs_t, B0_t);

        if( idx==3
          && !(tq_rf_m_cor/GeV>5.832 && tq_rf_m_cor/GeV<5.848)
          && !(tq_rf_m_cor/GeV<5.80)
        ) {
          idx = 4; // TQ mass sideband
          wt = run_number<=364485 ? 0.194 : 0.187;
        }
        if(ht.ok) {
          const double ct_reco = cos(ht.theta);
          if(tq_rf_m_cor/GeV>5.832 && tq_rf_m_cor/GeV<5.848) {
	    if(run_number<=364485) h[0][0]->Fill(ct_reco);
            if(run_number>=431810) h[1][0]->Fill(ct_reco);
	    h[2][0]->Fill(ct_reco);
	  }
	  if(idx==4) {
	    if(run_number<=364485) h[3][0]->Fill(ct_reco,wt);
	    if(run_number>=431810) h[4][0]->Fill(ct_reco,wt);
	    h[5][0]->Fill(ct_reco,wt);
	  }
        }

      }
    }
  }

  h[6][0] = (TH1F*)h[0][0]->Clone("h_SRR_run2");
  h[7][0] = (TH1F*)h[1][0]->Clone("h_SRR_run3");
  h[8][0] = (TH1F*)h[2][0]->Clone("h_SRR_all");
  h[6][0]->Add(h[3][0], -1.0);
  h[7][0]->Add(h[4][0], -1.0);
  h[8][0]->Add(h[5][0], -1.0);


  if(read_ratio) { 
    TFile* f2 = new TFile("eff_run2.root", "READ");
    TFile* f3 = new TFile("eff_run3.root", "READ");
    TFile* fin_all = new TFile("eff_all.root", "READ");
    TH1D* h_ratio2 = (TH1D*)f2->Get("h_eff_run2");
    TH1D* h_ratio3 = (TH1D*)f3->Get("h_eff_run3");
    TH1D* h_all   = (TH1D*)fin_all->Get("h_eff_all");
    if(h_ratio2) h_ratio2->SetDirectory(0);
    if(h_ratio3) h_ratio3->SetDirectory(0);
    if(h_all) h_all->SetDirectory(0);
    f2->Close();
    f3->Close();
    fin_all->Close();

 
    for(Int_t i=0; i<3; i++) {
      for(Int_t j=1; j<=h[i][0]->GetNbinsX(); j++) {

        Float_t tmp1_1 = h[i][0]->GetBinContent(j)>0 ? h[i][0]->GetBinError(j)/h[i][0]->GetBinContent(j) : 0;
        Float_t tmp1_2 = h[i+3][0]->GetBinContent(j)>0 ? h[i+3][0]->GetBinError(j)/h[i+3][0]->GetBinContent(j) : 0;
        Float_t tmp1_3 = h[i+6][0]->GetBinContent(j)>0 ? h[i+6][0]->GetBinError(j)/h[i+6][0]->GetBinContent(j) : 0;
        Float_t tmp2;
        if(i==2) tmp2 = h_all->GetBinContent(j)>0 ? h_all->GetBinError(j)/h_all->GetBinContent(j) : 0;
        else if(i==1) tmp2 = h_ratio3->GetBinContent(j)>0 ? h_ratio3->GetBinError(j)/h_ratio3->GetBinContent(j) : 0;
        else tmp2 = h_ratio2->GetBinContent(j)>0 ? h_ratio2->GetBinError(j)/h_ratio2->GetBinContent(j) : 0;

        const double eps = (i == 2) ? h_all->GetBinContent(j) : (i == 1) ? h_ratio3->GetBinContent(j) : h_ratio2->GetBinContent(j);
        if (eps <= 0) continue;
        const Float_t tmp_1 = h[i][0]->GetBinContent(j) / eps;
        const Float_t tmp_2 = h[i+3][0]->GetBinContent(j) / eps;
        const Float_t tmp_3 = h[i+6][0]->GetBinContent(j) / eps;
        h[i][1]->SetBinContent(j, tmp_1);
        h[i][1]->SetBinError(j, tmp_1 * sqrt(tmp1_1*tmp1_1 + tmp2*tmp2));
        h[i+3][1]->SetBinContent(j, tmp_2);
        h[i+3][1]->SetBinError(j, tmp_2 * sqrt(tmp1_2*tmp1_2 + tmp2*tmp2));
        h[i+6][1]->SetBinContent(j, tmp_3);
        h[i+6][1]->SetBinError(j, tmp_3 * sqrt(tmp1_3*tmp1_3 + tmp2*tmp2));

      }
    }
 
  }

  TCanvas* c[6];

  for(Int_t i=0; i<3; i++) {
    snprintf(str,100,"c%d",i+1);
    c[i] = new TCanvas(str,str,600,600);
    //h[i][3]->SetXTitle(xtitle[i]);
    h[i][0]->SetMinimum(1e-3);
    h[i][0]->SetLineColor(kBlack);
    h[i][0]->SetMarkerStyle(8);
    h[i][0]->SetMarkerSize(0.7);
    double wid = h[i][0]->GetBinWidth(1);
    if(wid<0.01)     snprintf(str,100,"Events / %.4f",wid);
    else if(wid<0.1) snprintf(str,100,"Events / %.3f",wid);
    else if(wid<1.0) snprintf(str,100,"Events / %.2f",wid);
    else if(wid<10.) snprintf(str,100,"Events / %.1f",wid);
    else             snprintf(str,100,"Events / %.0f",wid);
    h[i][0]->SetYTitle(str);
    h[i][0]->GetXaxis()->SetTitle("cos(#theta)");
    h[i][0]->Draw("E1");
    h[i+6][0]->SetLineColor(kRed);
    h[i+6][0]->SetLineWidth(3);
    h[i+6][0]->Draw("E1 SAME");
    h[i+3][0]->SetLineColor(kBlue);
    h[i+3][0]->SetLineWidth(3);
    h[i+3][0]->Draw("HIST SAME");
    float labelX = 0.20;
    if(i==0)      myText(labelX,0.85,kBlack,Form("Run2"));
    else if(i==1) myText(labelX,0.85,kBlack,Form("Run3"));
    else          myText(labelX,0.85,kBlack,Form("Run2+Run3")); 

    TLegend* leg = new TLegend(0.65, 0.7, 0.9, 0.9);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.03);
    leg->AddEntry(h[i][0],"Data","lep"); 
    leg->AddEntry(h[i+3][0],"SB","l");  
    leg->AddEntry(h[i+6][0],"Data-SB","lep");
    leg->Draw();

  }


  for(Int_t i=0; i<3; i++) {
    h[i][1]->Scale(0.03);
    h[i+3][1]->Scale(0.03);
    h[i+6][1]->Scale(0.03);
    snprintf(str,100,"c%d",i+4);
    c[i+3] = new TCanvas(str,str,600,600);
    //h[i][3]->SetXTitle(xtitle[i]);
    h[i][1]->SetMinimum(1e-3);
    h[i][1]->SetLineColor(kBlack);
    h[i][1]->SetMarkerStyle(8);
    h[i][1]->SetMarkerSize(0.7);
    double wid = h[i][1]->GetBinWidth(1);
    if(wid<0.01)     snprintf(str,100,"Events / %.4f",wid);
    else if(wid<0.1) snprintf(str,100,"Events / %.3f",wid);
    else if(wid<1.0) snprintf(str,100,"Events / %.2f",wid);
    else if(wid<10.) snprintf(str,100,"Events / %.1f",wid);
    else             snprintf(str,100,"Events / %.0f",wid);
    h[i][1]->SetYTitle(str);
    h[i][1]->GetXaxis()->SetTitle("cos(#theta)");
    h[i][1]->Draw("E1");

    h[i+6][1]->SetLineColor(kRed);
    h[i+6][1]->SetLineWidth(3);
    h[i+6][1]->Draw("PE1 SAME");
    h[i+3][1]->SetLineColor(kBlue);
    h[i+3][1]->SetLineWidth(3);
    h[i+3][1]->Draw("HIST SAME");
    float labelX = 0.20;
    if(i==0)      myText(labelX,0.85,kBlack,Form("Run2"));
    else if(i==1) myText(labelX,0.85,kBlack,Form("Run3"));
    else          myText(labelX,0.85,kBlack,Form("Run2+Run3")); 

    TLegend* leg = new TLegend(0.50, 0.7, 0.80, 0.9);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.03);
    leg->AddEntry(h[i][1],"Data,Eff Cor.","lep"); 
    leg->AddEntry(h[i+3][1],"SB,Eff Cor.","l");  
    leg->AddEntry(h[i+6][1],"Data-SB,Eff Cor.","lep");
    leg->Draw();

  }
}

/*
  if(read_ratio) { 
    TFile *fin = new TFile("../ratio_values.root", "READ");
    TH1D *h_ratio = (TH1D*)fin->Get("h_ratio");
    h_ratio->SetDirectory(0); 
    fin->Close();    
 
    for(Int_t j=1; j<=h[14][3]->GetNbinsX(); j++) {
      Float_t ratio_value = h_ratio->GetBinContent(j);
      if(ratio_value > 0) {
        Float_t corrected_value = h[14][3]->GetBinContent(j) / ratio_value;
        h[14][3]->SetBinContent(j, corrected_value);
      }
    } 
  }
*/

