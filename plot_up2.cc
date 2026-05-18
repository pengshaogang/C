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


Bool_t passFiducial(Bool_t isAnaCodeVersion=false) {
  Int_t pass_fiducial = false;
  if(isAnaCodeVersion) {
    pass_fiducial = truth_Bs2_pt/GeV>15 && fabs(truth_Bs2_eta)<2.5;
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

void plot() {
  TChain* ch[1] = {nullptr};
  //mc
  ch[0] = new TChain("truthTree");
  if(isRun3==-1 || isRun3==0) {
    ch[0]->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_nofilter/all/mc20_802829/mytruth.root.1");
    ch[0]->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_nofilter/all/mc20_802829/mytruth.root.2");
    ch[0]->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_nofilter/all/mc20_802829/mytruth.root.3");
    ch[0]->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_nofilter/all/mc20_802829/mytruth.root.4");
 
  }  
  if(isRun3==-1 || isRun3==1) {
    ch[0]->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_nofilter/all/mc23_802829/mytruth.root.1");
    ch[0]->Add("/Users/pengshaogang/Documents/analysis/bphy25/B0Ks/ntuple_lowpt/channel34_0_nofilter/all/mc23_802829/mytruth.root.2");
  }
  Init_t(ch[0]);

  char str[100];
  TH1F* h[3][3];
  TH1F* h_ratio[3];
  TLegend* leg[3];


  for(Int_t i=0; i<3; i++) {
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
 
  Long64_t nentries3 = ch[0]->GetEntries();
  printf("%llu entries to be processed...\n",nentries3);
  for(Long64_t entry=0; entry<nentries3; entry++) {
    ch[0]->GetEntry(entry);
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
        //if(truth_pT[i]/GeV>15 && fabs(truth_eta[i])<2.5) {
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
	//}	
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
    //if(ch[0]->GetTreeNumber()<=3) h[0][0]->Fill(ct_truth,weight);
    //if(ch[0]->GetTreeNumber()>3) h[1][0]->Fill(ct_truth,weight);
    //h[2][0]->Fill(ct_truth,weight);
    if(passFiducial(false)) {
      if(ch[0]->GetTreeNumber()<=3) h[0][0]->Fill(ct_truth,weight);
      if(ch[0]->GetTreeNumber()>3) h[1][0]->Fill(ct_truth,weight);
      h[2][0]->Fill(ct_truth,weight);  
      if(passFilter(false)) {
        if(ch[0]->GetTreeNumber()<=3) h[0][1]->Fill(ct_truth,weight); 
        if(ch[0]->GetTreeNumber()>3) h[1][1]->Fill(ct_truth,weight);
        h[2][1]->Fill(ct_truth,weight);   
      }
    }
  }

  for(Int_t i=0; i<3; i++) {
    snprintf(str,100,"hratio%d",i+1);
    h_ratio[i] = new TH1F(str,"",h[i][0]->GetNbinsX(),h[i][0]->GetXaxis()->GetXmin(),h[i][0]->GetXaxis()->GetXmax());
    leg[i] = new TLegend(0.6, 0.7, 0.9, 0.9);
  }
 

  TCanvas* c[3];
  TPad* padhigh[3];
  TPad* padlow[3];

  
  
  for(Int_t i=0; i<3; i++) {
    snprintf(str,100,"c%d",i+1);  
    c[i] = new TCanvas(str,str,600,600);

    padhigh[i] = new TPad(str,str,0,0.3,1,1);
    padhigh[i]->Draw();
    padhigh[i]->cd();
    padhigh[i]->SetGrid(0,0);
    padhigh[i]->SetBottomMargin(0.0);

    if(h[i][1]->Integral()>0 && h[i][0]->Integral()>0) h[i][1]->Scale(h[i][0]->Integral() * 0.5 / h[i][1]->Integral());

    //h[i][0]->Scale(18.5);
    //h[i][1]->Scale(18.5); 

    double ymax = TMath::Max(h[i][0]->GetMaximum(), h[i][1]->GetMaximum());
  
    h[i][0]->SetMinimum(1e-3);
    h[i][0]->SetMaximum(1.3*ymax);
    h[i][0]->SetLineColor(kBlue);
    h[i][0]->SetLineWidth(3);
    h[i][0]->SetMarkerStyle(9);
    h[i][0]->SetMarkerSize(0.8);
    h[i][0]->GetXaxis()->SetTitleSize(0);
    h[i][0]->GetXaxis()->SetLabelSize(0); 

    double wid = h[i][0]->GetBinWidth(1);
    if(wid<0.01)     snprintf(str,100,"Events / %.4f",wid);
    else if(wid<0.1) snprintf(str,100,"Events / %.3f",wid);
    else if(wid<1.0) snprintf(str,100,"Events / %.2f",wid);
    else if(wid<10.) snprintf(str,100,"Events / %.1f",wid);
    else             snprintf(str,100,"Events / %.0f",wid);
    h[i][0]->SetYTitle(str);
    h[i][0]->Draw("hist");
    h[i][1]->SetLineColor(kRed); h[i][1]->SetLineWidth(3);
    h[i][1]->Draw("hist same");

    leg[i]->SetFillStyle(0);              
    leg[i]->SetBorderSize(0);             
    leg[i]->SetTextSize(0.04);             
    leg[i]->AddEntry(h[i][0], "Fiducial", "l");     
    //leg[i]->AddEntry(h[i][1], "Fiducial", "l");     
    leg[i]->AddEntry(h[i][1], "Filter", "l");
    leg[i]->Draw();

    float labelX = 0.16;
    //ATLASLabel(labelX,0.85,"Internal",kBlack);
    //float textHeight = 0.06*(672./padhigh[i]->GetWh());
    if(i==0) myText(labelX,0.85,kBlack,Form("#sqrt{s} = 13 TeV, 140 fb^{-1}"));
    else if(i==1) myText(labelX,0.85,kBlack,Form("#sqrt{s} = 13.6 TeV, 175fb^{-1}"));
    else myText(labelX,0.85,kBlack,Form("#sqrt{s} = 13/13.6 TeV, 140+175 fb^{-1}"));

    c[i]->cd();
    snprintf(str,100,"padlow%d",i+1);
    padlow[i] = new TPad(str,str,0,0,1,0.3);
    padlow[i]->SetFillStyle(4000);
    padlow[i]->SetGrid(1,1);
    padlow[i]->SetTopMargin(0.0);
    padlow[i]->SetBottomMargin(0.35);
    padlow[i]->Draw();
    padlow[i]->cd();
    //TCanvas* c2 = new TCanvas("c2", "Ratio Plot", 600, 600);
    //h_ratio->SetLineColor(kBlack);
    //h_ratio->Draw();
    for(Int_t j=1; j<=h_ratio[i]->GetNbinsX(); j++) {
      Float_t tmp1 = h[i][0]->GetBinContent(j)>0 ? h[i][0]->GetBinError(j)/h[i][0]->GetBinContent(j) : 0;
      Float_t tmp2 = h[i][1]->GetBinContent(j)>0 ? h[i][1]->GetBinError(j)/h[i][1]->GetBinContent(j) : 0;
      Float_t tmp = h[i][0]->GetBinContent(j)>0 ? h[i][1]->GetBinContent(j)/h[i][0]->GetBinContent(j) : 0;
      h_ratio[i]->SetBinContent(j,tmp); 
      h_ratio[i]->SetBinError(j,tmp*sqrt(pow(tmp1,2)+pow(tmp2,2)));
      //cout << "tmp = " << tmp << endl;
    }

    h_ratio[i]->GetXaxis()->SetLabelFont(42);
    h_ratio[i]->GetXaxis()->SetLabelSize(0.12);
    h_ratio[i]->GetXaxis()->SetLabelOffset(0.02);
    h_ratio[i]->GetXaxis()->SetTitleOffset(1.0);
    h_ratio[i]->GetXaxis()->SetTitleSize(0.15);
    h_ratio[i]->GetYaxis()->SetLabelFont(42);
    h_ratio[i]->GetYaxis()->SetLabelSize(0.12);
    h_ratio[i]->GetYaxis()->SetTitleOffset(0.45);
    h_ratio[i]->GetYaxis()->SetTitleSize(0.12);
    h_ratio[i]->SetLineColor(kRed);
    h_ratio[i]->SetLineWidth(3);
    h_ratio[i]->SetMarkerStyle(8);
    h_ratio[i]->SetMarkerSize(0.6);
    h_ratio[i]->SetMaximum(1.0-1e-3);
    h_ratio[i]->SetMinimum(0.0+1e-3);
    h_ratio[i]->GetYaxis()->SetNdivisions(505);
    h_ratio[i]->GetYaxis()->SetTitle("Ratio");
    h_ratio[i]->GetYaxis()->CenterTitle();
    h_ratio[i]->GetXaxis()->SetTitle("cos(#theta)");
    h_ratio[i]->Draw("hist");

 
  }


}
 

