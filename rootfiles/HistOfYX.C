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
#include <TFile.h>
void HistOfYX()
{
    TFile *f = new TFile("out4.root");
    std::cout <<"File name: " << f->GetName() << std::endl;
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
    t->SetBranchAddress("Particle", &Particle);

    TH2F *h1 = new TH2F("h1", "h1", 100, -60, 60, 100, 0, 1900);

    TH2F *h2_1 = new TH2F("gamma", "gamma", 10, -5, 5, 10, -5, 5); // gamma
    TH2F *h2_2 = new TH2F("e-", "e-", 10, -5, 5, 10, -5, 5);       // electron
    TH2F *h2_3 = new TH2F("e+", "e+", 10, -5, 5, 10, -5, 5);       // positron
//y z gamma
    TH1F *h3_1 = new TH1F("HistOfEnergy", "HistOfEnergy", 100, -10, 10); // y
    TH1F *h3_2 = new TH1F("HistOfEnergy", "HistOfEnergy", 100, -10, 10); // 
    // TH1F *h4  = new TH1F("h3_1","h3_1",300, -100,100) ;
    Int_t ent = t->GetEntries();
    for (Int_t i = 0; i < ent; i++)
    {
        t->GetEntry(i);
        Double_t R = sqrt(pow(z, 2) + pow(y, 2));
        if (x >= -2.1 && x <= -1.9)
        {
            if (strcmp(Particle, "gamma") == 0)
            {
                h2_1->Fill(y, z);
                h3_1->Fill(y);
                h3_2->Fill(z);
            }

        }

    }

    t->ResetBranchAddresses();
    TCanvas *c = new TCanvas;
    c->Divide(3, 1);
    c->cd(1);
    h2_1->SetTitle("Hits");
    h2_1->SetYTitle("y");
    h2_1->SetXTitle("z");
    h2_1->Draw("colz");

    c->cd(2);
    h3_1->SetXTitle("y");
    h3_1->SetYTitle("Event");
    h3_1->Draw();

    c->cd(3);
    h3_2->SetXTitle("z");
    h3_2->SetYTitle("Event");
    h3_2->Draw();
    c->Update();
    
}