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
    TFile *f = new TFile("out.root");
    std::cout <<"File name: " << f->GetName() << std::endl;
    TTree *t = (TTree *)f->Get("Data");
    Double_t z;
    Double_t y;
    Double_t x;
    Double_t Energy;
    Char_t Particle[256];
    Char_t Detector[256];
    t->SetBranchAddress("y", &y);
    t->SetBranchAddress("z", &z);
    t->SetBranchAddress("x", &x);
    t->SetBranchAddress("Energy", &Energy);
    t->SetBranchAddress("Particle", &Particle);
    t->SetBranchAddress("Detector", &Detector);

    // TH2F *h1 = new TH2F("h1", "h1", 100, -60, 60, 100, 0, 1900);

    // TH2F *hHits = new TH2F("e-", "e-", 200, -200, 200, 200, -200, 200); 
    TH1F *hEnergyEl = new TH1F("e-", "e-", 200, 0, 4700);
    TH1F *hEnergyPo = new TH1F("e+", "e+", 200, 0, 4700);
    // TH1F *hEnergyEl = new TH1F("e-", "e-", 300, 0, 4700);
    TH2F *h1 = new TH2F("","", 20, -500, 500, 20, 0, 800);
    TH2F *h2 = new TH2F("","", 20, -500, 500, 20, -500, 500);
    TH1F *h3 = new TH1F("","", 50, 100, 700);

    Int_t ent = t->GetEntries();

    for (Int_t i = 0; i < ent; i++)
    {
        t->GetEntry(i);
        // Double_t R = sqrt(pow(z, 2) + pow(y, 2));
        if (strcmp(Detector, "det2") == 0 && strcmp(Particle, "e-") == 0){
            h1->Fill(z, Energy);
            h2->Fill(z, y);
            h3->Fill(Energy);
            cout << "Particle: " << Particle << "; Energy: " << Energy << endl;
        }
        if (true)
        {
            if (strcmp(Particle, "e-") == 0)
            {
                hEnergyEl->Fill(Energy);
            }
            if (strcmp(Particle, "e+") == 0)
            {
                hEnergyPo->Fill(Energy);
            }

        }

    }
    t->ResetBranchAddresses();
    TCanvas *c = new TCanvas;
    gStyle->SetOptFit();
    c->Divide(3, 2);
    c->cd(1);
    gPad->SetCrosshair();
    // hEnergyPo->SetTitle("Hits");
    hEnergyPo->SetYTitle("Event");
    hEnergyPo->SetXTitle("Energy");
    hEnergyPo->Draw("colz");
    hEnergyPo->SetFillColor(23);
    gPad->SetLogy();

    c->cd(2);
    hEnergyEl->SetXTitle("Energy");
    hEnergyEl->SetYTitle("Event");
    hEnergyEl->Draw();
    hEnergyEl->SetFillColor(23);
    gPad->SetLogy();

    c->cd(3);
    h1->SetXTitle("X [mm]");
    h1->SetYTitle("Energy [MeV]");
    h1->Draw("colz");

    c->cd(4);
    h2->SetXTitle("X [mm]");
    h2->SetYTitle("Y [mm]");
    h2->Draw("colz");

    c->cd(5);
    h3->SetTitle("Electron energy spectrum");
    h3->SetXTitle("Energy [MeV]");
    h3->SetYTitle("Event");
    h3->SetFillColor(23);
    h3->Draw();
    c->Update();

    //work with data
    
    // double hmax = gaussian->GetMaximum();
    // double halfmax = hmax/2;
    // double x1, x2;
    // x1 = gaussian->GetX(halfmax, -12, gaussian->GetParameter(1));
    // x2 = gaussian->GetX(halfmax, gaussian->GetParameter(1), 12);

    // double sigma = (x2-x1)/(2*sqrt(2*log(2)));

    
}
