#include <iostream>
#include <fstream>
#include <cmath>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TF1.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TTree.h>
#include <cstring>
void analysis()
{
    TFile *f = new TFile("out.root");
    TTree *t = (TTree *)f->Get("Data");
    Double_t z;
    Double_t y;
    Double_t x;
    Double_t Energy;
    Char_t Particle[256];
    t->SetBranchAddress("y", &y);
    t->SetBranchAddress("z", &z);
    t->SetBranchAddress("x", &x);
    t->SetBranchAddress("Energy", &Energy);
    t->SetBranchAddress("Particle", Particle);

    TH2F *h1 = new TH2F("h1", "h1", 100, -60, 60, 100, 0, 1900);
    TH2F *h2 = new TH2F("h2", "h2", 50, -60, 60, 50, -60, 60);
    TH1F *h3 = new TH1F("HistOfEnergy", "HistOfEnergy", 100, 0, 5000); // energy
    // TH1F *h4  = new TH1F("h3","h3",300, -100,100) ;
    Int_t ent = t->GetEntries();
    for (Int_t i = 0; i < ent; i++)
    {
        t->GetEntry(i);      
        if (strcmp(Particle, "e+") == 0)
        {
            cout << Particle << endl;
        }
        
       
       
        if (x >= -1.1 && x <= -0.9 && strcmp(Particle, "e-") == 0)
        {
            h1->Fill(z, Energy);
            h2->Fill(y, z);
            h3->Fill(Energy);
        }
    }

    t->ResetBranchAddresses();
    TCanvas *c = new TCanvas;
    // c->Divide(2);
    h3->GetYaxis()->SetTitle("Event");
    h3->GetXaxis()->SetTitle("Energy, MeV");
    h3->SetTitle("Energy");

    // c->cd(1);
    // h3->Draw();
    // c->cd(2);
    // h2->Draw("colz");

    h2->Draw("colz");
    // h1->Draw("colz");
    c->Update();
}
