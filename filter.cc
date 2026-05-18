#include <TFile.h>
#include <TTree.h>
#include <vector>
#include <string>
#include <set>
#include <iostream>
void filter_data24_hlt(const char* infile =
    "../../../../mix3/ntuple/data24_BphysDelayed_baseline.root",
    const char* outfile =
    "./data24_BphysDelayed_baseline_noC.root")
{
  const std::set<std::string> drop = {
    "HLT_mu22_mu8noL1_L1MU14FCH",
    "HLT_2mu6_bBmumux_BdmumuKst_L1LFV-MU5VF",
    "HLT_3mu4_bJpsi_L13MU3VF",

    "HLT_2mu4_bBmumux_BdmumuKst_L1BPH-2M9-0DR15-2MU3VF"
  };
  const std::set<std::string> keep = {
    "HLT_mu20_bBmux_BpmuD0X_L1MU14FCH",
    "HLT_mu11_mu6_bJpsimumu_L1MU8VF_2MU5VF",
    "HLT_2mu6_bJpsimumu_L1BPH-2M9-2DR15-2MU5VF",
    "HLT_mu6_mu4_bBmumux_BdmumuKst_L1BPH-2M9-0DR15-C-MU5VFMU3V",
    "HLT_mu6_mu4_bBmumux_BpmumuKp_L1BPH-2M9-0DR15-C-MU5VFMU3V",
    //"HLT_2mu4_bBmumux_BdmumuKst_L1BPH-2M9-0DR15-2MU3VF"
  };
  TFile* fin = TFile::Open(infile);
  TTree* tin = (TTree*)fin->Get("BPHY25");
  std::vector<std::string>* HLT_name = nullptr;
  tin->SetBranchAddress("HLT_name", &HLT_name);
  TFile* fout = TFile::Open(outfile, "RECREATE");
  TTree* tout = tin->CloneTree(0);
  Long64_t n = tin->GetEntries(), nkeep = 0;
  for (Long64_t i = 0; i < n; ++i) {
    tin->GetEntry(i);
    bool pass_keep = false;
    for (const auto& h : *HLT_name)
      if (keep.count(h)) { pass_keep = true; break; }
    if (pass_keep) { tout->Fill(); ++nkeep; }
  }
  fout->cd();
  tout->Write();
  fout->Close();
  fin->Close();
  printf("Wrote %lld / %lld entries to %s\n", nkeep, n, outfile);
}
