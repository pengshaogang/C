   // Declaration of leaf types
   Int_t           mcChannel;
   Int_t           EventNumber;
   Float_t         mc_event_weight;
   Int_t           truth_n;
   Int_t           truth_pdgId[26];   //[truth_n]
   Int_t           truth_motherID[26];   //[truth_n]
   Double_t        truth_mass[26];   //[truth_n]
   Double_t        truth_pT[26];   //[truth_n]
   Double_t        truth_eta[26];   //[truth_n]
   Double_t        truth_phi[26];   //[truth_n]

   // List of branches
   TBranch        *b_mcChannel;   //!
   TBranch        *b_EventNumber;   //!
   TBranch        *b_mc_event_weight;   //!
   TBranch        *b_truth_n;   //!
   TBranch        *b_truth_pdgId;   //!
   TBranch        *b_truth_motherID;   //!
   TBranch        *b_truth_mass;   //!
   TBranch        *b_truth_pT;   //!
   TBranch        *b_truth_eta;   //!
   TBranch        *b_truth_phi;   //!


void Init_t(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   TTree* fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("mcChannel", &mcChannel, &b_mcChannel);
   fChain->SetBranchAddress("EventNumber", &EventNumber, &b_EventNumber);
   fChain->SetBranchAddress("mc_event_weight", &mc_event_weight, &b_mc_event_weight);
   fChain->SetBranchAddress("truth_n", &truth_n, &b_truth_n);
   fChain->SetBranchAddress("truth_pdgId", truth_pdgId, &b_truth_pdgId);
   fChain->SetBranchAddress("truth_motherID", truth_motherID, &b_truth_motherID);
   fChain->SetBranchAddress("truth_mass", truth_mass, &b_truth_mass);
   fChain->SetBranchAddress("truth_pT", truth_pT, &b_truth_pT);
   fChain->SetBranchAddress("truth_eta", truth_eta, &b_truth_eta);
   fChain->SetBranchAddress("truth_phi", truth_phi, &b_truth_phi);
}

