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
    TFile *f = new TFile("out5.root");
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

    TH2F *h2_1 = new TH2F("gamma", "gamma", 30, -5, 5, 30, -5, 5); // gamma
    TH2F *h2_2 = new TH2F("e-", "e-", 10, -5, 5, 10, -5, 5); // electron
    TH2F *h2_3 = new TH2F("e+", "e+", 10, -5, 5, 10, -5, 5); // positron

    TH1F *h3_1 = new TH1F("HistOfEnergy", "HistOfEnergy", 200, -60, 60); // energy //gamma
    TH1F *h3_2 = new TH1F("HistOfEnergy", "HistOfEnergy", 100, 0, 5000); // electron
    TH1F *h3_3 = new TH1F("HistOfEnergy", "HistOfEnergy", 100, 0, 5000); // positron
    // TH1F *h4  = new TH1F("h3_1","h3_1",300, -100,100) ;
    Int_t ent = t->GetEntries();
    for (Int_t i = 0; i < ent; i++)
    {
        Double_t R = sqrt(pow(z, 2) + pow(y, 2));
        if (x >= -2.1 && x <= -1.9 )
        {
            if (strcmp(Particle, "gamma") == 0)
            {
                h2_1->Fill(y, z);
                h3_1->Fill(y);
            }
            if (strcmp(Particle, "e-") == 0)
            {
                h2_2->Fill(y, z);
                h3_2->Fill(Energy);
            }
            if (strcmp(Particle, "e+") == 0)
            {
                h2_3->Fill(y, z);
                h3_3->Fill(Energy);
            }
        }
        t->GetEntry(i);
    }

    t->ResetBranchAddresses();
    TCanvas *c = new TCanvas;
    c->Divide(3, 2);
    h3_1->GetYaxis()->SetTitle("Event");
    h3_1->GetXaxis()->SetTitle("Energy, MeV");
    h3_1->SetTitle("Energy(gamma)");
    h3_1->SetFillColor(kGray);
    c->cd(1);
    // gPad->SetLogy();
    h3_1->Draw();

    h3_2->GetYaxis()->SetTitle("Event");
    h3_2->GetXaxis()->SetTitle("Energy, MeV");
    h3_2->SetTitle("Energy(e-)");
    h3_2->SetFillColor(kGray);
    c->cd(2);
    gPad->SetLogy();
    h3_2->Draw();

    h3_3->GetYaxis()->SetTitle("Event");
    h3_3->GetXaxis()->SetTitle("Energy, MeV");
    h3_3->SetTitle("Energy(e+)");
    h3_3->SetFillColor(kGray);
    c->cd(3);
    gPad->SetLogy();
    h3_3->Draw();

    c->cd(4);
    h2_1->Draw("colz");

    c->cd(5);
    h2_2->Draw("colz");

    c->cd(6);
    h2_3->Draw("colz");
    // c->cd(2);
    // h2->Draw("colz");

    // h2->Draw("colz");
    // h1->Draw("colz");
    c->Update();
}
